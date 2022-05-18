#ifndef GNS_INCLUDE_UPDATE_DISPLAY_H_
#define GNS_INCLUDE_UPDATE_DISPLAY_H_

#ifndef GNS_LIB_UBLOX_GPS_UBLOX_M9N_I2C_H_
#include "ublox_m9n_i2c.h"
#endif

#ifndef GNS_LIB_TFT_TTGO_TTGO_H_
#include "ttgo_lcd.h"
#endif

namespace GNS {
    typedef struct Display_Update_Args {
        GNS::UBLOX* gps;
        GNS::TTGO* display;
    } display_update_args_t;
    void DisplayUpdateTimeDate(void* args);
}

#endif  // GNS_INCLUDE_UPDATE_DISPLAY_H_
