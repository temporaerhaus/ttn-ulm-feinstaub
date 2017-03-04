/*
	Sleepfunctions for Arduino Leonardo (ATmega32u4)
	!!! experimental !!!
	sleep mode leads to shutdown of usb port, atm it wont wake up again.
	New flash is only possible by reseting the board manually.
*/

#define SLEEP_FACTOR 15

static uint16_t iSleepTimeout = 0;
extern volatile uint16_t iWakeCntr;

// set sleep timeout in minutes
static bool setSleepTime(uint16_t iSleep){
	if(iSleep <= UINT16_MAX/SLEEP_FACTOR){
		iSleepTimeout = iSleep*SLEEP_FACTOR;
		Serial.println("Device sleep time set to "+String(iSleepTimeout/SLEEP_FACTOR)+" minutes");
		return true;
	}
	return false;
}

// deactivate functions to save energy while running the program
// e.g. if you don't use i2c deactivate the TWI
void setupPowerMode(){
	// prepare power down mode
	ADCSRA &= ~(1<<7);		// disable ADCSRA
	ACSR = 0b10000000;		// disable analog comperator
	SMCR |= (1<<SM1);			// setup sleep mode
	
	// disable digital input buffer in adc pins
	//DIDR0 = 0b11111111;		// disable ADC 0-7
	//DIDR1 = 0b00000011;		// disable AIN 0,1
	//DIDR2 = 0b11111111;		// disable ADC 8-15
	
	// disable not needed functions
	//PRR0 = 0b10000001;		// disable TWI, Timer2, ADCSRA
	//PRR1 = 0b00111000;		// disable Timer5, Timer4, Timer3
	
	// setup watchdog prescaler to 4.0s
	WDTCSR = 0b00011000;	
	WDTCSR = 0b00100000;	
}

// deactivate used functions before sleep, set outputs to inputs to save extra energy
static void prepareSleep(bool bSleep){
	if(bSleep){
		Serial.end();
		Serial1.end();
		//setupPowerMode();
		digitalWrite(13, LOW);
		
		WDTCSR |= (1<<WDIE);		// enable WDT
		SMCR |= 1;
	}
	else{
		SMCR &= 0xE;
		MCUSR &= ~(1<<WDRF);
		WDTCSR &= (0<<WDIE);		// disable WDT
		
		Serial1.begin(LORA_RATE);
		Serial.begin(DEBUGRATE);		
	}
	
	delay(10);	
}

// send µC to sleep
bool enterSleepFor(uint16_t iSleep){
	if(setSleepTime(iSleep)){
		Serial.println("Sleeping for " + String(iSleepTimeout/SLEEP_FACTOR) + " minutes...");
		prepareSleep(true);
	
		while(true){
			if(iWakeCntr >= iSleepTimeout)
				break;
			
			__asm__ __volatile__("sleep");
		}
		prepareSleep(false);
		Serial.println("Ticker: "+String(iWakeCntr));
		iWakeCntr = 0;
		Serial.println("back from sleep");
		
		return true;
	}
	else
		Serial.println("Invalid sleep timeout");
		
	return false;
}
