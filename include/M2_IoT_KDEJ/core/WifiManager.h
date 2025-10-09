#pragma once
#include <WiFi.h>
#include "M2_IoT_KDEJ/config/config.h"

class WiFiManager {
public:
    static void connect();
    static bool isConnected() { return WiFi.status() == WL_CONNECTED; }
};
