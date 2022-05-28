#ifndef GNS_LIB_GPS_GPS_H_
#include "GPS.h"
#endif  // GNS_LIB_GPS_GPS_H_

void GNS::GPS::SaveEpochToRtc() {
    this->GetEpoch();

    // Use time.h clock_settime() for high resolution timestamp
    ESP_LOGI("RTC", "Setting RTC to GPS time.");
    const timespec res = { .tv_sec = (time_t)this->epoch, .tv_nsec = (long)this->epoch_us };
    clock_settime(CLOCK_REALTIME, &res);
}

void GNS::GPS::TimeUpdate(void* args) {
    GNS::GPS* gps = static_cast<GNS::GPS*>(args);

    gps->SaveEpochToRtc();
}
