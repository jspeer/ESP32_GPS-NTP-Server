#ifndef _DISPLAY_UDPATE_TIMER_H_
#define _DISPLAY_UDPATE_TIMER_H_

#ifndef __TFT_ESPI
#include "ttgo_lcd.h"
#endif

typedef struct DisplayUpdateArgs {
    TTGO_Lcd* display;
} DisplayUpdateArgs_t;
void DisplayUpdateTimeDate(void* args);

#endif  // _DISPLAY_UDPATE_TIMER_H_
