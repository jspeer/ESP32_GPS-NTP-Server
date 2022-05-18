#include <time.h>

#ifndef GNS_INCLUDE_UPDATE_DISPLAY_H_
#include "update_display.h"
#endif

#ifndef GNS_LIB_WIFI_WIFI_HANDLER_H_
#include "wifi_handler.h"
#endif

#ifndef GNS_LIB_UBLOX_GPS_UBLOX_M9N_I2C_H_
#include "ublox_m9n_i2c.h"
#endif

void GNS::DisplayUpdateTimeDate(void* args) {
    // Cast args
    GNS::Display_Update_Args* displayUpdateArgs = static_cast<Display_Update_Args*>(args);

    // Update the wifi status indicator
    displayUpdateArgs->display->DrawWifiIcon(WiFi.status() == WL_CONNECTED);

    // Update the satellites in view indicator
    if (displayUpdateArgs->gps->gnss_is_initialized == false) {
        displayUpdateArgs->display->DrawNoSyncIcon();
    } else {
        displayUpdateArgs->display->DrawSyncIcon(displayUpdateArgs->gps->siv);
    }

    // Update display time
    time_t now;
    tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    displayUpdateArgs->display->DisplayTime(&timeinfo);
}
