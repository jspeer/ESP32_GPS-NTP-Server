#pragma once

#ifndef GNS_LIB_TIME_GNS_TIME_H_
#define GNS_LIB_TIME_GNS_TIME_H_

#include <time.h>
#include <Arduino.h>

namespace GNS {
    class Time {
    public:
        static void SetTimezone(const char* timezone);
        static tm GetTimeinfo();
        static timespec GetRealtime();
        static void SaveRealtime(const timespec* realtime);
    };
}

#endif  // GNS_LIB_TIME_GNS_TIME_H_
