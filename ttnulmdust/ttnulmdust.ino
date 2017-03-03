/*****************************************************
 * TTN Ulm particulate matter sensor
 *
 * Reads data from a SDS011 sensor and send the data
 * to the TTN network via LoRaWAN.
 *
 * Works like this:
 * 1) sleeps for a given amount of time.
 * 2) spins up the fan for a minute to clean the fan
 *    and the housing.
 * 3) takes 10 samples of sensor data (pm2.5 and pm10)
 * 4) calculates the median of each to avoid outlier
 * 5) encodes the values into bytes
 * 6) send them to the given TTN application
 *
******************************************************/
#include "SDS011.h"
#include <TheThingsNetwork.h>

#define DEBUGRATE 9600
#define LORA_RATE 57600

#define loraSerial Serial1
#define debugSerial Serial

#include "sleep_32u4.h"
#define PWR_DOWN 0

// copy and paste these values from your TTN console application
const char *devAddr = "";
const char *nwkSKey = "";
const char *appSKey = "";

// sleep time between fan spinup and sleep in minutes.
//const int sleep_duration = 60;
const int sleep_duration = 5;		// sleep for 5 minutes
volatile uint16_t iWakeCntr = 0;

// WDT ISR
ISR(WDT_vect){
	if(iWakeCntr > UINT16_MAX-1)
		iWakeCntr = 0;		
	else
		iWakeCntr++;
}

// PIN configuration
int PIN_TX = 8; // connect the SDS011 TX pin to this Arduino pin
int PIN_RX = 9; // connect the SDS011 RX pin to this Arduino pin

// TTN settings (no editing needed)
const ttn_fp_t freqPlan = TTN_FP_EU868;
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

// paticulate matter variables (no editing needed)
float p10,p25;
float samples_p10[10];
float samples_p25[10];
int error;
SDS011 my_sds;

void setup() {
	debugSerial.begin(DEBUGRATE);
    loraSerial.begin(LORA_RATE);    

    while (!debugSerial && millis() < 10000);

    // fine dust
    debugSerial.println("Started!");
	my_sds.begin(PIN_TX, PIN_RX);
	
	#if PWR_DOWN
	// for debugging
	pinMode(13, OUTPUT);
	#endif

    ttn.personalize(devAddr, nwkSKey, appSKey);
    ttn.showStatus();
}

void loop() {
	digitalWrite(13, HIGH);
	
    // wake up the sensor
    debugSerial.println("Waking up SDS...");	
    my_sds.wakeup();

    // let the fan run for a minute to clean the fan
    debugSerial.println("Letting fan speed up and clean itself for a minute...");
    long delay1 = 1000L * 60;
    delay(delay1);

    debugSerial.println("Reading 10 samples of sensor data (some may fail)...");
    // read pm25 and pm10 values from the sensor
    long delay2 = 1100;
    long samples = 10;
    for (int i = 0; i < samples; i++) {
        error = my_sds.read(&p25,&p10);
        if (!error) {

            samples_p25[i] = p25;
            samples_p10[i] = p10;

            debugSerial.println("P2.5: "+String(p25));
            debugSerial.println("P10:  "+String(p10));
        } else {
            debugSerial.println("error reading data!");
        }
        delay(delay2);
    }

    float p25median = median(samples_p25, 10);
    float p10median = median(samples_p10, 10);

    // Encode float as int (20.98 becomes 2098)
    int16_t p10int = round(p10median * 100);
    int16_t p25int = round(p25median * 100);

    debugSerial.println("P2.5 median: "+String(p25median));
    debugSerial.println("P10  median: "+String(p10median));

    // Encode int as bytes
    byte payload[4];
    payload[0] = highByte(p10int);
    payload[1] = lowByte(p10int);
    payload[2] = highByte(p25int);
    payload[3] = lowByte(p25int);
    // send via TTN
    debugSerial.println("Sending data to TTN...");
    ttn.sendBytes(payload, sizeof(payload));

    // put sensor to sleep so save battery
    debugSerial.println("Sending SDS to sleep...");
    my_sds.sleep();

    // sleep for a few minutes to save energy
    #if PWR_DOWN
	//delay(5000);
	enterSleepFor(sleep_duration);
	#else
	delay((1000L * 60 * sleep_duration) - (delay1 + samples*delay2)); // substract wakup time to prevent heavy drift
	#endif
}



//*****************************
// Helper functions
//*****************************

// calculate the median
float median(float samples[], int m) {
    float sorted[m];
    float temp=0.0;

    for(int i=0;i<m;i++){
        sorted[i]=samples[i];
    }
    bubbleSort(sorted,m);
    if (bitRead(m,0)==1) {
        return sorted[m/2];
    } else {
        return (sorted[(m/2)-1]+sorted[m/2])/2;
    }
}

// sort array with bubble sort (needed for the median)
void bubbleSort(float A[], int len) {
    unsigned long newn;
    unsigned long n=len;
    float temp=0.0;
    do {
        newn=1;
        for(int p=1;p<len;p++){
            if(A[p-1]>A[p]){
                temp=A[p];
                A[p]=A[p-1];
                A[p-1]=temp;
                newn=p;
            }
        }
        n=newn;
    } while(n>1);
}