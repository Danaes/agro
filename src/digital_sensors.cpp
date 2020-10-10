#include "headers/digital_sensors.h"

int ledRPin = 13;
int ledGPin = 12;
int ledBPin = 14;
int relayPin = 33;

AnalogSensors analogSensors;

void DigitalSensors::setup(){
    pinMode(ledRPin, OUTPUT);
    pinMode(ledGPin, OUTPUT);
    pinMode(ledBPin, OUTPUT);
    pinMode(relayPin, OUTPUT);

    leds(2);
    digitalWrite (relayPin, LOW);
}

void DigitalSensors::loop(){
    waterSwitch();
}

void DigitalSensors::leds(int mode){
    switch (mode) {
        case 0:
            digitalWrite (ledRPin, LOW);
            digitalWrite (ledGPin, LOW);
            digitalWrite (ledBPin, LOW);
        break;
        case 1: // There's an error
            digitalWrite (ledRPin, HIGH);
            digitalWrite (ledGPin, LOW);
            digitalWrite (ledBPin, LOW);
        break;
        case 2: // Send data to cloud
            digitalWrite (ledRPin, LOW);
            digitalWrite (ledGPin, HIGH);
            digitalWrite (ledBPin, LOW);
        break;
        case 3: // It's watering
            digitalWrite (ledRPin, LOW);
            digitalWrite (ledGPin, LOW);
            digitalWrite (ledBPin, HIGH);
        break;
    }
}

void DigitalSensors::waterSwitch() {

    if (analogSensors.checkWaterLevel() || analogSensors.getHumInt() > 70){
        digitalWrite(relayPin, LOW);
        leds(0);
        return;
    }

    if(analogSensors.getHumInt() < 20){
        leds(3);
        digitalWrite(relayPin, HIGH);
    }
}
