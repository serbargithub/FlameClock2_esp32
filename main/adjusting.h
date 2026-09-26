#ifndef _ADJUSTING_H_
#define _ADJUSTING_H_

#ifdef	__cplusplus
extern "C" {
#endif

void Adjusting__AdjustParametr(RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode, int16_t direction);
void Adjusting__ShowAdjustingParam(DisplayFrame_t* displayFrame, RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode);

#ifdef	__cplusplus
}
#endif

#endif //_ADJUSTING_H_


