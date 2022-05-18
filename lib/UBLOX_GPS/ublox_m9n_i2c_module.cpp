#ifndef GNS_LIB_UBLOX_GPS_UBLOX_M9N_I2C_MODULE_H_
#include "ublox_m9n_i2c_module.h"
#endif

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

// Returns unix epoch seconds
uint32_t GNS::UBLOX::GetUnixEpoch() {
    return this->receiver->getUnixEpoch();
}

// Sets epoch and epoch_us
bool GNS::UBLOX::GetUnixTime() {
    epoch = this->receiver->getUnixEpoch(epoch_us);           // Call receiver->getUnixEpoch(), which in turn should call receiver->getPVT() if the data is stale
    epoch_ns = this->receiver->packetUBXNAVPVT->data.nano;    // Grab the nano seconds from the data struct since it's fresh and doesn't require a new call to receiver->getPVT()
    return (epoch) ? true : false;
}

// Returns bool if time and date are both valid
bool GNS::UBLOX::DateAndTimeValid() {
    return (this->receiver->getTimeValid() && this->receiver->getDateValid());
}

// Returns the number of Satellites in View
uint8_t GNS::UBLOX::GetSIV(uint16_t maxWait) {
    return this->receiver->getSIV(maxWait);
}

void GNS::UBLOX::SaveEpochToRtc() {
    if (this->GetUnixTime()) {
        // Use time.h clock_settime() for high resolution timestamp
        const timespec res = { .tv_sec = (time_t)epoch, .tv_nsec = (long)epoch_us };
        clock_settime(CLOCK_REALTIME, &res);
    }
}
