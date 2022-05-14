#include "ttgo_lcd.h"
#include "time.h"
#include "../../include/iot_iconset_16x16/iot_iconset_16x16.h"

TTGO_Lcd::TTGO_Lcd() {
    ;; // Placeholder
}

void TTGO_Lcd::init(int rotation) {
    display->init();
    display->writecommand(ST7735_INVON);  // fix for inverted colors
    display->setRotation(rotation);
    clearScreen();

    pinMode(TFT_BACKLIGHT_GPIO, OUTPUT);
    tft_is_initialized = true;
}

void TTGO_Lcd::sleep() {
    if (!tft_is_initialized) init();        // Called out of order, init the screen

    digitalWrite(TFT_BACKLIGHT_GPIO, LOW);  // Turn off the backlight
    display->writecommand(ST7789_DISPOFF);      // Turn off the display
    display->writecommand(ST7789_SLPIN);        // Enable sleep mode
}

void TTGO_Lcd::wake() {
    if (!tft_is_initialized) init();        // Called out of order, init the screen

    display->writecommand(ST7789_SLPOUT);       // Disable sleep mode
    display->writecommand(ST7789_DISPON);       // Turn on the display
    digitalWrite(TFT_BACKLIGHT_GPIO, HIGH); // Turn on the backlight
}

void TTGO_Lcd::clearScreen() {
    display->fillScreen(TFT_BLACK);
}

void TTGO_Lcd::DrawBase(char* title, char* version) {
    int sizeX = screenX - (borderWidth*2);
    int sizeY = screenY - (borderWidth*2);
    display->fillScreen(bgColor);
    DrawTitlebar(title);
    DrawFooterBar();
    display->drawRoundRect(borderWidth, borderWidth, sizeX, sizeY, radiusSize, borderColor);

    display->setTextColor(TFT_CYAN, TFT_BLACK);
    display->drawString("00:00:00", 12, 30, 6);

    display->setTextColor(TFT_GREEN, TFT_BLACK);
    display->drawString(_tzname[_daylight], 206, 29, 2);

    WriteVersion(version);
}

void TTGO_Lcd::DrawTitlebar(char* title) {
    int sizeX = screenX - (borderWidth*2);
    display->fillRoundRect(borderWidth, borderWidth, sizeX, 20, radiusSize, fillColor);
    display->setTextColor(fontColorWarn, fillColor);
    display->drawCentreString(title, screenX / 2, 5, fontSizeNormal);
}

void TTGO_Lcd::DrawFooterBar() {
    int sizeX = screenX - (borderWidth*2);
    display->fillRoundRect(borderWidth, screenY - borderWidth - 20, sizeX, 20, radiusSize, fillColor);
}

void TTGO_Lcd::DrawWifiIcon(bool connected) {
    int posX = screenX - 22;
    int posY;
    int sizeX = 16;
    int sizeY = 16;
    if (connected) {
        posY = 6;
        display->drawRect(posX, posY-2, sizeX, sizeY, fillColor);
        display->drawBitmap(posX, posY, wifi1_icon16x16, sizeX, sizeY, TFT_GREEN, fillColor);
    } else {
        posY = 4;
        display->drawRect(posX, posY+2, sizeX, sizeY, fillColor);
        display->drawBitmap(posX, posY, nocon_icon16x16, sizeX, sizeY, TFT_RED, fillColor);
        String ipaddr = "No IP Address";
        WriteIPAddr(&ipaddr);
    }
}

void TTGO_Lcd::DrawNoSyncIcon() {
    int posX = 6;
    int posY = 4;
    int sizeX = 16;
    int sizeY = 16;
    display->drawRect(posX, posY+2, sizeX, sizeY, fillColor);
    display->drawBitmap(posX, posY, nocon_icon16x16, sizeX, sizeY, TFT_RED, fillColor);
}

void TTGO_Lcd::DrawSyncInProgressIcon() {
    int posX = 6;
    int posY = 4;
    int sizeX = 16;
    int sizeY = 16;
    display->drawRect(posX, posY+2, sizeX, sizeY, fillColor);
    display->drawBitmap(posX, posY, warning_icon16x16, sizeX, sizeY, TFT_YELLOW, fillColor);
}

void TTGO_Lcd::DrawSyncIcon(int level) {
    int posX = 6;
    int posY = 4;
    int sizeX = 16;
    int sizeY = 16;
    display->drawRect(posX, posY+2, sizeX, sizeY, fillColor);
    switch (level) {
        case 0:
            DrawSyncInProgressIcon();
            break;
        case 1:
            display->drawBitmap(posX, posY, signal1_icon16x16, sizeX, sizeY, TFT_GREEN, fillColor);;
            break;
        case 2:
            display->drawBitmap(posX, posY, signal2_icon16x16, sizeX, sizeY, TFT_GREEN, fillColor);;
            break;
        case 3:
            display->drawBitmap(posX, posY, signal3_icon16x16, sizeX, sizeY, TFT_YELLOW, fillColor);;
            break;
        case 4:
        default:
            display->drawBitmap(posX, posY, signal4_icon16x16, sizeX, sizeY, TFT_YELLOW, fillColor);;
            break;
    }
}

void TTGO_Lcd::WriteIPAddr(String* ipaddr) {
    int zeroY = screenY - borderWidth - 20;
    int zeroX = screenX - borderWidth;
    int posX = zeroX - 7;
    int posY = zeroY + 1;

    display->setTextColor(fontColorWarn, fillColor);
    display->drawRightString("               ", posX, posY, fontSizeNormal);
    display->drawRightString(ipaddr->c_str(), posX, posY, fontSizeNormal);
}

void TTGO_Lcd::WriteVersion(char* version) {
    int zeroY = screenY - borderWidth - 20;
    int zeroX = 0 + borderWidth;
    int posX = zeroX + 7;
    int posY = zeroY + 1;
    char versionString[100];
    strcpy(versionString, "FW: ");
    strcat(versionString, version);
    display->setTextColor(fontColorWarn, fillColor);
    display->drawString(versionString, posX, posY, fontSizeNormal);
}

void TTGO_Lcd::DisplayTime(tm* timeinfo) {
    char buffer[80];

    strftime(buffer, 80, "%T", timeinfo);
    display->setTextColor(TFT_CYAN, TFT_BLACK);
    display->drawString(buffer, 12, 30, 6);

    // If the day of the year has changed, update the date string
    if (dayOfTheYear != timeinfo->tm_yday) {
        dayOfTheYear = timeinfo->tm_yday;
        strftime(buffer, 80, "%a %b %e, %Y", timeinfo);
        display->setTextColor(TFT_YELLOW, TFT_BLACK);
        display->drawRect(borderWidth+1, 80, screenX - (borderWidth*2) - 2, display->fontHeight(4), TFT_BLACK);
        display->drawCentreString(buffer, ((screenX - (borderWidth*2))/2), 80, 4);
    }

    display->setTextColor(TFT_GREEN, TFT_BLACK);
    display->drawString(_tzname[_daylight], 206, 29, 2);
}
