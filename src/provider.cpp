#include "headers/provider.h"

WiFiClient client;
ThingsBoard tb(client);

void Provider::loop() {
    if ( !tb.connected() ) {
        digitalSensors.leds(1);
        reconnect();
    }
    sendData();
}

void Provider::reconnect(){
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

void Provider::sendData() {
    digitalSensors.leds(2);

    tb.sendTelemetryInt("temperatureInt", analogSensors.getTempInt());
    tb.sendTelemetryInt("temperatureExt", i2cSensors.getTempExt());
    tb.sendTelemetryInt("humidityInt", analogSensors.getHumInt());
    tb.sendTelemetryInt("humidityExt", i2cSensors.getHumExt());
    tb.sendTelemetryInt("light", i2cSensors.getLight());
    tb.sendTelemetryInt("weight", analogSensors.getWeight());
    tb.sendTelemetryInt("weight", analogSensors.getWeight());
    tb.sendTelemetryInt("pressure", analogSensors.getPressure());
}