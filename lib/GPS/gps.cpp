#ifndef GNS_LIB_GPS_GPS_H_
#include "GPS.h"
#endif  // GNS_LIB_GPS_GPS_H_

void GNS::GPS::TimeUpdate(void* args) {
    GNS::GPS* gps = static_cast<GNS::GPS*>(args);

    gps->GetEpoch();
    ESP_LOGI("GPS", "Saving GPS time to RTC.");
    const timespec res = { .tv_sec = (time_t)gps->epoch, .tv_nsec = (long)gps->epoch_us };
    GNS::Time::SaveRealtime(&res);
}
