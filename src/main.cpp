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
    Serial.println("Initializing display.");  // display is defined in main.h
    display->init(1);                                                       // Initialize the display
    display->drawBase(TITLE, VERSION);                                      // Draw the base display layer
    display->drawWifiIcon(false);                                           // Draw WiFi icon on display

    // Initialize the GPS unit
    Serial.print("Initializing GPS.");
    gps->init();  // gps is defined in main.h
    vTaskDelay(500 / portTICK_PERIOD_MS);                                   // Wait for GPS module to initialize
    while (gps->gnss_is_initialized == false) {
        Serial.print(".");
        display->drawNoSyncIcon();
        gps->init();                                                        // Retry GPS init()
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    display->drawSyncInProgressIcon();                                      // Draw sync icon on display
    Serial.println();

    // Set up Time Zone as defined in main.h
    Serial.printf("Setting up time zone (%s).\n", appSettings.timezone);
    setenv("TZ", appSettings.timezone, 1); tzset();                         // This is the easiest way to ensure we can use a full TZ string

    // Start up the WiFi
    Serial.println("Initializing and configuring WiFi.");
    GNS::startWifi(&appSettings, display);

    // Start mDNS
    GNS::startMdnsService(appSettings.mDNSSettings.hostname, appSettings.mDNSSettings.host_description, appSettings.mDNSSettings.service_type, appSettings.mDNSSettings.proto, appSettings.mDNSSettings.port);

/************************************************************************************
 * Start one shot hw timer which spawns other hardware timers (see startTimers.h)   *
 * Display updating and GPS reference time obtained using hardware timers           *
 * Using hardware timers instead of FreeRTOS software timers here for simplicity    *
 ************************************************************************************/
    Serial.println("Starting timers.");
    // Set up the arguments for StartTimers()
    GNS::StartTimersArgs* startTimersArgs = new GNS::StartTimersArgs {
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

    Serial.println("Obtaining first time stamp from GPS.");
    gps->saveEpochToRtc();
    display->drawSyncIcon(gps->getSIV());

    Serial.println("Starting up NTP server.");
    ntpServer.startUDPListener();  // ntpServer is defined in main.h
    xTaskCreate(GNS::NTPServer::waitForNTPPacket, "NTP Server", 5000, &ntpServer, 1, NULL);

    // Delete "setup and loop" tasks
    Serial.println("Terminating main thread, relying on spawned threads now.");
    vTaskDelete(NULL);
}

void loop() { ;; }  // loop() never runs
