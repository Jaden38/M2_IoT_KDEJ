#include <Arduino.h>
#include "M2_IoT_KDEJ/core/WiFiManager.h"
#include "M2_IoT_KDEJ/core/MQTTClient.h"
#include "M2_IoT_KDEJ/core/SensorManager.h"

SensorManager sensors;
MQTTClientWrapper mqtt;
unsigned long lastPublish = 0;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== ESP32 DHT22 MQTT Monitor ===");

    WiFiManager::connect();
    sensors.begin();
}

void loop() {
    mqtt.loop();

    if (millis() - lastPublish >= PUBLISH_INTERVAL) {
        float temp, hum, heatIndex, tempF;
        if (sensors.read(temp, hum, tempF, heatIndex))
            mqtt.publishSensorData(temp, hum, heatIndex, tempF);

        lastPublish = millis();
    }
}
