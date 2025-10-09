#pragma once

// Pin Definitions
#define DHTPIN 4
#define DHTTYPE DHT22
#define LED_PIN 2
#define PIR_PIN 15 

// WiFi Credentials
#define WIFI_SSID "AndroidAP Damien"
#define WIFI_PASS "motdepasse"

// MQTT Broker Settings
#define MQTT_BROKER "10.17.39.200"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "ESP32_DHT22_Client"

// MQTT Topics
#define MQTT_TOPIC_SENSOR "home/climate/sensors"
#define MQTT_TOPIC_TEMP "home/climate/temperature"
#define MQTT_TOPIC_HUM "home/climate/humidity"
#define MQTT_TOPIC_HEAT "home/climate/heatindex"
#define MQTT_TOPIC_STATUS "home/climate/status"
#define MQTT_TOPIC_COMMAND "home/climate/command"
#define MQTT_TOPIC_MOTION "home/security/motion"

// Timing
#define PUBLISH_INTERVAL 3000 // Publish every 3 seconds