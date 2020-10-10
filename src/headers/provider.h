#ifndef _provider_h_
#define _provider_h_

#include "WiFiClient.h"
#include <ThingsBoard.h>
#include "headers/config.h"
#include "headers/sensors.h"


class Provider {
private:
    void reconnect();
    void sendData();
public:
    void loop();
};

#endif