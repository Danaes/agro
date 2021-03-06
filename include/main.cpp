/* #include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "WiFiClient.h"
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <ThingsBoard.h>

#include <i2cdetect.h>

const char* ssid = "SBC";
const char* password = "sbc$18-macetero";

// ThingsBoard
#define TOKEN "NOyl8AkJjcNtI8X3sgQp"
const char* thingsboardServer = "192.168.43.171";
WiFiClient client;
ThingsBoard tb(client);

int ADXLIGHT = 0x10; // I2C address of light
int ADXTYH = 0x44; //I2C address of tempeture & humidity
int ledRPin = 4;
int ledGPin = 17;
int ledBPin = 16;
int tempExt;
int humExt;
int brightness;
byte light[2];
byte TYHWCommand[2] = {0x24,0x16};
byte LIGHTCommand[1] = {0x04};
byte TYH[6];
#define Power_Register 0X00 

int weightPin = 36;  //ADC0
int resistorWater1 = 37; //ADC1
int resistorWater2  = 38 ; //ADC2
int tempIntPin  = 39 ; //ADC3
int waterPin  = 32 ; //ADC4
int weight;
int tempInt;
int humInt;
int relayPin = 13;

void initOta();
void initI2C();
void initGPIOs();
void reconnect();
void scheduler();
void sendData();
void getTYH();
void getLIGHT();
void leds(int turnOn);
float truncate(float n);
void enviroment();
void ground();
void getWeight();
void getHumInt();
void getTempInt();
void checkWatering();

void setup() {
  Serial.begin(115200);
  analogReadResolution(10);
  // initOta();
  initI2C();
  initGPIOs();
}

void loop() {
  // ArduinoOTA.handle();
  // if ( !tb.connected() ) {
  //   reconnect();
  // }
  scheduler();
  delay(2000);

  // i2cdetect();
}

void initI2C(){
  Wire.begin();
  Wire.beginTransmission(ADXLIGHT); //Begin transmission to the sensor
  // Wire.write(Power_Register);     //Arduino stuff??? go to register
  byte a[] = {0,0,0};
  Wire.write(a,3);
  Wire.endTransmission(); // Power up
}

void initOta(){
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname("espressif");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!tb.connected()) {
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( tb.connect(thingsboardServer, TOKEN) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED]" );
      Serial.println( " : retrying in 5 seconds" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}

void scheduler() {
  enviroment();
  delay(500);
  // ground();
  // delay(500);
  // sendData();
  // delay(2000);
}

void sendData() {
  tb.sendTelemetryInt("temperatureInt", tempInt);
  tb.sendTelemetryInt("temperatureExt", tempExt);
  tb.sendTelemetryInt("humidityInt", humInt);
  tb.sendTelemetryInt("humidityExt", humExt);
  tb.sendTelemetryInt("light", brightness);
  tb.sendTelemetryInt("weight", weight);
}

void getTYH() {  
  Wire.beginTransmission(ADXTYH);
  Wire.write(TYHWCommand,2);
  Wire.endTransmission();

  delay(100); //100ms

  Wire.requestFrom(ADXTYH,6); //number of received byte

  // printf("num bytes %d\n", n);

  byte a[] = {0,0,0,0,0,0};
  Wire.readBytes(a,6);
  //printf(">>>>> %d", n);
  for(int i = 0;i < 6 ; i++)
      TYH[i] = a[i];
}

void getLIGHT() {

  Wire.beginTransmission(ADXLIGHT);  
  Wire.write(0x04);  //register pointer???
  Wire.endTransmission();
  // delay(1);//100ms

  Wire.requestFrom(ADXLIGHT, 2);

  byte a[] = {0,0};
  Wire.readBytes(a, 2);
  light[0] = a[0];
  light[1] = a[1];

  printf("light[0]: %d - light[1]: %d \n", a[0], a[1]);
}

void initGPIOs() {
  // leds
  pinMode(ledRPin, OUTPUT);
  pinMode(ledGPin, OUTPUT);
  pinMode(ledBPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  leds(2);
  digitalWrite (relayPin, LOW);
}

void leds(int mode) {

  switch (mode) {
  case 0:
    digitalWrite (ledRPin, LOW);
    digitalWrite (ledGPin, LOW);
    digitalWrite (ledBPin, LOW);
    break;
  case 1: 
    digitalWrite (ledRPin, HIGH);
    digitalWrite (ledGPin, LOW);
    digitalWrite (ledBPin, LOW);
    break;
  case 2: 
    digitalWrite (ledRPin, LOW);
    digitalWrite (ledGPin, HIGH);
    digitalWrite (ledBPin, LOW);
    break;
  case 3: 
    digitalWrite (ledRPin, LOW);
    digitalWrite (ledGPin, LOW);
    digitalWrite (ledBPin, HIGH);
    break;
  }
}

float truncate(float n) {
  return int(n * 100) / 100;
}

void enviroment(){
  // getTYH();
  // tempExt = (int)(TYH[0] * 256.0) + (int)(TYH[1]);
  // tempExt = truncate(-45 + (175 * tempExt) / (float)(pow(2, 16) - 1));
  // humExt = (int)(TYH[3] * 256.0) + (int)(TYH[4]);
  // humExt = truncate((100 * humExt) / (pow(2, 16) - 1));
  // printf("Temperatura : %d \n",tempExt);
  // printf("Humedad : %d \n",humExt);
  // delay(500);

  getLIGHT();
  brightness = (int)(light[0] * 256) + (int)(light[1]);
  printf("Luz: %d \n",brightness);
  if(brightness < 200) {
    leds(1);
  }else {
    leds(0);
  }
}

void ground() {
  getWeight();
  delay(500);
  getHumInt();
  checkWatering();
  delay(500);
  getTempInt();
  delay(500);
}

void getWeight(){
  int adc_value = analogRead(weightPin);
  printf("Dato: %d\n", adc_value);
  weight = (adc_value - 60 ) / 9.9;
  printf("Carga: %d\n", weight);
}

void getHumInt(){
  int res1 = analogRead(resistorWater1);
  int res2 = analogRead(resistorWater2);
  printf("Dato 1: %d\n", res1);
  printf("Dato 2: %d\n", res2);


  res1 = 100 - ((res1-170.0)/(1023.0-170.0))*100.0;
  res2 = 100 - ((res2-170.0)/(1023.0-170.0))*100.0;

  printf("H 1: %d\n", res1);
  printf("H 2: %d\n", res2);

  humInt = (res1 + res2) / 2;
}

void getTempInt() {
    int adc_value = analogRead(tempIntPin);
    printf("Dato: %d \n", adc_value);
    tempInt = ((10000.0 * ((1023.0 / (adc_value + 1) ) - 1)) - 1000) / 3.875;
    printf("Temperatura interior: %d \n", tempInt);
}

void checkWatering() {
  int water = analogRead(waterPin);
  printf("DATO AGUA: %d\n", water);
  if(humInt > 70 || water < 200){
    printf("APAGO\n");
    digitalWrite (relayPin, LOW);
  } 
  else if(humInt < 20 || water > 700){
    printf("ENCIENDO\n");
    digitalWrite (relayPin, HIGH);
  }
}*/