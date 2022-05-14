#pragma once

#ifndef _MDNS_SERVER_H_
#define _MDNS_SERVER_H_

#include <mdns.h>

bool start_mdns_services(const char* hostname, const char* host_description, const char* service_type, const char* proto, uint16_t port);
void stop_mdns_services();

#endif  // _MDNS_SERVER_H