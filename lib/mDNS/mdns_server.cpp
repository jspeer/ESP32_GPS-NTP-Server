#ifndef GNS_LIB_MDNS_MDNS_SERVER_H_
#include "mdns_server.h"
#endif

#include <string.h>

bool GNS::StartMDNSService(const char* hostname, const char* host_description, const char* service_type, const char* proto, uint16_t port) {
    // Start mDNS service
    esp_err_t err = mdns_init();                                            // Initialize mDNS
    if (err) {
        return false;
    } else {
        ESP_LOGI("mDNS", "Setting mDNS parameters hostname: %s description: %s", hostname, host_description);
        mdns_hostname_set(hostname);                                        // Set the mDNS hostname
        mdns_instance_name_set(host_description);                           // Set the mDNS instance name
        ESP_LOGI("mDNS", "Adding service %s %s/%i", service_type, proto, port);
        mdns_service_add(NULL, service_type, proto, port, NULL, 0);          // Add the NTP service to mDNS

        // Start NetBIOS resolver -- because Windows
        char nbnshostname[100];
        strcpy(nbnshostname, hostname);
        strcat(nbnshostname, ".local");  // Adding TLD of .local to match mDNS
        ESP_LOGI("mDNS", "Starting NetBIOS Name Service for %s", nbnshostname);
        NBNS.begin(nbnshostname);

        return true;
    }
}
