#ifndef _SCREEN_ANALOG_CLOCK_H_
#define _SCREEN_ANALOG_CLOCK_H_

#ifdef	__cplusplus
extern "C" {
#endif
#include "../definitions.h" 

AdjustingList_t* GetAdjustList__AnalogClockOne();    
void ScreenInit__AnalogClockOne();
void ScreenDraw__AnalogClockOne(DisplayFrame_t* displayFrame, RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode);


#ifdef	__cplusplus
}
#endif

#endif //_SCREEN_ANALOG_CLOCK_H_
