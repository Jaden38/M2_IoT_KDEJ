#pragma once
#include <PubSubClient.h>
#include "M2_IoT_KDEJ/config/config.h"
#include "M2_IoT_KDEJ/core/WiFiManager.h"

class MQTTClientWrapper {
public:
    MQTTClientWrapper();
    void connect();  // Made public so it can be called from main.cpp
    void loop();
    void publishSensorData(float temp, float hum, float heatIndex, float tempF);
    void publishMotionEvent(bool motionDetected);  // Added for PIR sensor
    void handleCommand(const String& topic, const String& payload);

private:
    WiFiClient wifiClient;
    PubSubClient mqttClient;
    
    static void callback(char* topic, byte* payload, unsigned int length);
};