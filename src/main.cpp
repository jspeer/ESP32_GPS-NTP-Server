/************************************************************************************
 Title:       ESP32 GPS Based NTP Server
 Author:      Jeremy Speer, jeremy.a.speer@thecodeninja.dev
 Date:        May 10, 2022

 Legal:       Released under CC0 1.0 Universal
              https://creativecommons.org/publicdomain/zero/1.0/legalcode
 ************************************************************************************/

#include "main.h"

void setup() {
    Serial.begin(115200);
    vTaskDelay(500 / portTICK_PERIOD_MS);                                   // Delay so we are sure serial is up

    Serial.printf("%s v%s starting up.\n", TITLE, VERSION);

    // Draw the base layer of the display
    Serial.println("Initializing display.");
    display->init(1);                                                       // Initialize the display
    display->DrawBase(TITLE, VERSION);                                      // Draw the base display layer
    display->DrawWifiIcon(false);                                           // Draw WiFi icon on display

    // Initialize the GPS unit
    Serial.print("Initializing GPS.");
    gps->init();
    vTaskDelay(500 / portTICK_PERIOD_MS);                                   // Wait for GPS module to initialize
    while (gps->gnss_is_initialized == false) {
        Serial.print(".");
        display->DrawNoSyncIcon();
        gps->init();                                                        // Retry GPS init()
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    display->DrawSyncInProgressIcon();                                      // Draw sync icon on display
    Serial.println();

    // Set up Time Zone as defined in main.h
    Serial.println("Setting up time zone.");
    setenv("TZ", appSettings.timezone, 1); tzset();                         // This is the easiest way to ensure we can use a full TZ string

    // Start up the WiFi
    Serial.println("Initializing and configuring WiFi.");
    WiFi.mode(WIFI_STA);                                                    // Set the wifi to station mode
    if (!appSettings.networkSettings.dhcp) {
        if (WiFi.config(
            appSettings.networkSettings.staticIP,
            appSettings.networkSettings.gateway,
            appSettings.networkSettings.subnet,
            appSettings.networkSettings.dns1,
            appSettings.networkSettings.dns2
        ) == false) {
                Serial.println("WiFi configuration failed.");
                while(1) { ;; };
        }
    }
    if (appSettings.wifiSettings.ssid == NULL) {
        Serial.println("No SSID set in config.json. Shutting down.");
        esp_deep_sleep_start();
    }

    Serial.printf("Connecting to %s", appSettings.wifiSettings.ssid);
    WiFi.begin(appSettings.wifiSettings.ssid, appSettings.wifiSettings.password); // Tell wifi to start connecting
    while (WiFi.status() != WL_CONNECTED) {                                 // Wait for connection
        Serial.print(".");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    Serial.println("connected.");
    display->DrawWifiIcon(true);                                            // Draw wifi icon on display
    String ipaddr = WiFi.localIP().toString();
    display->WriteIPAddr(&ipaddr);                                          // Draw IP address on display

    if (start_mdns_services(appSettings.mDNSSettings.hostname, appSettings.mDNSSettings.host_description, "_ntp", "_udp", NTP_PORT)) { // Start mDNS service
        Serial.printf("mDNS Started with hostname: %s.\n", appSettings.mDNSSettings.hostname);
    } else {
        Serial.println("mDNS failed to start.");
    }

/************************************************************************************
 * Start one shot timer which spawns the remainder of the timers and functions      *
 ************************************************************************************/
    Serial.println("Starting timers.");
    // Set up the arguments for StartTimers()
    StartTimersArgs* startTimersArgs = new StartTimersArgs {
        .display = display,
        .gps = gps
    };
    // Create the timer arguments
    const esp_timer_create_args_t startTimersTimerArgs = {
        .callback   = &StartTimers,
        .arg        = static_cast<void*>(startTimersArgs),
        .name       = "Start Timers"
    };
    // Create the timer
    esp_timer_handle_t startTimersTimerHandle;
    ESP_ERROR_CHECK(esp_timer_create(&startTimersTimerArgs, &startTimersTimerHandle));
    // Start the one shot event
    ESP_ERROR_CHECK(esp_timer_start_once(startTimersTimerHandle, 0));
/********************************* End of one-shot timer ****************************/

    Serial.println("Obtaining first time stamp from GPS.");
    gps->saveEpochToRtc();

    UDP.begin(NTP_PORT);                    // Start listening on the NTP/UDP port

    Serial.printf("\nReady to receive requests on %s:%i/UDP.\n", ipaddr, NTP_PORT);
}

// Using loop() for NTP server // TODO: change the NTP packet portion to a task/function within the NTP_Server library
void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        // Is there a new packet waiting?
        // I structured this in this way so I could use a timer task or something similar for SendNTPReply() with only minor changes to the function below
        if (UDP.parsePacket()) {
            SendNTPReplyArgs* sendNTPReplyArgs = new SendNTPReplyArgs {
                .UDP = &UDP,
                .gps = gps
            };
            SendNTPReply(sendNTPReplyArgs);
        }
    } else {
        // Duplicate code from above
        Serial.println("Stopping mDNS service.");
        stop_mdns_services();
        display->DrawWifiIcon(false);
        Serial.print("Reconnecting to WiFi");
        WiFi.reconnect();
        while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
        Serial.println("connected.");
        display->DrawWifiIcon(true);
        if (start_mdns_services(
            appSettings.mDNSSettings.hostname,
            appSettings.mDNSSettings.host_description,
            appSettings.mDNSSettings.service_type,
            appSettings.mDNSSettings.proto,
            appSettings.mDNSSettings.port
        )) {
                Serial.printf("mDNS started with hostname: %s.\n", appSettings.mDNSSettings.hostname);
        } else {
            Serial.println("mDNS failed to start.");
        }
    }
}
