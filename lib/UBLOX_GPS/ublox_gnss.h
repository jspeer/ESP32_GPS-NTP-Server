#pragma once

#ifndef GNS_LIB_UBLOX_GPS_UBLOX_GNSS_H_
#define GNS_LIB_UBLOX_GPS_UBLOX_GNSS_H_

#ifndef GNS_LIB_GPS_GPS_H_
#include "gps.h"
#endif

// Include UBLOX GPS library
#ifndef SPARKFUN_UBLOX_ARDUINO_LIBRARY_H
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#define UBLOX_RECEIVER_MEASUREMENT_RATE (uint16_t)  10  // in ms
#define UBLOX_RECEIVER_NAVIGATION_RATE  (uint8_t)   10  // in Hz
#endif

#ifndef __u_blox_structs_h__
#include "u-blox_structs.h"
#endif

namespace GNS {

    class UBLOX_GNSS : public GPS {
    // Members
    public:

    private:
        SFE_UBLOX_GNSS* receiver = new SFE_UBLOX_GNSS;

    // Class methods
    public:
        UBLOX_GNSS(int sda, int scl);
        ~UBLOX_GNSS() = default;

    // Custom methods
    public:
        bool Init();
        void GetEpoch();
    private:

    };
}

#endif  // GNS_LIB_UBLOX_GPS_UBLOX_GNSS_H_
