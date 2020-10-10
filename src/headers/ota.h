#ifndef _ota_h_
#define _ota_h_

#include <WiFi.h>
#include "WiFiClient.h"
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include "headers/config.h"
#include "headers/sensors.h"

class Ota {
public:
    void setup();
    void loop();
};

#endif