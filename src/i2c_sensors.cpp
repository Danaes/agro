#include "headers/i2c_sensors.h"

int ADXLIGHT = 0x10; // I2C address of light
int ADXTYH = 0x44;   //I2C address of tempeture & humidity

byte TYHWCommand[2] = {0x24,0x16};
#define Power_Register 0X00

float tempExt;
float humExt;
float brightness;

void I2cSensors::setup(){
    Wire.begin();
    Wire.beginTransmission(ADXLIGHT); //Begin transmission to the sensor

    // Wire.write(Power_Register);     //Arduino stuff??? go to register
    byte a[] = {0,0,0};
    Wire.write(a,3);
    Wire.endTransmission(); // Power up
}

void I2cSensors::loop(){
    readTYH();
    readLight();
}

void I2cSensors::readTYH(){
    Wire.beginTransmission(ADXTYH);
    Wire.write(TYHWCommand,2);
    Wire.endTransmission();

    delay(100); //100ms

    Wire.requestFrom(ADXTYH,6); //number of received byte

    byte TYH[] = {0,0,0,0,0,0};
    Wire.readBytes(TYH,6);

    tempExt = (int)(TYH[0] * 256.0) + (int)(TYH[1]);
    tempExt = truncate(-45 + (175 * tempExt) / (float)(pow(2, 16) - 1));

    humExt = (int)(TYH[3] * 256.0) + (int)(TYH[4]);
    humExt = truncate((100 * humExt) / (pow(2, 16) - 1));

    // printf("Temperatura : %d \n",tempExt);
    // printf("Humedad : %d \n",humExt);
    delay(500);
}

void I2cSensors::readLight(){
    Wire.beginTransmission(ADXLIGHT);  
    Wire.write(0x04);  //register pointer
    Wire.endTransmission();
    delay(1); //100ms

    Wire.requestFrom(ADXLIGHT, 2);

    byte light[] = {0,0};
    Wire.readBytes(light, 2);
    // printf("light[0]: %d - light[1]: %d \n", light[0], light[1]);

    brightness = (int)(light[0] * 256) + (int)(light[1]);
    // printf("Luz: %d \n",brightness);
}

float I2cSensors::getTempExt(){
    return tempExt;
}
float I2cSensors::getHumExt(){
    return humExt;
}
float I2cSensors::getLight(){
    return brightness;
}

float I2cSensors::truncate(float n){
    return int(n * 100) / 100;
}