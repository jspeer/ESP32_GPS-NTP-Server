#include "displayUpdateTimer.h"

void DisplayUpdateTimeDate(void* args) {
    // Cast args
    DisplayUpdateArgs* displayUpdateArgs = static_cast<DisplayUpdateArgs*>(args);

    // Update display time
    time_t now;
    tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    displayUpdateArgs->display->DisplayTime(&timeinfo);
}
