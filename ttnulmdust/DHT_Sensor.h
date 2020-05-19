#ifndef __DHT_SENSOR_H__
#define __DHT_SENSOR_H__

#include <Arduino.h>
#include "DHT.h"

// ****************************************************************
//                      DHT Sensor class
//  used to move out the code for the sensors from the main sketch
//  NOTE: the methods shall return the normalized "int" values for
//        the data transmission
// ****************************************************************
class DHT_Sensor
{
    public:
        DHT_Sensor(Serial_ serial, uint8_t pin, uint8_t type);
        bool setup();
        int16_t readTemperature(void);
        int16_t readPressure(void);
        int16_t readHumidity(void);

    private:
        Serial_ debugSerial;
        DHT dht;
};

#endif