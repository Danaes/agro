#ifndef _i2c_sensors_h_
#define _i2c_sensors_h_

#include <Arduino.h>
#include <Wire.h>

class I2cSensors {
private:
    void readTYH();
    void readLight();
    float truncate(float n);
public:
    void setup();
    void loop();
    float getTempExt();
    float getHumExt();
    float getLight();
};

#endif