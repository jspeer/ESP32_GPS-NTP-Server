#pragma once

#ifndef GNS_LIB_WIFI_WIFI_HANDLER_H_
#define GNS_LIB_WIFI_WIFI_HANDLER_H_

#include <WiFi.h>
#include "ttgo_lcd.h"
#include "../../include/settings.h"

namespace GNS {
    void StartWiFi(GNS::App_Settings* appSettings);
    void WiFiWatchdog(void* args);
    void WiFiReconnectTask(void* args);
}

#endif  // GNS_LIB_WIFI_WIFI_HANDLER_H_
