#ifndef GNS_LIB_TFT_TTGO_TTGO_H_
#include "ttgo_lcd.h"
#endif

#include "time.h"
#include "../../include/iot_iconset_16x16/iot_iconset_16x16.h"

void GNS::TTGO::Init(int rotation) {
    this->display->init();
    this->display->writecommand(ST7735_INVON);  // fix for inverted colors
    this->display->setRotation(rotation);
    this->ClearScreen();

    pinMode(TFT_BACKLIGHT_GPIO, OUTPUT);
    this->tft_is_initialized = true;
}

void GNS::TTGO::Sleep() {
    if (!this->tft_is_initialized) init();        // Called out of order, init the screen

    digitalWrite(TFT_BACKLIGHT_GPIO, LOW);  // Turn off the backlight
    this->display->writecommand(ST7789_DISPOFF);      // Turn off the display
    this->display->writecommand(ST7789_SLPIN);        // Enable sleep mode
}

void GNS::TTGO::Wake() {
    if (!this->tft_is_initialized) init();        // Called out of order, init the screen

    this->display->writecommand(ST7789_SLPOUT);       // Disable sleep mode
    this->display->writecommand(ST7789_DISPON);       // Turn on the display
    digitalWrite(TFT_BACKLIGHT_GPIO, HIGH); // Turn on the backlight
}

void GNS::TTGO::ClearScreen() {
    this->display->fillScreen(this->bgColor);
}

void GNS::TTGO::DrawBase(char const* title, char const* version) {
    int sizeX = this->screenX - (this->borderWidth*2);
    int sizeY = this->screenY - (this->borderWidth*2);
    this->display->fillScreen(this->bgColor);
    this->DrawTitlebar(title);
    this->DrawFooterBar();
    this->display->drawRoundRect(this->borderWidth, this->borderWidth, sizeX, sizeY, this->radiusSize, this->borderColor);

    this->display->setTextColor(this->fontTimeFg, this->bgColor);
    this->display->drawString("00:00:00", 12, 30, 6);

    this->display->setTextColor(this->fontColorOk, this->bgColor);
    this->display->drawString(_tzname[_daylight], 206, 29, 2);  // these are from time.h

    this->WriteVersion(version);
}

void GNS::TTGO::DrawTitlebar(char const* title) {
    int sizeX = this->screenX - (this->borderWidth*2);
    this->display->fillRoundRect(this->borderWidth, this->borderWidth, sizeX, 20, this->radiusSize, this->fillColor);
    this->display->setTextColor(this->fontColorWarn, this->fillColor);
    this->display->drawCentreString(title, this->screenX / 2, 5, this->fontSizeNormal);
}

void GNS::TTGO::DrawFooterBar() {
    int sizeX = this->screenX - (this->borderWidth*2);
    this->display->fillRoundRect(this->borderWidth, this->screenY - this->borderWidth - 20, sizeX, 20, this->radiusSize, this->fillColor);
}

void GNS::TTGO::DrawWifiIcon(bool connected) {
    int posX = this->screenX - 22;
    int posY;
    int sizeX = 16;
    int sizeY = 16;
    if (connected) {
        posY = 6;
        this->display->drawRect(posX, posY-2, sizeX, sizeY, this->fillColor);
        this->display->drawBitmap(posX, posY, wifi1_icon16x16, sizeX, sizeY, this->fontColorOk, this->fillColor);
    } else {
        posY = 4;
        this->display->drawRect(posX, posY+2, sizeX, sizeY, fillColor);
        this->display->drawBitmap(posX, posY, nocon_icon16x16, sizeX, sizeY, this->fontColorError, this->fillColor);
        String ipaddr = "No IP Address";
        this->WriteIPAddr(&ipaddr);
    }
}

void GNS::TTGO::DrawNoSyncIcon() {
    int posX = 6;
    int posY = 4;
    int sizeX = 16;
    int sizeY = 16;
    this->display->drawRect(posX, posY+2, sizeX, sizeY, this->fillColor);
    this->display->drawBitmap(posX, posY, nocon_icon16x16, sizeX, sizeY, this->fontColorError, this->fillColor);
}

void GNS::TTGO::DrawSyncInProgressIcon() {
    int posX = 6;
    int posY = 4;
    int sizeX = 16;
    int sizeY = 16;
    this->display->drawRect(posX, posY+2, sizeX, sizeY, this->fillColor);
    this->display->drawBitmap(posX, posY, warning_icon16x16, sizeX, sizeY, this->fontColorWarn, this->fillColor);
}

void GNS::TTGO::DrawSyncIcon(int level) {
    int posX = 6;
    int posY = 4;
    int sizeX = 16;
    int sizeY = 16;
    this->display->drawRect(posX, posY+2, sizeX, sizeY, this->fillColor);
    switch (level) {
        case 0:
            this->DrawSyncInProgressIcon();
            break;
        case 1:
            this->display->drawBitmap(posX, posY, signal1_icon16x16, sizeX, sizeY, this->fontColorOk, this->fillColor);;
            break;
        case 2:
            this->display->drawBitmap(posX, posY, signal2_icon16x16, sizeX, sizeY, this->fontColorOk, this->fillColor);;
            break;
        case 3:
            this->display->drawBitmap(posX, posY, signal3_icon16x16, sizeX, sizeY, this->fontColorWarn, this->fillColor);;
            break;
        case 4:
        default:
            this->display->drawBitmap(posX, posY, signal4_icon16x16, sizeX, sizeY, this->fontColorWarn, this->fillColor);;
            break;
    }
}

void GNS::TTGO::WriteIPAddr(String* ipaddr) {
    int zeroY = this->screenY - this->borderWidth - 20;
    int zeroX = this->screenX - this->borderWidth;
    int posX = zeroX - 7;
    int posY = zeroY + 1;

    this->display->setTextColor(this->fontColorWarn, this->fillColor);
    this->display->drawRightString("               ", posX, posY, this->fontSizeNormal);
    this->display->drawRightString(ipaddr->c_str(), posX, posY, this->fontSizeNormal);
}

void GNS::TTGO::WriteVersion(char const* version) {
    int zeroY = this->screenY - this->borderWidth - 20;
    int zeroX = 0 + this->borderWidth;
    int posX = zeroX + 7;
    int posY = zeroY + 1;
    char versionString[100];
    strcpy(versionString, "FW: ");
    strcat(versionString, version);
    this->display->setTextColor(this->fontColorWarn, this->fillColor);
    this->display->drawString(versionString, posX, posY, this->fontSizeNormal);
}

void GNS::TTGO::DisplayTime(tm* timeinfo) {
    char buffer[80];

    strftime(buffer, 80, "%T", timeinfo);
    this->display->setTextColor(fontColorWarn, bgColor);
    this->display->drawString(buffer, 12, 30, 6);

    // If the day of the year has changed, update the date string
    if (this->dayOfTheYear != timeinfo->tm_yday) {
        this->dayOfTheYear = timeinfo->tm_yday;
        strftime(buffer, 80, "%a %b %e, %Y", timeinfo);
        this->display->setTextColor(this->fontColorWarn, this->bgColor);
        this->display->drawRect(this->borderWidth+1, 80, this->screenX - (this->borderWidth*2) - 2, this->display->fontHeight(4), this->bgColor);
        this->display->drawCentreString(buffer, ((screenX - (borderWidth*2))/2), 80, 4);
    }

    this->display->setTextColor(this->fontColorOk, this->bgColor);
    this->display->drawString(_tzname[_daylight], 206, 29, 2);  // these are from time.h
}

void GNS::TTGO::DisplayUpdateTimeDate(void* args) {
    // Cast args
    GNS::TTGO::Display_Update_Args* displayUpdateArgs = static_cast<GNS::TTGO::Display_Update_Args*>(args);

    // Update the wifi status indicator
    displayUpdateArgs->display->DrawWifiIcon(WiFi.status() == WL_CONNECTED);

    // Update the satellites in view indicator
    if (displayUpdateArgs->gps->gnss_is_initialized == false) {
        displayUpdateArgs->display->DrawNoSyncIcon();
    } else {
        displayUpdateArgs->display->DrawSyncIcon(displayUpdateArgs->gps->siv);
    }

    // Update display time
    time_t now;
    tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    displayUpdateArgs->display->DisplayTime(&timeinfo);
}
