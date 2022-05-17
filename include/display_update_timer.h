#ifndef GNS_INCLUDE_DISPLAY_UPDATE_TIMER_H_
#define GNS_INCLUDE_DISPLAY_UPDATE_TIMER_H_

#ifndef __TFT_ESPI
#include "ttgo_lcd.h"
#endif

namespace GNS {
    typedef struct DisplayUpdateArgs {
        GNS::TTGO_Lcd* display;
    } DisplayUpdateArgs_t;
    void DisplayUpdateTimeDate(void* args);
}

#endif  // GNS_INCLUDE_DISPLAY_UPDATE_TIMER_H_
