/************************************************************************************
 Title:       ESP32 GPS Based NTP Server
 Author:      Jeremy Speer, jeremy.a.speer@thecodeninja.dev
 Date:        May 10, 2022

 Legal:       Released under CC0 1.0 Universal
              https://creativecommons.org/publicdomain/zero/1.0/legalcode
 ************************************************************************************/

#include "main.h"

void setup() {
    ESP_LOGI("System", "%s v%s starting up.", TITLE, VERSION);

    // Draw the base layer of the display
    ESP_LOGI("System", "Initializing display.");
    display->Init(1);                                                       // Initialize the display
    display->DrawBase(TITLE, VERSION);                                      // Draw the base display layer
    display->DrawWifiIcon(false);                                           // Draw WiFi icon on display

    // Initialize the GPS unit
    ESP_LOGI("System", "Initializing GPS.");
    gps->Init();  // gps is defined in main.h
    vTaskDelay(500 / portTICK_PERIOD_MS);                                   // Wait for GPS module to initialize
    while (gps->gnss_is_initialized == false) {
        display->DrawNoSyncIcon();
        gps->Init();                                                        // Retry GPS init()
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    display->DrawSyncInProgressIcon();                                      // Draw sync icon on display

    // Set up Time Zone as defined in config.json
    GNS::Time::SetTimezone(appSettings.timezone);

    // Start up the WiFi
    ESP_LOGI("System", "Initializing and configuring WiFi.");
    GNS::StartWiFi(&appSettings);
    String ipaddr = WiFi.localIP().toString();
    display->WriteIPAddr(&ipaddr);

    // Start mDNS
    ESP_LOGI("System", "Starting mDNS Service.");
    GNS::StartMDNSService(
        appSettings.mDNSSettings.hostname,
        appSettings.mDNSSettings.host_description,
        appSettings.mDNSSettings.service_type,
        appSettings.mDNSSettings.proto,
        appSettings.mDNSSettings.port
    );

/************************************************************************************
 * Start one shot hw timer which spawns other hardware timers (see start_timers.h)  *
 * Display updating and GPS reference time obtained using hardware timers           *
 * Using hardware timers instead of FreeRTOS software timers here for simplicity    *
 ************************************************************************************/
    ESP_LOGI("System", "Starting timers.");
    // Set up the arguments for StartTimers()
    GNS::Start_Timers_Args* startTimersArgs = new GNS::Start_Timers_Args {
        .display = display,
        .gps = gps
    };

    // Create the timer arguments
    const esp_timer_create_args_t startTimersTimerArgs = {
        .callback   = &GNS::StartTimers,
        .arg        = static_cast<void*>(startTimersArgs),
        .name       = "Start Timers"
    };
    // Create the timer
    esp_timer_handle_t startTimersTimerHandle;
    ESP_ERROR_CHECK(esp_timer_create(&startTimersTimerArgs, &startTimersTimerHandle));
    // Start the one shot event
    ESP_ERROR_CHECK(esp_timer_start_once(startTimersTimerHandle, 0));
/********************************* End of one-shot timer ****************************/
    ESP_LOGI("System", "Obtaining first time stamp.");
    GNS::GPS::TimeUpdate(gps);        // gps defined in main.h
    display->DrawSyncIcon(gps->siv);  // display is defined in main.h

    ESP_LOGI("System", "Starting up NTP server.");
    ntpServer.StartUDPListener();     // ntp server is defined in main.h
    xTaskCreate(GNS::NTPServer::WaitForNTPPacket, "NTP Server", 5000, &ntpServer, 1, NULL);

    // Delete "setup and loop" tasks
    ESP_LOGI("System", "Terminating main thread.");
    vTaskDelete(NULL);
}

void loop() { ;; }  // loop() never runs
