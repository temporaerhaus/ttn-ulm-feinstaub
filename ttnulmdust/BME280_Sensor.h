#ifndef __BME280_SENSOR_H__
#define __BME280_SENSOR_H__

#include <Arduino.h>
#include "Adafruit_BME280.h"

// ****************************************************************
//                      BME280 Sensor class
//  used to move out the code for the sensors from the main sketch
//  NOTE: the methods shall return the normalized "int" values for
//        the data transmission
// ****************************************************************
class BME280_Sensor
{
    public:
        BME280_Sensor(Serial_ serial, uint8_t address);
        bool setup();
        int16_t readTemperature(void);
        int16_t readPressure(void);
        int16_t readHumidity(void);

    private:
        Serial_ debugSerial;
        uint8_t address;
        Adafruit_BME280 bme280;
};

#endif