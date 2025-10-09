#pragma once
#include <PubSubClient.h>
#include "M2_IoT_KDEJ/config/config.h"
#include "M2_IoT_KDEJ/core/WiFiManager.h"

class MQTTClientWrapper {
public:
    MQTTClientWrapper();
    void loop();
    void publishSensorData(float temp, float hum, float heatIndex, float tempF);
    void handleCommand(const String& topic, const String& payload);

private:
    WiFiClient wifiClient;
    PubSubClient mqttClient;

    void connect();
    static void callback(char* topic, byte* payload, unsigned int length);
};
