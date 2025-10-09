#include "M2_IoT_KDEJ/core/MQTTClient.h"
#include <ArduinoJson.h>

static MQTTClientWrapper* instance = nullptr;

MQTTClientWrapper::MQTTClientWrapper() : mqttClient(wifiClient) {
    instance = this;
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCallback(callback);
}

void MQTTClientWrapper::connect() {
    while (!mqttClient.connected()) {
        Serial.print("Connecting to MQTT...");
        if (mqttClient.connect(MQTT_CLIENT_ID)) {
            Serial.println("✓ Connected");
            mqttClient.subscribe(MQTT_TOPIC_COMMAND);
            mqttClient.publish(MQTT_TOPIC_STATUS, "online");
        } else {
            Serial.printf("✗ Failed, rc=%d. Retrying in 5s...\n", mqttClient.state());
            delay(5000);
        }
    }
}

void MQTTClientWrapper::loop() {
    if (!mqttClient.connected()) connect();
    mqttClient.loop();
}

void MQTTClientWrapper::callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (unsigned int i = 0; i < length; ++i) msg += static_cast<char>(payload[i]);
    instance->handleCommand(String(topic), msg);
}

void MQTTClientWrapper::handleCommand(const String& topic, const String& payload) {
    if (topic != MQTT_TOPIC_COMMAND) return;
    if (payload == "LED_ON") {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED ON");
    } else if (payload == "LED_OFF") {
        digitalWrite(LED_PIN, LOW);
        Serial.println("LED OFF");
    }
}

void MQTTClientWrapper::publishSensorData(float temp, float hum, float heatIndex, float tempF) {
    JsonDocument doc;
    doc["temperature_c"]     = temp;
    doc["temperature_f"] = tempF;
    doc["humidity"] = hum;
    doc["heat_index_c"] = heatIndex;
    doc["timestamp"] = millis();

    char buffer[256];
    serializeJson(doc, buffer);

    if (mqttClient.publish(MQTT_TOPIC_SENSOR, buffer)) {
        Serial.println("Data published");
    } else {
        Serial.println("Publish failed");
    }
}
