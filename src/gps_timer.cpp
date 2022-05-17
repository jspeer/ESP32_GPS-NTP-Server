#ifndef GNS_INCLUDE_GPS_TIMER_H_
#include "gps_timer.h"
#endif

void GNS::GpsUpdate(void* args) {
    // Cast args
    GNS::GpsUpdateArgs* gpsUpdateArgs = static_cast<GNS::GpsUpdateArgs*>(args);

    // Get epoch from GPS and update RTC
    gpsUpdateArgs->gps->saveEpochToRtc();

    // Update SAT signal indicator
    if (gpsUpdateArgs->gps->gnss_is_initialized) {
        gpsUpdateArgs->display->drawSyncIcon(gpsUpdateArgs->gps->getSIV());  // TODO: We should replace this call to use the gps flags after the previous saveEpocToRtc() call at some point
    } else {
        gpsUpdateArgs->display->drawNoSyncIcon();
    }
}
