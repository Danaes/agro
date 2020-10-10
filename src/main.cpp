#include "headers/provider.h" 
#include "headers/ota.h"

Provider provider;
Ota ota;
Sensors sensors;

void setup(){
    Serial.begin(115200);
    
    ota.setup();
    sensors.setup();
}

void loop(){
    ota.loop();
    sensors.loop();
    provider.loop();
}