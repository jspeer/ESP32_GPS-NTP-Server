#ifndef _GPS_TIMER_H_
#define _GPS_TIMER_H_

#ifndef __TFT_ESPI
#include "ttgo_lcd.h"
#endif

#ifndef __SPARKFUN_UBLOX_GNSS_ARDUINO_LIBRARY
#include "ublox_m9n_i2c_module.h"
#endif

typedef struct GpsUpdateArgs {
    TTGO_Lcd* display;
    UBLOX_M9N* gps;
} GpsUpdateArgs_t;
void GpsUpdate(void* args);

#endif  // _GPS_TIMER_H_
