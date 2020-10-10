#ifndef _analogSensors_h_
#define _analogSensors_h_

#include <Wire.h>

class AnalogSensors {
private:
    void readWeight();
    void readHumInt();
    void readTempInt();
    void readWaterLevel();
    void readPressure();
public:
    void setup();
    void loop();
    int getHumInt();
    int getWeight();
    int getTempInt();
    int getPressure();
    bool checkWaterLevel();
};

#endif