#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include "config.h"

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

float temp, hum, heatIndex, tempF;
unsigned long lastPublish = 0;

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received [");
  Serial.print(topic);
  Serial.print("]: ");
  
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  
  // Handle commands
  if (String(topic) == MQTT_TOPIC_COMMAND) {
    if (message == "LED_ON") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("✓ LED turned ON");
    } else if (message == "LED_OFF") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("✓ LED turned OFF");
    }
  }
}

void mqttReconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (mqttClient.connect(MQTT_CLIENT_ID)) {
      Serial.println("connected!");
      
      // Subscribe to command topic
      mqttClient.subscribe(MQTT_TOPIC_COMMAND);
      Serial.print("Subscribed to: ");
      Serial.println(MQTT_TOPIC_COMMAND);
      
      // Publish online status
      mqttClient.publish(MQTT_TOPIC_STATUS, "online");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void publishSensorData() {
  // Create JSON document (updated syntax for ArduinoJson v7)
  JsonDocument doc;
  doc["temperature_c"] = temp;
  doc["temperature_f"] = tempF;
  doc["humidity"] = hum;
  doc["heat_index_c"] = heatIndex;
  doc["timestamp"] = millis();
  
  // Serialize to string
  char jsonBuffer[200];
  serializeJson(doc, jsonBuffer);
  
  // Publish to MQTT
  if (mqttClient.publish(MQTT_TOPIC_SENSOR, jsonBuffer)) {
    Serial.println("✓ Data published:");
    Serial.println(jsonBuffer);
  } else {
    Serial.println("✗ Failed to publish data");
  }
  
  // Publish individual topics (optional)
  mqttClient.publish(MQTT_TOPIC_TEMP, String(temp).c_str());
  mqttClient.publish(MQTT_TOPIC_HUM, String(hum).c_str());
  mqttClient.publish(MQTT_TOPIC_HEAT, String(heatIndex).c_str());
}

void readSensors() {
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  tempF = dht.readTemperature(true);
  
  if (isnan(hum) || isnan(temp) || isnan(tempF)) {
    Serial.println("✗ Failed to read from DHT sensor!");
    return;
  }
  
  heatIndex = dht.computeHeatIndex(temp, hum, false);
  
  Serial.printf("🌡️  %.1f°C | 💧 %.1f%% | 🔥 %.1f°C\n", 
                temp, hum, heatIndex);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════╗");
  Serial.println("║  ESP32 DHT22 MQTT Monitor     ║");
  Serial.println("╚════════════════════════════════╝\n");
  
  dht.begin();
  setupWiFi();
  
  // Configure MQTT
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  
  Serial.print("MQTT Broker: ");
  Serial.print(MQTT_BROKER);
  Serial.print(":");
  Serial.println(MQTT_PORT);
  Serial.println();
}

void loop() {
  // Maintain MQTT connection
  if (!mqttClient.connected()) {
    mqttReconnect();
  }
  mqttClient.loop();
  
  // Read sensors and publish
  if (millis() - lastPublish >= PUBLISH_INTERVAL) {
    readSensors();
    publishSensorData();
    lastPublish = millis();
  }
}