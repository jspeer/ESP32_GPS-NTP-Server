#pragma once

#ifndef GNS_INCLUDE_MAIN_H_
#define GNS_INCLUDE_MAIN_H_

// System Includes
#include <Arduino.h>
#include <esp_timer.h>
#include <byteswap.h>
#include <time.h>

// App Settings
#include "version.h"
#include "settings.h"
GNS::App_Settings appSettings = GNS::LoadAppSettings("/config.json");

// #include <WiFi.h>
#include "wifi_handler.h"

// mDNS
#include "mdns_server.h"

// Init u-Blox M9N
#include "ublox_m9n_i2c.h"
GNS::UBLOX* gps = new GNS::UBLOX(appSettings.gpsSettings.sda_pin, appSettings.gpsSettings.scl_pin);

// Init TFT
#include "ttgo_lcd.h"
GNS::TTGO* display = new GNS::TTGO;

// Include the timers
#include "start_timers.h"

// Include the NTP server
#include "ntp_server.h"
GNS::NTPServer ntpServer = GNS::NTPServer(gps);

// ============ GLOBAL VARIABLES ===================================================
#define TITLE "GPS NTP"  // shown at top of display

#endif  // GNS_INCLUDE_MAIN_H_
