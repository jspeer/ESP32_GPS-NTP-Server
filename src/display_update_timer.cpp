#ifndef GNS_INCLUDE_DISPLAY_UPDATE_TIMER_H_
#include "display_update_timer.h"
#endif

void GNS::DisplayUpdateTimeDate(void* args) {
    // Cast args
    GNS::Display_Update_Args* displayUpdateArgs = static_cast<GNS::Display_Update_Args*>(args);

    // Update display time
    time_t now;
    tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    displayUpdateArgs->display->DisplayTime(&timeinfo);
}
