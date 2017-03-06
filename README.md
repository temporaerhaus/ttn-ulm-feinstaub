# TTN Ulm particulate matter sensor

A small Arduino sketch to measure particulates in the air via the SDS011 sensor and sending them to TheThingsNetwork.

## Usage
1. Connect the SDS011 to the PINs configured in the sketch (PIN 8 and 9 are preconfigured)
2. Connect the 5V and GND pins
3. Fill in your TTN credentials
4. Make sure you have the SDS0011 and TheThingsNetwork library present in the Arduino library folder,
   you can get the TTN library from the Arduino Library Manager or from github
5. Write the sketch to the arduino

## Quick explanation what this does
1. sleeps for a given amount of time.
2. spins up the fan for a minute to clean the fan and the housing.
3. takes 10 samples of sensor data (pm2.5 and pm10)
4. calculates the median of each to avoid outlier
5. encodes the values into bytes
6. send them to the given TTN app

## Arduino Libraries

Install the following libraries through the Arduino Library Manager:

* TheThingsNetwork
* Adafruit Unified Sensor (scroll wayyy down)
* DHT sensor library


## License
GNU Affero General Public License v3 (AGPL-3.0)
See LICENSE file

## Other code used
SDS011 library by R. Zschiegner (rz@madavi.de) https://github.com/ricki-z/SDS011
We keep a local copy if this library in the libs/ subfolder. Makes building easier for new users.

TheThingsNetwork library by Johan Stokking (johan@stokking.nl)
https://github.com/TheThingsNetwork/arduino-node-lib
