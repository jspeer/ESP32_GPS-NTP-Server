#pragma once

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "IPAddress.h"
#include "FS.h"
#include "LittleFS.h"

typedef struct App_Settings {
    // Sub-structs to hold data
    typedef struct WiFi_Settings {
        const char* ssid = NULL;
        const char* password = NULL;
    } WiFi_Settings_t;

    typedef struct Network_Settings {
        bool dhcp = true;
        IPAddress staticIP;
        IPAddress subnet;
        IPAddress gateway;
        IPAddress dns1;
        IPAddress dns2;
    } Network_Settings_t;

    typedef struct mDNS_Settings {
        const char* hostname = (char*)"esp32-ntp";
        const char* host_description = (char*)"ESP32 NTP Server";
        const char* service_type = (char*)"_ntp";
        const char* proto = (char*)"_udp";
        uint16_t port = 123;
    } mDNS_Settings_t;

    // typedef struct GPS_Settings {
    //     uint16_t sda_pin = 21;
    //     uint16_t scl_pin = 22;
    // } GPS_Settings_t;
    // End of sub-structs

    // Main structure for App Settings
    const char* timezone = (char*)"UTC";
    WiFi_Settings wifiSettings;
    Network_Settings networkSettings;
    mDNS_Settings mDNSSettings;
    // GPS_Settings gpsSettings;
} App_Settings_t;

App_Settings LoadAppSettings(const char* inifile);

#endif  // _SETTINGS_H_
