#include "M2_IoT_KDEJ/core/PIRSensor.h"

PIRSensor::PIRSensor(uint8_t pin) : sensorPin(pin), lastMotionState(false), lastMotionTime(0), motionActive(false), motionStartTime(0) {}

void PIRSensor::begin() {
    pinMode(sensorPin, INPUT);
    Serial.println("PIR Sensor initialized on pin " + String(sensorPin));
    Serial.println("Waiting for PIR sensor to stabilize (10 seconds)...");
    delay(1000); // PIR sensor needs time to stabilize
    Serial.println("PIR Sensor ready!");
}

bool PIRSensor::detectMotion() {
    return digitalRead(sensorPin) == HIGH;
}

void PIRSensor::loop() {
    bool currentMotion = detectMotion();
    
    // Motion just started
    if (currentMotion && !lastMotionState) {
        motionStartTime = millis();
        motionActive = true;
        Serial.println("========================================");
        Serial.println("🚨 MOTION DETECTED!");
        Serial.printf("Time: %lu ms\n", millis());
        Serial.println("Status: Motion START");
        Serial.println("========================================");
        lastMotionTime = millis();
    }
    // Motion just ended
    else if (!currentMotion && lastMotionState) {
        unsigned long motionDuration = millis() - motionStartTime;
        motionActive = false;
        Serial.println("========================================");
        Serial.println("✅ Motion ENDED");
        Serial.printf("Duration: %lu ms (%.2f seconds)\n", motionDuration, motionDuration / 1000.0);
        Serial.printf("Time: %lu ms\n", millis());
        Serial.println("========================================");
    }
    // Continuous motion (log every 2 seconds while motion is active)
    else if (currentMotion && motionActive && (millis() - lastMotionTime > 2000)) {
        unsigned long duration = millis() - motionStartTime;
        Serial.printf("📍 Motion ongoing... Duration: %.1f seconds\n", duration / 1000.0);
        lastMotionTime = millis();
    }
    
    lastMotionState = currentMotion;
}