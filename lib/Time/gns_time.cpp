#ifndef GNS_LIB_TIME_GNS_TIME_H_
#include "gns_time.h"
#endif

// Set the system time zone environment variable to provided string
void GNS::Time::SetTimezone(const char* timezone) {
    ESP_LOGI("Time", "Adjusting time zone to: %s", timezone);
    setenv("TZ", timezone, 1);      // This is the easiest way to ensure we can use a full TZ string
    tzset();
}

// Get the default precision time from RTC, returns tm struct
tm GNS::Time::GetTimeinfo() {
    time_t now;
    tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    ESP_LOGI("Time", "Obtaining default resolution timestamp.");
    return timeinfo;
}

// Get the high precision time from RTC, returns timespec struct
timespec GNS::Time::GetRealtime() {
    timespec realtime;
    clock_gettime(CLOCK_REALTIME, &realtime);
    ESP_LOGI("Time", "Obtaining high resolution timestamp.");
    return realtime;
}

// Save high precision timespec struct to RTC
void GNS::Time::SaveRealtime(const timespec* realtime) {
    ESP_LOGI("Time", "Saving high resolution timespec struct to RTC.");
    clock_settime(CLOCK_REALTIME, realtime);
}
