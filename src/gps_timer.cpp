#ifndef GNS_INCLUDE_GPS_TIMER_H_
#include "gps_timer.h"
#endif

void GNS::GpsUpdate(void* args) {
    // Cast args
    GNS::GPS_Update_Args* gpsUpdateArgs = static_cast<GNS::GPS_Update_Args*>(args);

    // Get epoch from GPS and update RTC
    gpsUpdateArgs->gps->SaveEpochToRtc();

    // Update SAT signal indicator
    if (gpsUpdateArgs->gps->gnss_is_initialized) {
        gpsUpdateArgs->display->DrawSyncIcon(gpsUpdateArgs->gps->siv);
    } else {
        gpsUpdateArgs->display->DrawNoSyncIcon();
    }
}
