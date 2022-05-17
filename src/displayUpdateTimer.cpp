#ifndef GNS_INCLUDE_DISPLAY_UPDATE_TIMER_H_
#include "displayUpdateTimer.h"
#endif

void GNS::DisplayUpdateTimeDate(void* args) {
    // Cast args
    GNS::DisplayUpdateArgs* displayUpdateArgs = static_cast<GNS::DisplayUpdateArgs*>(args);

    // Update display time
    time_t now;
    tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    displayUpdateArgs->display->displayTime(&timeinfo);
}
