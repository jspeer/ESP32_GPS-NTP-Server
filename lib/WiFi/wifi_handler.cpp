#include "wifi_handler.h"

void WifiWatchdog(void* args) {
    WifiTaskArgs* wifiTaskArgs = static_cast<WifiTaskArgs*>(args);

    for (;;) {
        if (WiFi.status() != WL_CONNECTED) {
            xTaskCreatePinnedToCore(WifiReconnectTask, "Reconnect to WiFi", 5000, wifiTaskArgs, 1, NULL, 1);
        }

        vTaskDelay(5 / portTICK_RATE_MS * 1000);    // delay 5 seconds before checking again
    }
}

void WifiReconnectTask(void* args) {
    WifiTaskArgs* wifiTaskArgs = static_cast<WifiTaskArgs*>(args);

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
