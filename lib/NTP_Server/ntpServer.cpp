#ifndef _NTP_SERVER_H_
#include "ntpServer.h"
#endif

NTPServer::NTPServer(UBLOX_M9N* gps) {
    this->port = NTP_PORT;
    this->gps = gps;
}

NTPServer::~NTPServer() {
    StopUDPListener();
}

void NTPServer::GetRealtime() {
    clock_gettime(CLOCK_REALTIME, &this->timestamp);
}

void NTPServer::StartUDPListener() {
    Serial.printf("Starting NTP UDP listener on 0.0.0.0:%i.\n", this->port);
    uint8_t err = this->UDP.begin(NTP_PORT);
    if (err == 0) {
        Serial.println("Failed to start NTP UDP listener. Shutting down.");
        esp_deep_sleep_start();
    }
}

void NTPServer::StopUDPListener() {
    Serial.println("Terminating NTP UDP listener.");
    this->UDP.stop();
}

void NTPServer::WaitForNTPPacket(void* args) {
    NTPServer* ntpServer = static_cast<NTPServer*>(args);

    for (;;) {
        if (ntpServer->UDP.parsePacket()) {
            Serial.println("Got UDP packet. Grabbing timestamp.");
            ntpServer->GetRealtime();
            Serial.println("Spawning reply thread.");
            xTaskCreate(NTPServer::SendNTPReply, "Send NTP Reply", 5000, ntpServer, 1, NULL);
        }

        vTaskDelay(1 / portTICK_RATE_MS);  // Very short delay to give FreeRTOS watchdog timer a chance
    }
}

void NTPServer::SendNTPReply(void* args) {
    NTPServer* ntpServer = static_cast<NTPServer*>(args);

    // Set the timestamp for the RX packet
    uint32_t rxTm_s = (uint32_t)ntpServer->timestamp.tv_sec;
    uint32_t rxTm_f = (uint32_t)ntpServer->timestamp.tv_nsec;

    // Create the client packet
    NTPPacket clientPacket;
    memset(&clientPacket, 0, sizeof(NTPPacket));

    // Get the remote address information
    Serial.println("Getting remote connection information.");
    IPAddress remoteIP = ntpServer->UDP.remoteIP();
    int remotePort = ntpServer->UDP.remotePort();

    // Read the RX packet
    Serial.println("Reading client packet.");
    ntpServer->UDP.read(((char*) &clientPacket), NTP_PACKET_SIZE);

    // Create and zero out the reply packet.
    Serial.println("Starting reply packet.");
    NTPPacket replyPacket;
    memset(&replyPacket, 0, sizeof(NTPPacket));

    // Set the first bytes bits to 00,011,100 for li = 0, vn = 3, and mode = 4. The rest will be left set to zero.
    *((char*) &replyPacket + 0) = 0b00011100;

    // Set the statum, 1 for reference clock
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
    Serial.println("Assigning RX time.");
    replyPacket.rxTm_s          = __bswap_32(rxTm_s + EPOCH_NTP64_OFFSET);
    replyPacket.rxTm_f          = __bswap_32(rxTm_f);

    // Assign the source tx time to the originating time in the reply packet
    Serial.println("Assinging client TX time.");
    replyPacket.origTm_s        = clientPacket.txTm_s;
    replyPacket.origTm_f        = clientPacket.txTm_f;

    // Assign the reference timestamp in the reply packet
    Serial.println("Assigning GPS reference time.");
    replyPacket.refTm_s = __bswap_32(ntpServer->gps->epoch + EPOCH_NTP64_OFFSET);  // Apply epoch ntp64 offset
    replyPacket.refTm_f = __bswap_32((uint32_t)ntpServer->gps->epoch_ns);

    // Assign the tx timestamp in the reply packet
    Serial.println("Assinging TX realtime.");
    ntpServer->GetRealtime();
    replyPacket.txTm_s = __bswap_32((uint32_t)ntpServer->timestamp.tv_sec + EPOCH_NTP64_OFFSET);  // Apply epoch ntp64 offset
    replyPacket.txTm_f = __bswap_32((uint32_t)ntpServer->timestamp.tv_nsec);

    // Send the TX packet
    Serial.println("Sending the reply packet.");
    ntpServer->UDP.beginPacket(remoteIP, remotePort);
    ntpServer->UDP.write((uint8_t*) &replyPacket, sizeof(replyPacket));
    ntpServer->UDP.endPacket();

    // Destroy the task
    Serial.println("Terminating reply thread.");
    vTaskDelete(NULL);
}
