#ifndef GNS_INCLUDE_START_TIMERS_H_
#include "start_timers.h"
#endif

void GNS::StartTimers(void* args) {
    GNS::Start_Timers_Args* startTimersArgs = static_cast<GNS::Start_Timers_Args*>(args);  // Cast the void* back into the arguments struct

/************************************************************************************
 * Start DisplayUpdateTimeDate() timer with an interval of 1 second                 *
 ************************************************************************************/
    // Set up the arguments for DisplayUpdateTimeDate()
    GNS::Display_Update_Args* displayUpdateArgs = new GNS::Display_Update_Args {
        .gps     = startTimersArgs->gps,
        .display = startTimersArgs->display
    };
    // Create the timer arguments
    const esp_timer_create_args_t displayUpdateTimerArgs = {
        .callback   = &GNS::DisplayUpdateTimeDate,
        .arg        = static_cast<void*>(displayUpdateArgs),
        .name       = "Display Update Time/Date"
    };
    // Create the timer
    esp_timer_handle_t displayUpdateTimerHandle;
    ESP_ERROR_CHECK(esp_timer_create(&displayUpdateTimerArgs, &displayUpdateTimerHandle));
    // Start the timer, update the screen every second
    Serial.println("Starting Display Update timer.");
    ESP_ERROR_CHECK(esp_timer_start_periodic(displayUpdateTimerHandle, 1000000));

/************************************************************************************
 * Start GpsUpdate() timer with an interval of 60 seconds                           *
 ************************************************************************************/
    // Create the timer arguments
    const esp_timer_create_args_t gpsUpdateTimerArgs = {
        .callback   = &GNS::GpsUpdate,
        .arg        = static_cast<void*>(startTimersArgs->gps),
        .name       = "GPS Update"
    };
    // Create the timer
    esp_timer_handle_t gpsUpdateTimerHandle;
    ESP_ERROR_CHECK(esp_timer_create(&gpsUpdateTimerArgs, &gpsUpdateTimerHandle));
    // Start the timer, update the RTC every minute
    Serial.println("Starting GPS Update timer.");
    ESP_ERROR_CHECK(esp_timer_start_periodic(gpsUpdateTimerHandle, 60000000));
}
