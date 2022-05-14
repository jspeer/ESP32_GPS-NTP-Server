#include "settings.h"
#include <arpa/inet.h>
#include <ArduinoJson.h>

#define FORMAT_LITTLEFS_IF_FAILED false

App_Settings LoadAppSettings(const char* inifile) {
    App_Settings appSettings;

    DynamicJsonDocument doc(1024);
    if (LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
        if (File fp = LittleFS.open(inifile, "r", false)) {
            // Read the contents of the file
            char buf[fp.size()];
            fp.readBytes(buf, fp.size());
            fp.close();

            // Parse the JSON content into multidimensional object
            DeserializationError err = deserializeJson(doc, buf);
            if (err.code() == err.Ok) {
                JsonObject jsonConfig = doc.as<JsonObject>();

                // Load up the app settings stuct with the JSON data

                // Time zone
                if (jsonConfig["time"]["timezone"]) appSettings.timezone = jsonConfig["time"]["timezone"].as<const char*>();

                // WiFi connection settings
                appSettings.wifiSettings.ssid     = jsonConfig["wifi"]["ssid"];
                appSettings.wifiSettings.password = jsonConfig["wifi"]["password"];

                // Network settings
                String dhcpStr = jsonConfig["network"]["dhcp"].as<String>();
                dhcpStr.toLowerCase();
                appSettings.networkSettings.dhcp = (dhcpStr == "yes" || dhcpStr == "true");
                if (!appSettings.networkSettings.dhcp) {
                    appSettings.networkSettings.staticIP = IPAddress(inet_addr((char*)jsonConfig["network"]["ip"].as<const char*>()));
                    appSettings.networkSettings.subnet = IPAddress(inet_addr((char*)jsonConfig["network"]["subnet"].as<const char*>()));
                    appSettings.networkSettings.gateway = IPAddress(inet_addr((char*)jsonConfig["network"]["gateway"].as<const char*>()));
                    appSettings.networkSettings.dns1 = IPAddress(inet_addr((char*)jsonConfig["network"]["dns1"].as<const char*>()));
                    appSettings.networkSettings.dns2 = IPAddress(inet_addr((char*)jsonConfig["network"]["dns2"].as<const char*>()));
                }

                // mDNS settings
                if (jsonConfig["mdns"]["hostname"]) appSettings.mDNSSettings.hostname = jsonConfig["mdns"]["hostname"];
                if (jsonConfig["mdns"]["description"]) appSettings.mDNSSettings.host_description = jsonConfig["mdns"]["description"];

                // GPS GPIO settings
                if (jsonConfig["gps"]["i2c_sda_pin"]) appSettings.gpsSettings.sda_pin = jsonConfig["gps"]["i2c_sda_pin"];
                if (jsonConfig["gps"]["i2c_scl_pin"]) appSettings.gpsSettings.scl_pin = jsonConfig["gps"]["i2c_scl_pin"];
            } else {
                Serial.println("Failed to deserialize JSON.");
                Serial.printf("%i: %s\n", err.code(), err.c_str());
            }
        } else {
            Serial.println("Failed to open the file for read.");
        }
    } else {
        Serial.println("LittleFS couldn't start");
    }

    return appSettings;
}