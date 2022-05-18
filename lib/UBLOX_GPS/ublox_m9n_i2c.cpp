#ifndef GNS_LIB_UBLOX_GPS_UBLOX_M9N_I2C_H_
#include "ublox_m9n_i2c.h"
#endif

#include <time.h>

// Set up i2c wires
GNS::UBLOX::UBLOX(int sda, int scl) {
    Wire.setPins(sda, scl);
    Wire.begin();
}

// Initialize GNSS unit
bool GNS::UBLOX::Init() {
    if (!this->gnss_is_initialized) {
        if (this->receiver->begin() == true) {
            this->receiver->setI2COutput(COM_TYPE_UBX);
            this->receiver->setMeasurementRate(UBLOX_RECEIVER_MEASUREMENT_RATE);
            this->receiver->setNavigationFrequency(UBLOX_RECEIVER_NAVIGATION_RATE);
            this->gnss_is_initialized = true;
        }
    }
    return this->gnss_is_initialized;
}

// Obtain and save the epoch time to ESP RTC from the receiver
void GNS::UBLOX::SaveEpochToRtc() {
    // Get the packet from the receiver
    epoch = this->receiver->getUnixEpoch(epoch_us);           // Call receiver->getUnixEpoch(), which in turn should call receiver->getPVT() if the data is stale
    epoch_ns = this->receiver->packetUBXNAVPVT->data.nano;    // Grab the nano seconds from the current receiver packet
    this->siv = this->receiver->packetUBXNAVPVT->data.numSV;  // Get the SIV from the current receiver packet

    // Use time.h clock_settime() for high resolution timestamp
    const timespec res = { .tv_sec = (time_t)epoch, .tv_nsec = (long)epoch_us };
    clock_settime(CLOCK_REALTIME, &res);
}

void GNS::UBLOX::TimeUpdate(void* args) {
    GNS::UBLOX* gps = static_cast<GNS::UBLOX*>(args);

    gps->SaveEpochToRtc();
}
