#ifndef _NTP_SERVER_H_
#include "ntpServer.h"
#endif

void WaitForNTPPacket(void* args) {
    SendNTPReplyArgs* sendNTPReplyArgs = static_cast<SendNTPReplyArgs*>(args);

    for (;;) {
        if (sendNTPReplyArgs->UDP->parsePacket()) {
            xTaskCreate(SendNTPReply, "Send NTP Reply", 5000, sendNTPReplyArgs, 1, NULL);
        }

        vTaskDelay(1 / portTICK_RATE_MS);  // Very short delay to give FreeRTOS watchdog timer a chance
    }
}

void SendNTPReply(void* args) {
    SendNTPReplyArgs* sendNtpReplyArgs = static_cast<SendNTPReplyArgs*>(args);

    // Set the timestamp for the RX packet
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    uint32_t rxTm_s = (uint32_t)ts.tv_sec;
    uint32_t rxTm_f = (uint32_t)ts.tv_nsec;

    // Create the client packet
    NTPPacket clientPacket;
    memset(&clientPacket, 0, sizeof(NTPPacket));

    // Get the remote address information
    IPAddress remoteIP = sendNtpReplyArgs->UDP->remoteIP();
    int remotePort = sendNtpReplyArgs->UDP->remotePort();

    // Read the RX packet
    sendNtpReplyArgs->UDP->read(((char*) &clientPacket), NTP_PACKET_SIZE);

    // Create and zero out the reply packet.
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
    replyPacket.rxTm_s          = __bswap_32(rxTm_s + EPOCH_NTP64_OFFSET);
    replyPacket.rxTm_f          = __bswap_32(rxTm_f);

    // Assign the source tx time to the originating time in the reply packet
    replyPacket.origTm_s        = clientPacket.txTm_s;
    replyPacket.origTm_f        = clientPacket.txTm_f;

    // Assign the reference timestamp in the reply packet
    replyPacket.refTm_s = __bswap_32(sendNtpReplyArgs->gps->epoch + EPOCH_NTP64_OFFSET);  // Apply epoch ntp64 offset
    replyPacket.refTm_f = __bswap_32((uint32_t)sendNtpReplyArgs->gps->epoch_ns);

    // Assign the tx timestamp in the reply packet
    clock_gettime(CLOCK_REALTIME, &ts);
    replyPacket.txTm_s = __bswap_32((uint32_t)ts.tv_sec + EPOCH_NTP64_OFFSET);  // Apply epoch ntp64 offset
    replyPacket.txTm_f = __bswap_32((uint32_t)ts.tv_nsec);

    // Send the TX packet
    sendNtpReplyArgs->UDP->beginPacket(remoteIP, remotePort);
    sendNtpReplyArgs->UDP->write((uint8_t*) &replyPacket, sizeof(replyPacket));
    sendNtpReplyArgs->UDP->endPacket();
    
    // Destroy the task
    vTaskDelete(NULL);
}
