#include "M2_IoT_KDEJ/core/SensorManager.h"

void SensorManager::begin() {
    dht.begin();
}

bool SensorManager::read(float& temp, float& hum, float& tempF, float& heatIndex) {
    hum = dht.readHumidity();
    temp = dht.readTemperature();
    tempF = dht.readTemperature(true);

    if (isnan(hum) || isnan(temp)) {
        Serial.println("Failed to read from DHT");
        return false;
    }

    heatIndex = dht.computeHeatIndex(temp, hum, false);
    Serial.printf("%.1f°C | %.1f%% | %.1f°C\n", temp, hum, heatIndex);
    return true;
}
