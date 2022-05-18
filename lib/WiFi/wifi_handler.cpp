#ifndef GNS_LIB_WIFI_WIFI_HANDLER_H_
#include "wifi_handler.h"
#endif

void GNS::StartWifi(GNS::App_Settings* appSettings, GNS::TTGO* display) {
    WiFi.mode(WIFI_STA);                                                    // Set the wifi to station mode
    if (!appSettings->networkSettings.dhcp) {
        if (WiFi.config(
            appSettings->networkSettings.staticIP,
            appSettings->networkSettings.gateway,
            appSettings->networkSettings.subnet,
            appSettings->networkSettings.dns1,
            appSettings->networkSettings.dns2
        ) == false) {
                Serial.println("WiFi configuration failed. Shutting down.");
                esp_deep_sleep_start();
        }
    }
    if (appSettings->wifiSettings.ssid == NULL) {
        Serial.println("No SSID set in config.json. Shutting down.");
        esp_deep_sleep_start();
    }

    Serial.printf("Connecting to %s", appSettings->wifiSettings.ssid);
    WiFi.begin(appSettings->wifiSettings.ssid, appSettings->wifiSettings.password); // Tell wifi to start connecting
    while (WiFi.status() != WL_CONNECTED) {                                 // Wait for connection
        Serial.print(".");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    Serial.println("connected.");
    display->DrawWifiIcon(true);                                            // Draw wifi icon on display
    String ipaddr = WiFi.localIP().toString();
    display->WriteIPAddr(&ipaddr);                                          // Draw IP address on display

    // Start the wifi watchdog task
    GNS::WifiTaskArgs* wifiTaskArgs = new GNS::WifiTaskArgs {
        .display = display,
        .appSettings = *appSettings
    };
    xTaskCreate(GNS::WifiWatchdog, "Reconnect to WiFi", 5000, wifiTaskArgs, 1, NULL);
}

void GNS::WifiWatchdog(void* args) {
    GNS::WifiTaskArgs* wifiTaskArgs = static_cast<GNS::WifiTaskArgs*>(args);

    for (;;) {
        if (WiFi.status() != WL_CONNECTED) {
            xTaskCreatePinnedToCore(GNS::WifiReconnectTask, "Reconnect to WiFi", 5000, wifiTaskArgs, 1, NULL, 1);
        }

        vTaskDelay(5 / portTICK_RATE_MS * 1000);    // delay 5 seconds before checking again
    }
}

void GNS::WifiReconnectTask(void* args) {
    GNS::WifiTaskArgs* wifiTaskArgs = static_cast<GNS::WifiTaskArgs*>(args);

    wifiTaskArgs->display->DrawWifiIcon(false);
    Serial.print("Reconnecting to WiFi");
    WiFi.reconnect();
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    Serial.println("connected.");
    wifiTaskArgs->display->DrawWifiIcon(true);

    vTaskDelete(NULL);
}
