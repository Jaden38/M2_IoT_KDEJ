#ifndef CONFIG_H
#define CONFIG_H

// Pin Definitions
#define DHTPIN 4
#define DHTTYPE DHT22
#define LED_PIN 2

// WiFi Credentials - DECLARATIONS ONLY
extern const char* WIFI_SSID;
extern const char* WIFI_PASS;

// MQTT Broker Settings - DECLARATIONS ONLY
extern const char* MQTT_BROKER;
extern const int MQTT_PORT;
extern const char* MQTT_CLIENT_ID;

// MQTT Topics - DECLARATIONS ONLY
extern const char* MQTT_TOPIC_SENSOR;
extern const char* MQTT_TOPIC_TEMP;
extern const char* MQTT_TOPIC_HUM;
extern const char* MQTT_TOPIC_HEAT;
extern const char* MQTT_TOPIC_STATUS;
extern const char* MQTT_TOPIC_COMMAND;

// Timing - DECLARATIONS ONLY
extern const unsigned long PUBLISH_INTERVAL;

#endif