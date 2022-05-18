#pragma once

#ifndef GNS_LIB_WIFI_WIFI_HANDLER_H_
#define GNS_LIB_WIFI_WIFI_HANDLER_H_

#include <WiFi.h>
#include "ttgo_lcd.h"
#include "../../include/settings.h"

namespace GNS {
    typedef struct WifiTaskArgs {
        GNS::TTGO* display;
        GNS::App_Settings appSettings;
    } WifiTaskArgs_t;

    void StartWifi(GNS::App_Settings* appSettings, GNS::TTGO* display);
    void WifiWatchdog(void* args);
    void WifiReconnectTask(void* args);
}

#endif  // GNS_LIB_WIFI_WIFI_HANDLER_H_
