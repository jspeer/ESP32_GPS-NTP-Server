#ifndef GNS_INCLUDE_START_TIMERS_H_
#define GNS_INCLUDE_START_TIMERS_H_

// Include the timers
#include "gps_timer.h"
#include "display_update_timer.h"

#ifndef __TFT_ESPI
#include "ttgo_lcd.h"
#endif

#ifndef GNS_LIB_UBLOX_GPS_UBLOX_M9N_I2C_H_
#include "ublox_m9n_i2c.h"
#endif

namespace GNS {
    typedef struct Start_Timers_Args {
        GNS::TTGO* display;
        GNS::UBLOX* gps;
    } start_timers_args_t;
    void StartTimers(void* args);
}

#endif  // GNS_INCLUDE_START_TIMERS_H_
