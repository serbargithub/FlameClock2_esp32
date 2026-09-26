#include <p24Fxxxx.h>
#include "screens.h"
#include "screen_flame_clock.h"
#include "screen_analog_clock.h"


typedef void (*CURRENT_SCREEN_DRAW)(DisplayFrame_t* displayFrame, RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode);

CURRENT_SCREEN_DRAW g_CurrentScreenDraw_CallBack;
AdjustingList_t* g_pCurrentAdjustScreenList;

void Screens__SetCurrentScreen(ScreenNames_t screenName) {

    switch (screenName) {
        case SCREEN__FLAME_CLOCK_ONE:
            g_CurrentScreenDraw_CallBack = ScreenDraw__FlameClockOne;
            g_pCurrentAdjustScreenList = GetAdjustList__FlameClockOne();
            break;
        case SCREEN__ANALOG_CLOCK_ONE:
            ScreenInit__AnalogClockOne();
            g_CurrentScreenDraw_CallBack = ScreenDraw__AnalogClockOne;
            g_pCurrentAdjustScreenList = GetAdjustList__AnalogClockOne();
            break;
        default:
            break;
    }

}

void Screens__DrawCurrentScreen(DisplayFrame_t* displayFrame, RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode) {

    if (g_CurrentScreenDraw_CallBack != 0) {
        (*g_CurrentScreenDraw_CallBack)(displayFrame, dataTime, adjustMode);
    }
}

AdjustingList_t* Screens__GetScreenAdjustList() {
    return g_pCurrentAdjustScreenList;
}

AdjustingMode_t Screens__GetNextAdjustMode(AdjustingMode_t currentMode) {
    
    for (int16_t i = 0; i < (g_pCurrentAdjustScreenList->sizeOfList - 1); i++) {
        if (currentMode == *(g_pCurrentAdjustScreenList->dataList + i)) {
            return *(g_pCurrentAdjustScreenList->dataList + i + 1);
        }
    }
    return AJUST_MODE__OFF;
}