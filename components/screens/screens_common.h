#ifndef _SCREENS_COMMON_H_
#define _SCREENS_COMMON_H_

#include "definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

void ScreensCommon__ShowAdjustingParam(DisplayFrame_t* displayFrame, RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode);

#ifdef __cplusplus
}
#endif

#endif //_SCREENS_COMMON_H_
