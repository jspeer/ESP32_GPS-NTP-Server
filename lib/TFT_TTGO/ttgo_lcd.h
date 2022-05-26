#pragma once

#ifndef GNS_LIB_TFT_TTGO_TTGO_H_
#define GNS_LIB_TFT_TTGO_TTGO_H_

// Include TFT library
#include <TFT_eSPI.h>                              // display driver - install within IDE
#define TFT_BACKLIGHT_GPIO 4

#include <WiFi.h>

#ifndef GNS_LIB_UBLOX_GPS_UBLOX_M9N_I2C_H_
#include "ublox_m9n_i2c.h"
#endif

#ifndef TFT_DISPOFF
#define TFT_DISPOFF 0x28
#endif

#ifndef TFT_SLPIN
#define TFT_SLPIN   0x10
#endif

#define ST7735_INVOFF 0x20
#define ST7735_INVON 0x21

namespace GNS {
    class TTGO {
    // Public structs
    public:
        typedef struct Display_Update_Args {
            GNS::UBLOX* gps;
            GNS::TTGO* display;
        } display_update_args_t;

    // Default methods
    public:
        TTGO()  = default;
        ~TTGO() = default;

    // Public members
    public:

    // Private members
    private:
        bool tft_is_initialized = false;
        // Sizes
        int screenX = 240;
        int screenY = 135;
        int radiusSize = 4;
        int borderWidth = 2;

        TFT_eSPI *display = new TFT_eSPI(screenY, screenX);

        // Colors
        int bgColor = TFT_BLACK;
        int borderColor = TFT_WHITE;
        int fillColor = TFT_BLUE;
        int fontColor = TFT_YELLOW;
        int fontTimeFg = TFT_CYAN;
        int fontColorBars = TFT_YELLOW;
        int fontColorOk = TFT_GREEN;
        int fontColorWarn = TFT_YELLOW;
        int fontColorError = TFT_RED;

        // Font sizes
        int fontSizeNormal = 2;
        int fontSizeLarge = 4;

        // Date members
        int dayOfTheYear = -1;

    // Public methods
    public:
        void Init(int rotation = 0);
        void Sleep();
        void Wake();
        void ClearScreen();
        void DrawBase(char const* title, char const* version);
        void WriteIPAddr(String* ipaddr);

    // Private methods
    private:
        void DrawTitlebar(char const* title);
        void DrawFooterBar();
        void WriteVersion(char const* version);

    // Drawing Icons
    public:
        void DrawWifiIcon(bool connected);
        void DrawSyncInProgressIcon();
        void DrawSyncIcon(int level);
        void DisplayTime(tm* timeinfo);
        void DrawNoSyncIcon();
        static void DisplayUpdateTimeDate(void* args);
    private:
        static tm GetTimeinfo();
    };
}

#endif  // GNS_LIB_TFT_TTGO_TTGO_H_
