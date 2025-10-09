#pragma once
#include <DHT.h>
#include "M2_IoT_KDEJ/config/config.h"

class SensorManager {
public:
    void begin();
    bool read(float& temp, float& hum, float& tempF, float& heatIndex);
private:
    DHT dht{DHTPIN, DHTTYPE};
};
