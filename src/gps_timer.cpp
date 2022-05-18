#ifndef GNS_INCLUDE_GPS_TIMER_H_
#include "gps_timer.h"
#endif

void GNS::GpsUpdate(void* args) {
    // Cast args
    GNS::UBLOX* gps = static_cast<GNS::UBLOX*>(args);

    // Get epoch from GPS and update RTC
    gps->SaveEpochToRtc();
}
