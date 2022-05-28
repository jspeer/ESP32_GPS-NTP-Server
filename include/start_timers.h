#ifndef GNS_INCLUDE_START_TIMERS_H_
#define GNS_INCLUDE_START_TIMERS_H_

// Include the timers
#ifndef GNS_LIB_TFT_TTGO_TTGO_H_
#include "ttgo_lcd.h"
#endif

#ifndef GNS_LIB_UBLOX_GPS_UBLOX_GNSS_H_
#include "ublox_gnss.h"
#endif

namespace GNS {
    typedef struct Start_Timers_Args {
        GNS::TTGO* display;
        GNS::GPS* gps;
    } start_timers_args_t;
    void StartTimers(void* args);
}

#endif  // GNS_INCLUDE_START_TIMERS_H_
