#pragma once

#ifndef GNS_LIB_UBLOX_GPS_UBLOX_M9N_I2C_H_
#define GNS_LIB_UBLOX_GPS_UBLOX_M9N_I2C_H_

// Include TFT library
#ifndef __SPARKFUN_UBLOX_GNSS_ARDUINO_LIBRARY
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
#define UBLOX_RECEIVER_MEASUREMENT_RATE (uint16_t)  10  // in ms
#define UBLOX_RECEIVER_NAVIGATION_RATE  (uint8_t)   10  // in Hz
#define UBLOX_USEC_PER_SEC 1000000                      // usecs per second
#define __SPARKFUN_UBLOX_GNSS_ARDUINO_LIBRARY
#endif
#ifndef __u_blox_structs_h__
#include "u-blox_structs.h"
#endif

#include "time.h"

namespace GNS {
    class UBLOX {
    // Members
    public:
        int32_t  epoch_ns    = 0;     // epoch nanoseconds
        uint32_t epoch_us    = 0;     // epoch microseconds
        uint32_t epoch       = 0;     // epoch seconds
        uint8_t  siv         = 0;     // satellites in view
        bool gnss_is_initialized = false;
    private:
        SFE_UBLOX_GNSS* receiver = new SFE_UBLOX_GNSS;

    // Class methods
    public:
        UBLOX(int sda, int scl);
        ~UBLOX() = default;

    // Custom methods
    public:
        bool Init();
        bool DateAndTimeValid();
        uint8_t GetSIV(uint16_t maxWait = (uint16_t) 1100U);
        uint32_t GetUnixEpoch();
        int32_t GetUnixEpochNs();
        bool GetUnixTime();
        void SaveEpochToRtc();
    private:

    };
}

#endif  // GNS_LIB_UBLOX_GPS_UBLOX_M9N_I2C_H_
