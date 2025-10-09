#pragma once

// Pin Definitions
#define DHTPIN 4
#define DHTTYPE DHT22
#define LED_PIN 2

// WiFi Credentials
#define WIFI_SSID "iPhone de Jory"
#define WIFI_PASS "Jory38360@@"

// MQTT Broker Settings
#define MQTT_BROKER "172.20.10.11"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "ESP32_DHT22_Client"

// MQTT Topics
#define MQTT_TOPIC_SENSOR "home/climate/sensors"
#define MQTT_TOPIC_TEMP "home/climate/temperature"
#define MQTT_TOPIC_HUM "home/climate/humidity"
#define MQTT_TOPIC_HEAT "home/climate/heatindex"
#define MQTT_TOPIC_STATUS "home/climate/status"
#define MQTT_TOPIC_COMMAND "home/climate/command"

// Timing
#define PUBLISH_INTERVAL 3000 // Publish every 3 seconds