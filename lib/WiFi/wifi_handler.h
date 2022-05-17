#pragma once

#ifndef GNS_LIB_WIFI_WIFI_HANDLER_H_
#define GNS_LIB_WIFI_WIFI_HANDLER_H_

#include <WiFi.h>
#include "ttgo_lcd.h"
#include "../../include/settings.h"

namespace GNS {
    typedef struct WifiTaskArgs {
        GNS::TTGO_Lcd* display;
        GNS::App_Settings appSettings;
    } WifiTaskArgs_t;

    void startWifi(GNS::App_Settings* appSettings, GNS::TTGO_Lcd* display);
    void wifiWatchdog(void* args);
    void wifiReconnectTask(void* args);
}

#endif  // GNS_LIB_WIFI_WIFI_HANDLER_H_
