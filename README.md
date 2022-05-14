 Title:       ESP32 GPS Based NTP Server
 Author:      Jeremy Speer, jeremy.a.speer@thecodeninja.dev
 Date:        May 10, 2022
 Hardware:    Lilygo TTGO ESP32 T-Display,
              u-Blox NEO-M9N (Sparkfun breakout board)
 Software:    VSCode, PlatformIO
              Using bleeding edge of platform-espressif32 and framework-arduinoespressif32
 Libraries:   TFT_eSPI, SparkFun u-blox GNSS

 Description: An ESP32 based NTP server with time synchronized by GPS

              M9N is connected via I2C via pins 13 (SDA) and 15 (SCL).
              See /data/config.json (default is pins 21/22).

              IoT_16x16_icon_set_oled_2_color from engsta.net (no copyright noted)

 Config:      Configuration is stored in /data/config.json
 
              After first build and after every change of config.json, run "Build Filesystem
              Image" then run "Upload Filesystem Image".

 Legal:       Released under CC0 1.0 Universal
              https://creativecommons.org/publicdomain/zero/1.0/legalcode

 TODO: Remove debugging output to serial.....eventually.
       Improve comments throughout to ensure reader understands what I wrote... including
       future me.
