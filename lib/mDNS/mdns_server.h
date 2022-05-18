#pragma once

#ifndef GNS_LIB_MDNS_MDNS_SERVER_H_
#define GNS_LIB_MDNS_MDNS_SERVER_H_

#include <mdns.h>

#ifdef ARDUINO_ARCH_ESP32
#ifndef __ARDUHAL_LOG_H__
#include <esp32-hal-log.h>
#endif
#endif

namespace GNS {
    bool StartMDNSService(const char* hostname, const char* host_description, const char* service_type, const char* proto, uint16_t port);
}

#endif  // GNS_LIB_MDNS_MDNS_SERVER_H_
