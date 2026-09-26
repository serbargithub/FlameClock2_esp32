#ifndef _SCREENS_H_
#define _SCREENS_H_

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "../definitions.h"

typedef enum {
	SCREEN__FLAME_CLOCK_ONE,
    SCREEN__ANALOG_CLOCK_ONE,
} ScreenNames_t;

void Screens__SetCurrentScreen(ScreenNames_t screenName);
void Screens__DrawCurrentScreen(DisplayFrame_t* displayFrame, RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode);
AdjustingList_t* Screens__GetScreenAdjustList();
AdjustingMode_t Screens__GetNextAdjustMode(AdjustingMode_t currentMode);

#ifdef	__cplusplus
}
#endif

#endif //_SCREENS_H_
