#include <p24Fxxxx.h>
#include <string.h>
#include "screen_flame_clock.h"
#include "../adjusting.h"
#include "../display_utils.h"
#include "../fonts/fonts.h"

void AdjustFlameClockOne(DisplayFrame_t* displayFrame, RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode);

void ScreenDraw__FlameClockOne(DisplayFrame_t* displayFrame, RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode) {

    static int16_t internalTimer;
    memset(displayFrame->data, 0xFF, sizeof (DisplayFrame_t));
    internalTimer++;
    if ((internalTimer & 0x0001)&& (adjustMode > AJUST_MODE__OFF)) {
        Adjusting__ShowAdjustingParam(displayFrame, dataTime, adjustMode);
    } else {
        Font_SetCurrentFont(FONT_ANTONIO_REGULAR_SIZE16);
        Display_Printf(displayFrame, 15, 5, "Flame clock");
    }
    Font_SetCurrentFont(FONT_M12_SIZE20);
    Display_Printf(displayFrame, 10, 35, "%02i.%02i", dataTime->hour, dataTime->minute);
    Font_SetCurrentFont(FONT_M12_SIZE16);
    Display_Printf(displayFrame, 65, 64, "%02i", dataTime->second);
    Font_SetCurrentFont(FONT_BEBAS_BOLD_SIZE16);
    Display_Printf(displayFrame, 5, 83, "%s, %02i  %s  20%02i", WeekDay[dataTime->weekday], dataTime->day, Month[dataTime->month], dataTime->year);
}

static const int16_t  AdjustList[] ={AJUST_MODE__OFF, AJUST_MODE__HOURS, AJUST_MODE__MINUTES, AJUST_MODE__SECONDS, AJUST_MODE__WEEKS, AJUST_MODE__DAYS, AJUST_MODE__MONTHS, AJUST_MODE__YEARS};
static AdjustingList_t FlameClockAdjustList;

AdjustingList_t* GetAdjustList__FlameClockOne() {
    FlameClockAdjustList.dataList = AdjustList;
    FlameClockAdjustList.sizeOfList = sizeof(AdjustList) / sizeof(int16_t);
    return &FlameClockAdjustList;
}