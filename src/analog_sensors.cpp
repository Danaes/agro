#include "headers/analog_sensors.h"

int weightPin = 36;  //ADC0
int resistorWater1 = 37; //ADC1
int resistorWater2  = 38 ; //ADC2
int tempIntPin  = 39 ; //ADC3
int pressurePin = 32; //ADC4
int waterPin  = 34 ; //ADC6

int weight;
int humInt;
int tempInt;
int waterAmount;
int pressureValue;

void AnalogSensors::setup(){
    analogReadResolution(10);
}

void AnalogSensors::loop(){
  readWeight();
  readHumInt();
  readTempInt();
  readWaterLevel();
}

void AnalogSensors::readWeight(){
  weight = analogRead(weightPin);
  // // printf("Dato: %d\n", weight);
  // // printf("Carga: %d\n", (weight - 60 ) / 9.9);
  weight = (weight - 60 ) / 9.9;
}

void AnalogSensors::readHumInt(){
  int res1 = analogRead(resistorWater1);
  int res2 = analogRead(resistorWater2);
  // // printf("Dato 1: %d\n", res1);
  // // printf("Dato 2: %d\n", res2);


  res1 = 100 - ((res1-170.0)/(1023.0-170.0))*100.0;
  res2 = 100 - ((res2-170.0)/(1023.0-170.0))*100.0;

  // // printf("H 1: %d\n", res1);
  // // printf("H 2: %d\n", res2);

  humInt = (res1 + res2) / 2;
}

void AnalogSensors::readTempInt() {
    tempInt = analogRead(tempIntPin);
    // printf("Dato: %d \n", tempInt);
    tempInt = ((10000.0 * ((1023.0 / (tempInt + 1) ) - 1)) - 1000) / 3.875;
    // printf("Temperatura interior: %d \n", tempInt);
}

void AnalogSensors::readWaterLevel() {
    waterAmount = analogRead(waterPin);
  // printf("DATO AGUA: %d\n", waterAmount);
}

void AnalogSensors::readPressure(){
    pressureValue = analogRead(pressurePin);
    // TODO falta calibrar el sensor
    // printf("Presión: %d \n", pressureValue);
}

int AnalogSensors::getPressure(){
    return pressureValue;
}
int AnalogSensors::getHumInt(){
  return humInt;
}
int AnalogSensors::getTempInt(){
  return tempInt;
}
int AnalogSensors::getWeight(){
  return weight;
}
bool AnalogSensors::checkWaterLevel(){
  return waterAmount < 200;
}