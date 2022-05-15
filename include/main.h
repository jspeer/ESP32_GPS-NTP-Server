#pragma once

#ifndef _MAIN_H_
#define _MAIN_H_

// System Includes
#include <Arduino.h>
#include <esp_timer.h>
#include <byteswap.h>
#include <time.h>

// App Settings
#include "version.h"
#include "settings.h"
App_Settings appSettings = LoadAppSettings("/config.json");

// #include <WiFi.h>
#include "wifi_handler.h"

// mDNS
#include "mDNS_Server.h"

// Init u-Blox M9N
#include "ublox_m9n_i2c_module.h"
UBLOX_M9N* gps = new UBLOX_M9N(appSettings.gpsSettings.sda_pin, appSettings.gpsSettings.scl_pin);

// Init TFT
#include "ttgo_lcd.h"
TTGO_Lcd* display = new TTGO_Lcd;

// Include the timers
#include "startTimers.h"

// Include the NTP server
#include "ntpServer.h"
NTPServer ntpServer = NTPServer(gps);

// ============ GLOBAL VARIABLES ===================================================
#define TITLE "GPS NTP"  // shown at top of display

#endif  // _MAIN_H_
