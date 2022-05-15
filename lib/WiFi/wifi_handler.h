#pragma once

#ifndef _WIFI_HANDLER_H_
#define _WIFI_HANDLER_H_

#include <WiFi.h>
#include "ttgo_lcd.h"
#include "../../include/settings.h"

typedef struct WifiTaskArgs {
    TTGO_Lcd* display;
    App_Settings appSettings;
} WifiTaskArgs_t;

void startWifi(App_Settings* appSettings, TTGO_Lcd* display);
void wifiWatchdog(void* args);
void wifiReconnectTask(void* args);

#endif  // _WIFI_HANDLER_H_
