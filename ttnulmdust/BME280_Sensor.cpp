#include <Arduino.h>
#include "BME280_Sensor.h"
#include "Adafruit_BME280.h"

BME280_Sensor::BME280_Sensor(Serial_ serial, uint8_t bme280Address)
{
    debugSerial = serial;
    address = bme280Address;
}

// ****************************************************************
// Initialize the weather sensor
// ****************************************************************
bool BME280_Sensor::setup()
{
	bool status = bme280.begin(address);  
    if (!status) 
    {
        debugSerial.println(F("Could not find a valid BME280 sensor, check wiring!"));
        while (1);
    }

    // set BME280 weather station mode (save some energy)
    bme280.setSampling(Adafruit_BME280::MODE_FORCED,
                        Adafruit_BME280::SAMPLING_X1, // temperature
                        Adafruit_BME280::SAMPLING_X1, // pressure
                        Adafruit_BME280::SAMPLING_X1, // humidity
                        Adafruit_BME280::FILTER_OFF);

    return true;
}

// ****************************************************************
//  read Temperature
// ****************************************************************
int16_t BME280_Sensor::readTemperature(void)
{
    float temperature = bme280.readTemperature();
    if (isnan(temperature))
    {
        return -1;
    }

    debugSerial.print(F("Temperature: "));
    debugSerial.println(String(temperature));

    return round(temperature * 100);
}

// ****************************************************************
//  read barometric pressure
// ****************************************************************
int16_t BME280_Sensor::readPressure(void)
{
    float pressure = bme280.readPressure();
    if (isnan(pressure))
    {
        return -1;
    }

    debugSerial.print(F("Pressure: "));
    debugSerial.println(String(pressure));

    return round(pressure * 100);
}

// ****************************************************************
//  read humidity
// ****************************************************************    
int16_t BME280_Sensor::readHumidity(void)
{
    float humidity = bme280.readHumidity();
    if (isnan(humidity))
    {
        return -1;
    }

    debugSerial.print(F("Humidity: "));
    debugSerial.println(String(humidity));

    return round(humidity * 100);
}