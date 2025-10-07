#include <Arduino.h>
#include <DHT.h>

// Constants
#define DHTPIN 2        // DHT22 connected to digital pin 2
#define DHTTYPE DHT22   // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

// Variables
float hum;  // Stores humidity value
float temp; // Stores temperature value

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);  // Initialize serial communication at 115200 baud
  
  // Wait for Serial to be ready (important for some boards)
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  dht.begin();
  
  Serial.println("DHT22 Sensor Test");
  Serial.println("-------------------");
  Serial.println("Waiting for sensor to stabilize...");
  
  // Give sensor time to stabilize
  delay(2000);
}

void loop() {
  // Blink LED to show the program is running
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  
  // Read humidity and temperature
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  
  // Check if readings are valid
  if (isnan(hum) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    Serial.println("Check wiring: Data pin to D2, VCC to 5V, GND to GND");
  } else {
    // Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" °C");
  }
  
  // Wait before next reading
  delay(2000);
}