#pragma once
#include <Arduino.h>

class PIRSensor {
public:
    PIRSensor(uint8_t pin);
    void begin();
    bool detectMotion();
    void loop();
    
    // Public getters for motion status
    bool isMotionDetected() const { return motionActive; }
    unsigned long getMotionDuration() const;
    
private:
    uint8_t sensorPin;
    bool lastMotionState;
    unsigned long lastMotionTime;
    unsigned long motionStartTime;
    bool motionActive;
};