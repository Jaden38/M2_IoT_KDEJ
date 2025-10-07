#ifndef CONFIG_H
#define CONFIG_H

// Pin Definitions
#define DHTPIN 4
#define DHTTYPE DHT22
#define LED_PIN 2

// WiFi Credentials
const char* WIFI_SSID = "iPhone de Jory";
const char* WIFI_PASS = "Jory38360@@";

// MQTT Broker Settings
// ⚠️ CHANGE THIS TO YOUR COMPUTER'S IP FROM STEP 3
const char* MQTT_BROKER = "172.20.10.11";
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "ESP32_DHT22_Client";

// MQTT Topics
const char* MQTT_TOPIC_SENSOR = "home/climate/sensors";
const char* MQTT_TOPIC_TEMP = "home/climate/temperature";
const char* MQTT_TOPIC_HUM = "home/climate/humidity";
const char* MQTT_TOPIC_HEAT = "home/climate/heatindex";
const char* MQTT_TOPIC_STATUS = "home/climate/status";
const char* MQTT_TOPIC_COMMAND = "home/climate/command";

// Timing
const unsigned long PUBLISH_INTERVAL = 3000; // Publish every 3 seconds

#endif