#include <p24Fxxxx.h>
#include <string.h>
#include "screen_analog_clock.h"
#include "../adjusting.h"
#include "../images/screens_static.h"
#include "../display_utils.h"
#include "../fonts/fonts.h"
#include "../graphics/draw.h"

void AdjustAnalogClockOne(DisplayFrame_t* displayFrame, RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode);
// structure of drawing oblect : X position, Y position, color(0 or 1)
static const int arrow1[7][3] = {
    {0, 20, 0},
    {0, -33, 0},
    {0, 20, 0}, 
    {-1, 20, 1},
    {0, -33, 1},
    {1, 20, 1},
    {-1, 20, 1}
};
static const int arrow2[15][3] = {
    {0, -30, 0},
    {-2, 10, 0}, 
    {-1, 10, 0}, 
    {0, -30, 0},
    {0,  10, 0}, 
    {1, 10, 0}, 
    {0, -30, 0}, 
    {2, 10, 0}, 
    {-2, 10, 0}, 
    {-2, 10, 1},
    {-2, -5, 1},
    {0, -30, 1},
    {2, -5, 1},
    {2, 10, 1},
    {-2, 10, 1}
};
static const int arrow3[15][3] = {
    {0, -20, 0},
    {-2, 10, 0}, 
    {-1, 10, 0}, 
    {0, -20, 0}, 
    {0, 10, 0},
    {1, 10, 0}, 
    {0, -20, 0}, 
    {2, 10, 0},
    {-2, 10, 0},
    {-2, 10, 1},
    {-2, -5, 1},
    {0, -20, 1},
    {2, -5, 1},
    {2, 10, 1},
    {-2, 10, 1}
};

DrawObj_t ArrowSeconds;
DrawObj_t ArrowMinutes;
DrawObj_t ArrowHours;

void ScreenInit__AnalogClockOne() {

    ArrowSeconds.data = (int*) arrow1;
    ArrowSeconds.size = sizeof (arrow1) / sizeof (int);
    ArrowMinutes.data = (int*) arrow2;
    ArrowMinutes.size = sizeof (arrow2) / sizeof (int);
    ArrowHours.data = (int*) arrow3;
    ArrowHours.size = sizeof (arrow3) / sizeof (int);

}
int second;
int min;
int hour;

void ScreenDraw__AnalogClockOne(DisplayFrame_t* displayFrame, RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode) {
    static int16_t internalTimer;
    internalTimer++;
    Display_SetImage(displayFrame, (const uint8_t*)AnalogClock, sizeof (AnalogClock));
    
    if ((internalTimer & 0x0001)&& (adjustMode > AJUST_MODE__OFF)) {
        Adjusting__ShowAdjustingParam(displayFrame, dataTime, adjustMode);
    }
    Draw__Object(displayFrame, ArrowHours, 51, 50, dataTime->hour * 30 + dataTime->minute/2 );
    Draw__Object(displayFrame, ArrowMinutes, 51, 50, dataTime->minute * 6);
    Draw__Object(displayFrame, ArrowSeconds, 51, 50, dataTime->second * 6);
}


static const int16_t  AdjustList[] ={AJUST_MODE__OFF, AJUST_MODE__HOURS, AJUST_MODE__MINUTES, AJUST_MODE__SECONDS};
static AdjustingList_t AnalogClockOneAdjustList;

AdjustingList_t* GetAdjustList__AnalogClockOne(){
    AnalogClockOneAdjustList.dataList = AdjustList;
    AnalogClockOneAdjustList.sizeOfList = sizeof(AdjustList) / sizeof(int16_t);
    return &AnalogClockOneAdjustList;
}