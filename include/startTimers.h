#ifndef _START_TIMERS_H_
#define _START_TIMERS_H_

// Include the timers
#include "gpsTimer.h"
#include "displayUpdateTimer.h"

#ifndef __TFT_ESPI
#include "ttgo_lcd.h"
#endif

#ifndef __SPARKFUN_UBLOX_GNSS_ARDUINO_LIBRARY
#include "ublox_m9n_i2c_module.h"
#endif

typedef struct StartTimersArgs {
    TTGO_Lcd* display;
    UBLOX_M9N* gps;
} StartTimersArgs_t;
void StartTimers(void* args);

#endif  // _START_TIMERS_H_
