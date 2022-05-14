#include "ublox_m9n_i2c_module.h"

// Set up i2c wires
UBLOX_M9N::UBLOX_M9N() {
    Wire.setPins(UBLOX_I2C_SDA_GPIO_PIN, UBLOX_I2C_SCL_GPIO_PIN);
    Wire.begin();
}

// Initialize GNSS unit
bool UBLOX_M9N::init() {
    if (!gnss_is_initialized) {
        if (receiver->begin() == true) {
            receiver->setI2COutput(COM_TYPE_UBX);
            receiver->setMeasurementRate(UBLOX_RECEIVER_MEASUREMENT_RATE);
            receiver->setNavigationFrequency(UBLOX_RECEIVER_NAVIGATION_RATE);
            gnss_is_initialized = true;
        }
    }
    return gnss_is_initialized;
}

// Returns unix epoch seconds
uint32_t UBLOX_M9N::getUnixEpoch() {
    return receiver->getUnixEpoch();
}

// Sets epoch and epoch_us
bool UBLOX_M9N::getUnixTime() {
    epoch = receiver->getUnixEpoch(epoch_us);           // Call receiver->getUnixEpoch(), which in turn should call receiver->getPVT() if the data is stale
    epoch_ns = receiver->packetUBXNAVPVT->data.nano;    // Grab the nano seconds from the data struct since it's fresh and doesn't require a new call to receiver->getPVT()
    return (epoch) ? true : false;
}

// Returns bool if time and date are both valid
bool UBLOX_M9N::dateAndTimeValid() {
    return (receiver->getTimeValid() && receiver->getDateValid());
}

// Returns the number of Satellites in View
uint8_t UBLOX_M9N::getSIV(uint16_t maxWait) {
    return receiver->getSIV(maxWait);
}

void UBLOX_M9N::saveEpochToRtc() {
    if (getUnixTime()) {
        // Use time.h clock_settime() for high resolution timestamp
        const timespec res = { .tv_sec = (time_t)epoch, .tv_nsec = (long)epoch_us };
        clock_settime(CLOCK_REALTIME, &res);
    }
}
