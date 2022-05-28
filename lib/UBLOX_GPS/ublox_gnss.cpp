#ifndef GNS_LIB_UBLOX_GPS_UBLOX_GNSS_H_
#include "ublox_gnss.h"
#endif

#include <time.h>

// Set up i2c wires
GNS::UBLOX_GNSS::UBLOX_GNSS(int sda, int scl) {
    ESP_LOGI("GPS", "Setting up pins and starting i2c.");
    Wire.setPins(sda, scl);
    Wire.begin();
}

// Initialize GNSS unit
bool GNS::UBLOX_GNSS::Init() {
    if (!this->gnss_is_initialized) {
        if (this->receiver->begin() == true) {
            ESP_LOGI("GPS", "GPS initialized, setting parameters");
            this->receiver->setI2COutput(COM_TYPE_UBX);
            this->receiver->setMeasurementRate(UBLOX_RECEIVER_MEASUREMENT_RATE);
            this->receiver->setNavigationFrequency(UBLOX_RECEIVER_NAVIGATION_RATE);
            this->gnss_is_initialized = true;
        }
    }
    return this->gnss_is_initialized;
}

// Obtain and save the epoch time to ESP RTC from the receiver
void GNS::UBLOX_GNSS::GetEpoch() {
    // Get the packet from the receiver
    ESP_LOGI("GPS", "Obtaining UBXNAVPVT packet from receiver");
    this->epoch = this->receiver->getUnixEpoch(this->epoch_us);           // Call receiver->getUnixEpoch(), which in turn should call receiver->getPVT() if the data is stale
    this->epoch_ns = this->receiver->packetUBXNAVPVT->data.nano;    // Grab the nano seconds from the current receiver packet
    this->siv = this->receiver->packetUBXNAVPVT->data.numSV;  // Get the SIV from the current receiver packet
}
