#ifndef GNS_LIB_WIFI_WIFI_HANDLER_H_
#include "wifi_handler.h"
#endif

// Start up the WiFi connection using settings from App_Settings (see app_settings.h)
void GNS::StartWiFi(GNS::App_Settings* appSettings) {
    WiFi.mode(WIFI_STA);
    if (!appSettings->networkSettings.dhcp) {
        if (WiFi.config(
            appSettings->networkSettings.staticIP,
            appSettings->networkSettings.gateway,
            appSettings->networkSettings.subnet,
            appSettings->networkSettings.dns1,
            appSettings->networkSettings.dns2
        ) == false) {
                ESP_LOGE("WiFi", "Wireless configuration failed. Entering deep sleep.");
                esp_deep_sleep_start();
        }
    }
    if (appSettings->wifiSettings.ssid == NULL) {
        ESP_LOGE("WiFi", "No SSID set in config.json. Entering deep sleep.");
        esp_deep_sleep_start();
    }

    ESP_LOGI("WiFi", "Connecting to %s", appSettings->wifiSettings.ssid);
    WiFi.begin(appSettings->wifiSettings.ssid, appSettings->wifiSettings.password); // Tell wifi to start connecting
    while (WiFi.status() != WL_CONNECTED) {                                 // Wait for connection
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    // Start the wifi watchdog task
    xTaskCreate(GNS::WiFiWatchdog, "Reconnect to WiFi", 5000, nullptr, 1, NULL);
}

// WiFi watchdog task, used to automatically reconnect to wifi if it gets disconnected
void GNS::WiFiWatchdog(void* args) {
    for (;;) {
        if (WiFi.status() != WL_CONNECTED) {
            xTaskCreatePinnedToCore(GNS::WiFiReconnectTask, "Reconnect to WiFi", 5000, nullptr, 1, NULL, 0);  // Pin to core 0, usually used for system/wifi/bluetooth
        }

        vTaskDelay(5 / portTICK_RATE_MS * 1000);  // delay 5 seconds before checking again
    }
}

// WiFi reconnection task
void GNS::WiFiReconnectTask(void* args) {
    ESP_LOGI("WiFi", "Reconnecting to WiFi");
    WiFi.reconnect();
    while (WiFi.status() != WL_CONNECTED)
        vTaskDelay(500 / portTICK_PERIOD_MS);

    vTaskDelete(NULL);
}
