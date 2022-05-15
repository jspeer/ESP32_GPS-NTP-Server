#pragma once

#ifndef _MDNS_SERVER_H_
#define _MDNS_SERVER_H_

#include <mdns.h>

bool startMdnsService(const char* hostname, const char* host_description, const char* service_type, const char* proto, uint16_t port);

#endif  // _MDNS_SERVER_H