#pragma once

#ifndef GNS_LIB_GPS_GPS_H_
#define GNS_LIB_GPS_GPS_H_

#include <Arduino.h>
#include <time.h>

namespace GNS {
    class GPS {
    // class methods
    public:
        ~GPS() = default;
    protected:
        GPS() = default;

    // class members
    public:
        bool gnss_is_initialized = false; // module initialized
        int32_t  epoch_ns = 0;  // epoch nanoseconds
        uint32_t epoch_us = 0;  // epoch microseconds
        uint32_t epoch    = 0;  // epoch seconds
        uint8_t  siv      = 0;  // satellites in view

    // custom pure virtual methods
    public:
        virtual bool Init()     = 0;
        virtual void GetEpoch() = 0;

    // Custom methods
    public:
        void SaveEpochToRtc();
        static void TimeUpdate(void* args);
    };
}

#endif  // GNS_LIB_GPS_GPS_H_
