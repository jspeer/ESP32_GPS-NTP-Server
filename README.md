
    Title:       ESP32 GPS Based NTP Server
    Author:      Jeremy Speer, jeremy.a.speer@thecodeninja.dev
    Date:        May 10, 2022
    Hardware:    Lilygo TTGO ESP32 T-Display,
                 u-Blox GNSS Breakout
    Software:    VSCode, PlatformIO
                 Using bleeding edge of platform-espressif32 and framework-arduinoespressif32
    Libraries:   TFT_eSPI, SparkFun u-blox GNSS

Description: An ESP32 based NTP server with time synchronized by GPS

    uBlox M9N is connected via I2C
    See /data/config.json (default is pins 21/22).

    To replace uBlox with another sensor, replace lib/UBLOX_GPS (see current implementation)
    and update include/main.h to reflect new constructor.

    IoT_16x16_icon_set_oled_2_color from engsta.net (no copyright noted)

Config:      Configuration is stored in /data/config.json

    After first build and after every change of config.json, run "Build Filesystem
    Image" then run "Upload Filesystem Image".

    Hostname is for mDNS, so the top level domain is automatically set to .local
    Example: if hostname is set to ESP32-NTP, mDNS hostname is ESP32-NTP.local

Legal

    Released under CC0 1.0 Universal
    https://creativecommons.org/publicdomain/zero/1.0/legalcode
