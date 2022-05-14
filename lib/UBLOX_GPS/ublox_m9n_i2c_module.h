#pragma once

// Include TFT library
#ifndef __SPARKFUN_UBLOX_GNSS_ARDUINO_LIBRARY
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
#define UBLOX_I2C_SDA_GPIO_PIN 13
#define UBLOX_I2C_SCL_GPIO_PIN 15
#define UBLOX_RECEIVER_MEASUREMENT_RATE (uint16_t)  10  // in ms
#define UBLOX_RECEIVER_NAVIGATION_RATE  (uint8_t)   10  // in Hz
#define UBLOX_USEC_PER_SEC 1000000                      // usecs per second
#define __SPARKFUN_UBLOX_GNSS_ARDUINO_LIBRARY
#endif
#ifndef __u_blox_structs_h__
#include "u-blox_structs.h"
#endif

#include "time.h"

class UBLOX_M9N {
// Members
public:
    int32_t  epoch_ns    = 0;     // epoch nanoseconds
    uint32_t epoch_us    = 0;     // epoch microseconds
    uint32_t epoch       = 0;     // epoch seconds
    bool gnss_is_initialized = false;
private:
    SFE_UBLOX_GNSS* receiver = new SFE_UBLOX_GNSS;

// Class methods
public:
    UBLOX_M9N();
    ~UBLOX_M9N() = default;

// Custom methods
public:
    bool init();
    bool dateAndTimeValid();
    uint8_t getSIV(uint16_t maxWait = (uint16_t) 1100U);
    // void saveEpochToRtc();
    uint32_t getUnixEpoch();
    int32_t getUnixEpochNs();
    bool getUnixTime();
    void saveEpochToRtc();
private:

};
