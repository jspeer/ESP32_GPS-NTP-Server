#pragma once

#ifndef GNS_INCLUDE_MAIN_H_
#define GNS_INCLUDE_MAIN_H_

#define TITLE "GPS NTP"  // shown at top of display

// System Includes
#include <Arduino.h>
#include <esp_timer.h>
#include <byteswap.h>
#include <time.h>

// Debug logging
#ifdef ARDUINO_ARCH_ESP32
#ifndef __ARDUHAL_LOG_H__
#include <esp32-hal-log.h>
#endif
#endif

// App Settings
#include "version.h"
#include "settings.h"
GNS::App_Settings appSettings = GNS::LoadAppSettings("/config.json");

// WiFi Handler
#include "wifi_handler.h"

// mDNS
#include "mdns_server.h"

// Init u-Blox M9N
#include "ublox_m9n_i2c.h"
GNS::GPS* gps = new GNS::UBLOX_M9N_I2C(appSettings.gpsSettings.sda_pin, appSettings.gpsSettings.scl_pin);

// Init TFT
#include "ttgo_lcd.h"
GNS::TTGO* display = new GNS::TTGO;

// Init the NTP server
#include "ntp_server.h"
GNS::NTPServer ntpServer = GNS::NTPServer(gps);

// Include the timers
#include "start_timers.h"

#endif  // GNS_INCLUDE_MAIN_H_
