#ifndef GNS_LIB_NTP_SERVER_NTP_SERVER_H_
#include "ntp_server.h"
#endif

// Constructor with initialization list
GNS::NTPServer::NTPServer(GNS::UBLOX* ublox) : port{NTP_PORT}, gps{ublox} { }  // NTP_PORT is defined in ntp_server.h

GNS::NTPServer::~NTPServer() {
    StopUDPListener();
}

// Grab the RTC time stamp and apply it to the object timestamp
void GNS::NTPServer::GetRealtime() {
    clock_gettime(CLOCK_REALTIME, &this->timestamp);
}

// Start UDP listener on port (defined in constructor)
void GNS::NTPServer::StartUDPListener() {
    ESP_LOGI("UDP", "Starting UDP listener on 0.0.0.0:%i", this->port);
    uint8_t err = this->UDP.begin(NTP_PORT);
    if (err == 0) {
        ESP_LOGE("UDP", "Failed to start UDP listener. Entering deep sleep.");
        esp_deep_sleep_start();
    }
}

// Terminate UDP listener
void GNS::NTPServer::StopUDPListener() {
    ESP_LOGI("UDP", "Terminating NTP UDP listener.");
    this->UDP.stop();
}

// Loop to handle incoming UDP packets
void GNS::NTPServer::WaitForNTPPacket(void* args) {
    GNS::NTPServer* ntpServer = static_cast<GNS::NTPServer*>(args);

    for (;;) {  // Infinite loop
        if (ntpServer->UDP.parsePacket()) {  // Packet was found in UDP queue
            ESP_LOGI("UDP", "Got UDP packet. Spawning NTP reply thread.");
            ntpServer->GetRealtime();
            xTaskCreate(GNS::NTPServer::SendNTPReply, "Send NTP Reply", 5000, ntpServer, 1, NULL);  // Spawn task to reply to this packet
        }

        vTaskDelay(1 / portTICK_RATE_MS);  // Very short delay to give FreeRTOS watchdog timer a chance
    }
}

// Task responsible for handing UDP packet reply
void GNS::NTPServer::SendNTPReply(void* args) {
    GNS::NTPServer* ntpServer = static_cast<GNS::NTPServer*>(args);

    ESP_LOGI("NTP-S", "Collecting RX timestamp.");
    // Set the timestamp for the RX packet
    uint32_t rxTm_s = (uint32_t)ntpServer->timestamp.tv_sec;
    uint32_t rxTm_f = (uint32_t)ntpServer->timestamp.tv_nsec;

    // Create the client packet
    ESP_LOGI("NTP-C", "Reading client packet");
    GNS::NTP_Packet clientPacket;
    memset(&clientPacket, 0, sizeof(GNS::NTP_Packet));

    // Read the RX packet
    ntpServer->UDP.read(((char*) &clientPacket), NTP_PACKET_SIZE);

    // Get the remote address information
    ESP_LOGI("NTP-C", "Getting remote address and port.");
    IPAddress remoteIP = ntpServer->UDP.remoteIP();
    int remotePort = ntpServer->UDP.remotePort();

    // Create and zero out the reply packet.
    ESP_LOGI("NTP-R", "Starting reply packet.");
    GNS::NTP_Packet replyPacket;
    memset(&replyPacket, 0, sizeof(GNS::NTP_Packet));

    // Set the first bytes bits to 00,011,100 for li = 0, vn = 3, and mode = 4. The rest will be left set to zero.
    *((char*) &replyPacket + 0) = 0b00011100;

    // Set the stratum, 1 for reference clock
    replyPacket.stratum         = 0b00000001;

    // Set the polling interval
    replyPacket.poll            = 4;

    // Set the clock precision, in this case we're using 9 decimal places or 0xF7
    replyPacket.precision       = 0xF7;

    // Set the root delay and dispersion
    replyPacket.rootDelay       = __bswap_32(0x00);
    replyPacket.rootDispersion  = __bswap_32(0x50);

    // Set the reference identifier
    replyPacket.refId           = __bswap_32(0x47505300);  // "GPS\0" in hex

    // Assign the receive time in the reply packet
    ESP_LOGI("NTP-R", "Assigning RX timestamp.");
    replyPacket.rxTm_s          = __bswap_32(rxTm_s + EPOCH_NTP64_OFFSET);
    replyPacket.rxTm_f          = __bswap_32(rxTm_f);

    // Assign the source tx time to the originating time in the reply packet
    ESP_LOGI("NTP-R", "Assigning client TX timestamp.");
    replyPacket.origTm_s        = clientPacket.txTm_s;
    replyPacket.origTm_f        = clientPacket.txTm_f;

    // Assign the reference timestamp in the reply packet
    ESP_LOGI("NTP-R", "Assigning GPS reference timestamp.");
    replyPacket.refTm_s = __bswap_32(ntpServer->gps->epoch + EPOCH_NTP64_OFFSET);  // Apply epoch ntp64 offset
    replyPacket.refTm_f = __bswap_32((uint32_t)ntpServer->gps->epoch_ns);

    // Assign the tx timestamp in the reply packet
    ESP_LOGI("NTP-R", "Assigning TX timestamp.");
    ntpServer->GetRealtime();
    replyPacket.txTm_s = __bswap_32((uint32_t)ntpServer->timestamp.tv_sec + EPOCH_NTP64_OFFSET);  // Apply epoch ntp64 offset
    replyPacket.txTm_f = __bswap_32((uint32_t)ntpServer->timestamp.tv_nsec);

    // Send the TX packet
    ESP_LOGI("NTP-S", "Sending the reply packet");
    ntpServer->UDP.beginPacket(remoteIP, remotePort);
    ntpServer->UDP.write((uint8_t*) &replyPacket, sizeof(replyPacket));
    ntpServer->UDP.endPacket();

    // Destroy the task
    ESP_LOGI("UDP", "Terminating reply thread.");
    vTaskDelete(NULL);
}
