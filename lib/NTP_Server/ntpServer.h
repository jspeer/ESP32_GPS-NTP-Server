#ifndef _NTP_SERVER_H_
#define _NTP_SERVER_H_

// Definitions for NTP server
#include <byteswap.h>
#include <WiFiUdp.h>
#include <time.h>

#include "ublox_m9n_i2c_module.h"

#define NTP_PORT 123
#define NTP_PACKET_SIZE 48
#define EPOCH_NTP64_OFFSET 2208988800   // Difference from NTP64 to EPOCH (Jan 1 1900 to Jan 1 1970)

typedef struct NTPPacket {
    uint8_t li_vn_mode;      // Eight bits. li, vn, and mode.
                            // li.   Two bits.   Leap indicator.
                            // vn.   Three bits. Version number of the protocol.
                            // mode. Three bits. Client will pick mode 3 for client.

    uint8_t stratum;         // Eight bits. Stratum level of the local clock.
    uint8_t poll;            // Eight bits. Maximum interval between successive messages.
    uint8_t precision;       // Eight bits. Precision of the local clock.

    uint32_t rootDelay;      // 32 bits. Total round trip delay time.
    uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
    uint32_t refId;          // 32 bits. Reference clock identifier.

    uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
    uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.

    uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
    uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.

    uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
    uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.

    uint32_t txTm_s;         // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
    uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.
} ntp_packet_t;              // Total: 384 bits or 48 bytes.

typedef struct SendNTPReplyArgs {
    WiFiUDP* UDP;
    UBLOX_M9N* gps;
} send_ntp_reply_t;

void SendNTPReply(SendNTPReplyArgs* args);

#endif  // _NTP_SERVER_H_
