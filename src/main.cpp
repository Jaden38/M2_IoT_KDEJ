#include <Arduino.h>
#include <DHT.h>
#include "WiFi.h"


// WiFi credentials.
const char* WIFI_SSID = "iPhone de Jory";
const char* WIFI_PASS = "Jory38360@@";


// Constants - ESP32 specific
#define DHTPIN 4        // DHT22 connected to GPIO4 (common choice for ESP32)
#define DHTTYPE DHT22   // DHT 22 (AM2302)
#define LED_PIN 2       // Built-in LED on most ESP32 boards is GPIO2

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

// Variables
float hum;  // Stores humidity value
float temp; // Stores temperature value
float tempF; // Temperature in Fahrenheit
float heatIndex; // Heat index

void setup() {
  // Setup LED
  pinMode(LED_PIN, OUTPUT);
  
  // Start Serial - ESP32 default
  Serial.begin(115200);
  
  // ESP32 doesn't need to wait for Serial like some Arduino boards
  delay(100); // Small delay for serial to initialize
  
  // Print ESP32 info
  Serial.println("\n================================");
  Serial.println("ESP32 DHT22 Sensor Test");
  Serial.println("================================");
  Serial.print("ESP32 Chip Model: ");
  Serial.println(ESP.getChipModel());
  Serial.print("CPU Frequency: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");
  Serial.println("--------------------------------");
  
  // Initialize DHT sensor
  dht.begin();
  
  Serial.println("DHT22 initializing...");
  Serial.print("Sensor on GPIO: ");
  Serial.println(DHTPIN);
  Serial.println("Waiting for sensor to stabilize...");
  
  // Give sensor time to stabilize (DHT22 needs 2 seconds)
  delay(2500);
  Serial.println("Ready!");
  Serial.println("================================\n");

  Serial.begin(9600);
    // Giving it a little time because the serial monitor doesn't
    // immediately attach. Want the firmware that's running to
    // appear on each upload.
    delay(2000);

    Serial.println();
    Serial.println("Running Firmware.");

    // Connect to Wifi.
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("Connecting...");

    while (WiFi.status() != WL_CONNECTED) {
      // Check to see if connecting failed.
      // This is due to incorrect credentials
      if (WiFi.status() == WL_CONNECT_FAILED) {
        Serial.println("Failed to connect to WIFI. Please verify credentials: ");
        Serial.println();
        Serial.print("SSID: ");
        Serial.println(WIFI_SSID);
        Serial.print("Password: ");
        Serial.println(WIFI_PASS);
        Serial.println();
      }
      delay(5000);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Hello World, I'm connected to the internets!!");
}

void loop() {
  // Blink LED to show the program is running
  digitalWrite(LED_PIN, HIGH);
  delay(50);
  digitalWrite(LED_PIN, LOW);
  
  // Read humidity and temperature
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  tempF = dht.readTemperature(true); // Read temperature in Fahrenheit
  
  // Check if readings are valid
  if (isnan(hum) || isnan(temp) || isnan(tempF)) {
    Serial.println("❌ Failed to read from DHT sensor!");
    Serial.println("   Check wiring:");
    Serial.println("   - Data pin → GPIO4");
    Serial.println("   - VCC → 3.3V (NOT 5V!)");
    Serial.println("   - GND → GND");
    Serial.println("   - 10kΩ pull-up resistor between Data and VCC");
  } else {
    // Calculate heat index
    heatIndex = dht.computeHeatIndex(temp, hum, false);
    
    // Print readings with better formatting
    Serial.println("--- Sensor Readings ---");
    
    Serial.print("🌡️  Temperature: ");
    Serial.print(temp);
    Serial.print(" °C / ");
    Serial.print(tempF);
    Serial.println(" °F");
    
    Serial.print("💧 Humidity:    ");
    Serial.print(hum);
    Serial.println(" %");
    
    Serial.print("🔥 Heat Index:  ");
    Serial.print(heatIndex);
    Serial.println(" °C");
    
    // Add comfort level indicator
    Serial.print("📊 Comfort:     ");
    if (temp < 18) {
      Serial.println("Too Cold");
    } else if (temp >= 18 && temp <= 24 && hum >= 40 && hum <= 60) {
      Serial.println("Comfortable");
    } else if (temp > 24 && temp <= 27) {
      Serial.println("Warm");
    } else if (temp > 27) {
      Serial.println("Hot");
    } else if (hum < 40) {
      Serial.println("Too Dry");
    } else if (hum > 60) {
      Serial.println("Too Humid");
    }
    
    Serial.println("-----------------------\n");
  }
  
  // Wait before next reading (DHT22 should not be read more than once every 2 seconds)
  delay(3000);
}