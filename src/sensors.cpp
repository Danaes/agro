#include "headers/sensors.h"

DigitalSensors digitalSensors;
I2cSensors i2cSensors;

void Sensors::setup(){
    i2cSensors.setup();
    analogSensors.setup();
    digitalSensors.setup();
}

void Sensors::loop(){
    i2cSensors.loop();
    analogSensors.loop();
    digitalSensors.loop();
}