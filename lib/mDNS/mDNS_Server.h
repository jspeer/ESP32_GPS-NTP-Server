#pragma once

#ifndef GNS_LIB_MDNS_MDNS_SERVER_H_
#define GNS_LIB_MDNS_MDNS_SERVER_H_

#include <mdns.h>

namespace GNS {
    bool startMdnsService(const char* hostname, const char* host_description, const char* service_type, const char* proto, uint16_t port);
}

#endif  // GNS_LIB_MDNS_MDNS_SERVER_H
