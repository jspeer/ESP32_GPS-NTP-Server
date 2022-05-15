#pragma once

// Include TFT library
#ifndef __TFT_ESPI
#include <TFT_eSPI.h>                              // display driver - install within IDE
#define TFT_BACKLIGHT_GPIO 4
#define __TFT_ESPI
#endif

#ifndef TFT_DISPOFF
#define TFT_DISPOFF 0x28
#endif

#ifndef TFT_SLPIN
#define TFT_SLPIN   0x10
#endif

#define ST7735_INVOFF 0x20
#define ST7735_INVON 0x21

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;

class TTGO_Lcd {
// Default methods
public:
    TTGO_Lcd();
    ~TTGO_Lcd() = default;

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
    int fontColor = TFT_WHITE;
    int fontTimeFg = TFT_CYAN;
    int fontColorInverse = TFT_BLACK;
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
    void init(int rotation = 0);
    void sleep();
    void wake();
    void clearScreen();
    void DrawBase(char* title, char* version);
    void WriteIPAddr(String* ipaddr);

// Private methods
private:
    void DrawTitlebar(char* title);
    void DrawFooterBar();
    void WriteVersion(char* version);

// Drawing Icons
public:
    void DrawWifiIcon(bool connected);
    void DrawSyncInProgressIcon();
    void DrawSyncIcon(int level);
    void DisplayTime(tm* timeinfo);
    void DrawNoSyncIcon();

private:
};
