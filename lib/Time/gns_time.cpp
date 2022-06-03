#ifndef GNS_LIB_TIME_GNS_TIME_H_
#include "gns_time.h"
#endif

// Get the default precision time, returns tm struct
tm GNS::Time::GetTimeinfo() {
    time_t now;
    tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    ESP_LOGI("Time", "Obtaining default resolution timestamp.");
    return timeinfo;
}

// Get the high precision time, returns timespec struct
timespec GNS::Time::GetRealtime() {
    timespec realtime;
    clock_gettime(CLOCK_REALTIME, &realtime);
    ESP_LOGI("Time", "Obtaining high resolution timestamp.");
    return realtime;
}

void GNS::Time::SaveRealtime(const timespec* realtime) {
    clock_settime(CLOCK_REALTIME, realtime);
}
