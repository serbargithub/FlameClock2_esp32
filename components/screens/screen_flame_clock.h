#ifndef _SCREEN_FLAME_CLOCK_H_
#define _SCREEN_FLAME_CLOCK_H_

#ifdef	__cplusplus
extern "C" {
#endif
#include "../definitions.h" 
    
AdjustingList_t* GetAdjustList__FlameClockOne();
void ScreenDraw__FlameClockOne(DisplayFrame_t* displayFrame, RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode);


#ifdef	__cplusplus
}
#endif

#endif //_SCREEN_FLAME_CLOCK_H_
