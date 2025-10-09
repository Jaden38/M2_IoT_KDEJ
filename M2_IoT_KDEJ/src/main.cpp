#include <Arduino.h>
#include "M2_IoT_KDEJ/core/WiFiManager.h"
#include "M2_IoT_KDEJ/core/MQTTClient.h"
#include "M2_IoT_KDEJ/core/SensorManager.h"
#include "M2_IoT_KDEJ/core/PIRSensor.h"

SensorManager sensors;
MQTTClientWrapper mqtt;
PIRSensor pirSensor(PIR_PIN);

unsigned long lastPublish = 0;
unsigned long lastPIRCheck = 0;
const unsigned long PIR_CHECK_INTERVAL = 100; // Check PIR every 100ms

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== ESP32 DHT22 + PIR MQTT Monitor ===");
    Serial.println("PIR Sensor on GPIO 15");
    Serial.println("=====================================\n");

    // Connect to WiFi
    WiFiManager::connect();
    
    // Initialize MQTT connection after WiFi is connected
    Serial.println("Establishing MQTT connection...");
    mqtt.connect();  // Add explicit initial connection
    
    // Initialize sensors
    sensors.begin();
    pirSensor.begin();
    
    Serial.println("\n=== System Ready ===");
    Serial.println("PIR sensor is now monitoring for motion...\n");
}

void loop() {
    // Maintain MQTT connection
    mqtt.loop();

    // Check PIR sensor frequently
    static bool lastMotionState = false;
    if (millis() - lastPIRCheck >= PIR_CHECK_INTERVAL) {
        pirSensor.loop();
        
        // Check if motion state changed and publish to MQTT
        bool currentMotion = pirSensor.isMotionDetected();
        if (currentMotion != lastMotionState) {
            mqtt.publishMotionEvent(currentMotion);
            lastMotionState = currentMotion;
        }
        
        lastPIRCheck = millis();
    }

    // Publish DHT sensor data
    if (millis() - lastPublish >= PUBLISH_INTERVAL) {
        float temp, hum, heatIndex, tempF;
        if (sensors.read(temp, hum, tempF, heatIndex)) {
            // UNCOMMENTED: Actually publish the data
            mqtt.publishSensorData(temp, hum, heatIndex, tempF);
            
            // Also log to Serial for debugging
            Serial.println("--- Climate Data ---");
            Serial.printf("Temperature: %.1f°C (%.1f°F)\n", temp, tempF);
            Serial.printf("Humidity: %.1f%%\n", hum);
            Serial.printf("Heat Index: %.1f°C\n", heatIndex);
            Serial.println("-------------------\n");
        }
        lastPublish = millis();
    }
}