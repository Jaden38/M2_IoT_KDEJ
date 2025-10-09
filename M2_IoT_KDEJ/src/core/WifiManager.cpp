#include "M2_IoT_KDEJ/core/WifiManager.h"

void WiFiManager::connect() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\nConnected to %s\nIP: %s\n", WIFI_SSID, WiFi.localIP().toString().c_str());
}
