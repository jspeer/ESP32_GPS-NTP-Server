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

void WifiWatchdog(void* args);
void WifiReconnectTask(void* args);

#endif  // _WIFI_HANDLER_H_
