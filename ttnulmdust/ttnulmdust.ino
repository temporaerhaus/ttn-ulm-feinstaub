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

// uncomment to use BME280 weather sensor
// #define BME280_SENSOR

#ifdef BME280_SENSOR
    // default I2C address for the BME280 is 0x77
    // if your chinese import does not work, try to use 0x76 instead
    #define BME_ADDRESS 0x76 

    #include "BME280_Sensor.h"
#else
    // DHT configuration
    #define DHTPIN 10      // connect the DHT22 PIN to this The Things Uno PIN
    #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

    #include <DHT_Sensor.h>
#endif

#include <TheThingsNetwork.h>

//*******************************
// User variables
//*******************************

// copy and paste these values from your TTN console application
const char *appEui = "";
const char *appKey = "";

// PIN configuration for the SDS011 dust sensor
#define PIN_RX 8       // connect the SDS011 RX pin to this The Things Uno pin
#define PIN_TX 9       // connect the SDS011 TX pin to this The Things Uno pin

#define SLEEP_ON 1     // if the fan should go to sleep
#define SLEEP_TIME  5  // sleep for x minutes between readings
#define FAN_SPINUP 30  // how long should the fan 'clean' itself before measurements are taken (if SLEEP_ON = 1)
#define PWR_DOWN 0     // set to 1 to use power down mode of µC, new flash needs manual reset

//***************************************************
// You don't need to change anyhting below this line
//***************************************************

#define DEBUGRATE 9600
#define LORA_RATE 57600

#define loraSerial Serial1
#define debugSerial Serial

#ifdef BME280_SENSOR
    BME280_Sensor weatherSensor(debugSerial, BME_ADDRESS); // I2C - connect SCL to SCL and SDA to SDA
#else
    DHT_Sensor weatherSensor(debugSerial, DHTPIN, DHTTYPE);
#endif

// Power saving
#include "sleep_32u4.h"

// sleep time between fan spinup and sleep in minutes.
volatile uint16_t iWakeCntr = 0;

// WDT ISR
ISR(WDT_vect) 
{
	if (iWakeCntr > UINT16_MAX-1) 
    {
		iWakeCntr = 0;
    } 
    else 
    {
        iWakeCntr++;
    }
}

// TTN settings (no editing needed)
const ttn_fp_t freqPlan = TTN_FP_EU868;
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

// SDS011, paticulate matter variables (no editing needed)
float p10, p25;
float samples_p10[10];
float samples_p25[10];
int error;
SDS011 my_sds;

void setup()
{
	debugSerial.begin(DEBUGRATE);
    loraSerial.begin(LORA_RATE);    

    while (!debugSerial && millis() < 10000);

    // fine dust
    debugSerial.println(F("Started!"));
	my_sds.begin(PIN_RX, PIN_TX);

    // start the weather sensor
    weatherSensor.setup();

	#if PWR_DOWN
	// for debugging
	pinMode(13, OUTPUT);
	setupPowerMode();
	#endif

    ttn.showStatus();
    ttn.join(appEui, appKey); // OTAA
}

void loop() {
	digitalWrite(13, HIGH);

    // **************************
    // Wake up and fan speed up
    // **************************
    // wake up the sensor
    #if SLEEP_ON
    debugSerial.println(F("Waking up SDS..."));	
    my_sds.wakeup();

    // let the fan run for a minute to clean the fan
    debugSerial.println(F("Letting fan speed up and clean itself for a minute..."));
    long delay1 = 1000L * FAN_SPINUP;
    delay(delay1);
    #endif

    // ******************************************************************
    // Read weather sensor data (temperature / humidity)
    // ******************************************************************
    int16_t hint = weatherSensor.readHumidity();
    int16_t tint = weatherSensor.readTemperature();

    // **********************
    // SDS011
    // **********************
    debugSerial.println(F("Reading 10 samples of sensor data (some may fail)..."));

    // read pm25 and pm10 values from the sensor
    long delay2 = 1100;
    long samples = 10;

    for (int i = 0; i < samples; i++) 
    {
        error = my_sds.read(&p25, &p10);
        if (!error) 
        {
            samples_p25[i] = p25;
            samples_p10[i] = p10;

            debugSerial.println("P2.5: " + String(p25));
            debugSerial.println("P10:  " + String(p10));
        } 
        else 
        {
            debugSerial.println(F("error reading data!"));
        }

        delay(delay2);
    }

    float p25median = median(samples_p25, 10);
    float p10median = median(samples_p10, 10);

    // Encode float as int (20.98 becomes 2098)
    int16_t p10int = round(p10median * 100);
    int16_t p25int = round(p25median * 100);

    // show the median on the debug console
    debugSerial.print(F("P2.5 median: "));
    debugSerial.println(String(p25median));
    debugSerial.print(F("P10  median: "));
    debugSerial.println(String(p10median));

    // **********************
    // TTN
    // **********************
    // Encode int as bytes
    byte payload[8];

    // sds011
    payload[0] = highByte(p10int);
    payload[1] = lowByte(p10int);
    payload[2] = highByte(p25int);
    payload[3] = lowByte(p25int);

    // sensor (temperature / humidity)
    payload[4] = highByte(hint); // humidity
    payload[5] = lowByte(hint);
    payload[6] = highByte(tint); // temperature
    payload[7] = lowByte(tint);

    // send via TTN
    debugSerial.println(F("Sending data to TTN..."));
    ttn.sendBytes(payload, sizeof(payload));

    // **********************
    // Sleep
    // **********************
    #if SLEEP_ON
    // put sensor to sleep so save battery
    debugSerial.println(F("Sending SDS to sleep..."));
    my_sds.sleep();

    // sleep for a few minutes to save energy
    #if PWR_DOWN
	//delay(5000);
	enterSleepFor(SLEEP_TIME);
	#else
	delay((1000L * 60 * SLEEP_TIME) - (delay1 + samples * delay2)); // substract wakup time to prevent heavy drift
    #endif

    #endif
}

//*****************************
// Helper functions
//*****************************

// calculate the median
float median(float samples[], int m)
 {
    float sorted[m];

    for(int i = 0; i < m; i++)
    {
        sorted[i] = samples[i];
    }

    bubbleSort(sorted, m);

    if ((m & 0x01) == 0)
    {
        // even number of elements
        return (sorted[(m / 2) - 1] + sorted[m / 2]) / 2;        
    } 
    else
    {
        // odd number of elements
        return sorted[m / 2];
    }
}

// sort array with bubble sort (needed for the median)
void bubbleSort(float A[], int len) {
    unsigned long newn;
    unsigned long n = len;
    float temp = 0.0;

    do {
        newn = 1;
        
        for(int p = 1; p < len; p++)
        {
            if (A[p - 1] > A[p]) 
            {
                temp = A[p];
                A[p] = A[p - 1];
                A[p - 1] = temp;
                newn = p;
            }
        }

        n = newn;
    } while (n > 1);
}
