#include <Arduino.h>
#include "DHT_Sensor.h"
#include "DHT.h"

DHT_Sensor::DHT_Sensor(Serial_ serial, uint8_t pin, uint8_t type) : dht(pin, type)
{
    debugSerial = serial;
}

// ****************************************************************
// Initialize the weather sensor
// ****************************************************************
bool DHT_Sensor::setup()
{
    dht.begin();

    return true;
}

// ****************************************************************
//  read Temperature
// ****************************************************************
int16_t DHT_Sensor::readTemperature(void)
{
    float temperature = dht.readTemperature();
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
int16_t DHT_Sensor::readPressure(void)
{
    return -1;
}

// ****************************************************************
//  read humidity
// ****************************************************************
int16_t DHT_Sensor::readHumidity(void)
{
    float humidity = dht.readHumidity();
    if (isnan(humidity))
    {
        return -1;
    }

    debugSerial.print(F("Humidity: "));
    debugSerial.println(String(humidity));

    return round(humidity * 100);
}