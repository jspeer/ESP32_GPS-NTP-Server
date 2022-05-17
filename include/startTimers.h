#ifndef GNS_INCLUDE_START_TIMERS_H_
#define GNS_INCLUDE_START_TIMERS_H_

// Include the timers
#include "gpsTimer.h"
#include "displayUpdateTimer.h"

#ifndef __TFT_ESPI
#include "ttgo_lcd.h"
#endif

#ifndef __SPARKFUN_UBLOX_GNSS_ARDUINO_LIBRARY
#include "ublox_m9n_i2c_module.h"
#endif

namespace GNS {
    typedef struct StartTimersArgs {
        GNS::TTGO_Lcd* display;
        GNS::UBLOX_M9N* gps;
    } StartTimersArgs_t;
    void StartTimers(void* args);
}

#endif  // GNS_INCLUDE_START_TIMERS_H_
