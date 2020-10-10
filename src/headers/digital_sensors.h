#ifndef _digitalSensors_h_
#define _digitalSensors_h_

#include <Wire.h>
#include "analog_sensors.h"

extern AnalogSensors analogSensors;

class DigitalSensors {
private:
    void waterSwitch();
public:
    void setup();
    void loop();
    void leds(int mode);
};

#endif