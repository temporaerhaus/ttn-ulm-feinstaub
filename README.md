# TTN Ulm particulate matter sensor

A Arduino sketch to measure particulates in the air via the SDS011 sensor and sending them to TheThingsNetwork.

## Intro




## Usage
1. Connect the SDS011 to the PINs configured in the sketch (PIN 8 and 9 are preconfigured)
2. Connect the 5V and GND pins
3. Connect the DHT sensor to PIN 10, 3.3v and GND or connect the BME280 sensor to PIN SCL, SDA, 3.3V and GND.
4. Copy the file "configuration.h.default" to "configuration.h"
5. Fill in your TTN credentials in the "configuration.h"
6. If you are using a BME280 sensor, uncomment the "#define BME280" line in the "configuration.h"
7. Make sure you have the TheThingsNetwork, DHT library and BME280 library installed,
   you can get them from the Arduino Library Manager (see below)
8. Write the sketch to the The Things Uno

## Quick explanation what this does
1. sleeps for a given amount of time.
2. spins up the fan for some time to clean the fan and the housing (1 minute recommened).
3. takes 10 samples of sensor data (pm2.5 and pm10)
4. calculates the median of each to avoid outlier
5. takes the current humidity and temperature values
6. encodes the values into bytes
7. send them to the given TTN app

## Arduino Libraries

Install the following libraries through the Arduino Library Manager:

* TheThingsNetwork
* Adafruit Unified Sensor (scroll wayyy down)
* DHT sensor library
* Adafruit BME280 library


## License
GNU Affero General Public License v3 (AGPL-3.0)
See LICENSE file

## Other code used
SDS011 library by R. Zschiegner (rz@madavi.de) https://github.com/ricki-z/SDS011
We keep a local copy if this library. Makes building easier for new users.

TheThingsNetwork library by Johan Stokking (johan@stokking.nl)
https://github.com/TheThingsNetwork/arduino-node-lib
