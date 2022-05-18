#ifndef GNS_INCLUDE_GPS_TIMER_H_
#define GNS_INCLUDE_GPS_TIMER_H_

#ifndef __TFT_ESPI
#include "ttgo_lcd.h"
#endif

#ifndef __SPARKFUN_UBLOX_GNSS_ARDUINO_LIBRARY
#include "ublox_m9n_i2c.h"
#endif

namespace GNS {
    void GpsUpdate(void* args);
}

#endif  // GNS_INCLUDE_GPS_TIMER_H_
