#pragma once

#ifndef GNS_LIB_UBLOX_GPS_UBLOX_M9N_I2C_H_
#define GNS_LIB_UBLOX_GPS_UBLOX_M9N_I2C_H_

// Include UBLOX GPS library
#ifndef SPARKFUN_UBLOX_ARDUINO_LIBRARY_H
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
#define UBLOX_RECEIVER_MEASUREMENT_RATE (uint16_t)  10  // in ms
#define UBLOX_RECEIVER_NAVIGATION_RATE  (uint8_t)   10  // in Hz
#endif

#ifndef __u_blox_structs_h__
#include "u-blox_structs.h"
#endif

namespace GNS {

    class UBLOX {
    // Members
    public:
        bool gnss_is_initialized = false; // module initialized
        int32_t  epoch_ns    = 0;     // epoch nanoseconds
        uint32_t epoch_us    = 0;     // epoch microseconds
        uint32_t epoch       = 0;     // epoch seconds
        uint8_t  siv             = 0;     // satellites in view

    private:
        SFE_UBLOX_GNSS* receiver = new SFE_UBLOX_GNSS;

    // Class methods
    public:
        UBLOX(int sda, int scl);
        ~UBLOX() = default;

    // Custom methods
    public:
        bool Init();
        void SaveEpochToRtc();
    private:

    };
}

#endif  // GNS_LIB_UBLOX_GPS_UBLOX_M9N_I2C_H_
