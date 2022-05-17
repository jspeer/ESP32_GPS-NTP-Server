#ifndef GNS_LIB_MDNS_MDNS_SERVER_H_
#include "mDNS_Server.h"
#endif

bool GNS::startMdnsService(const char* hostname, const char* host_description, const char* service_type, const char* proto, uint16_t port) {
    // Start mDNS service
    esp_err_t err = mdns_init();                                            // Initialize mDNS
    if (err) {
        return false;
    } else {
        mdns_hostname_set(hostname);                                        // Set the mDNS hostname
        mdns_instance_name_set(host_description);                           // Set the mDNS instance name
        mdns_service_add(NULL, service_type, proto, port, NULL, 0);          // Add the NTP service to mDNS
        return true;
    }
}
