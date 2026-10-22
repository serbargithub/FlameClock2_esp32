#ifndef _CLOCK_MANAGER_H_
#define _CLOCK_MANAGER_H_

#ifdef	__cplusplus
extern "C" {
#endif

void ClockManager__AdjustParametr(RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode, int16_t direction);

#ifdef	__cplusplus
}
#endif

#endif //_CLOCK_MANAGER_H_


