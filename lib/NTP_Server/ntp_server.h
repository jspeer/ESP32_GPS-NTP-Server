#ifndef GNS_LIB_NTP_SERVER_NTP_SERVER_H_
#define GNS_LIB_NTP_SERVER_NTP_SERVER_H_

// Definitions for NTP server
#include <byteswap.h>
#include <WiFiUdp.h>
#include <time.h>

#include "ublox_m9n_i2c.h"

#define NTP_PORT 123
#define NTP_PACKET_SIZE 48
#define EPOCH_NTP64_OFFSET 2208988800   // Difference from NTP64 to EPOCH (Jan 1 1900 to Jan 1 1970)

namespace GNS {
    // Convert NTP server to OOP
    class NTPServer {
    // Default methods
    public:
        NTPServer(GNS::UBLOX* gps);
        ~NTPServer();

    // public members
    public:
        WiFiUDP UDP;
        timespec timestamp;
        uint16_t port;
        GNS::UBLOX* gps;

    // private members
    private:

    // public methods
    public:
        static void SendNTPReply(void* args);
        static void WaitForNTPPacket(void* args);
        void StartUDPListener();
        void StopUDPListener();

    // private methods
    private:
        void GetRealtime();
    };

    typedef struct NTP_Packet {
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

    void WaitForNTPPacket(void* args);
    void SendNTPReply(void* args);
}

#endif  // GNS_LIB_NTP_SERVER_NTP_SERVER_H_
