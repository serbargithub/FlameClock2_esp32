#include <p24Fxxxx.h>
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "definitions.h"
#include "adjusting.h"
#include "display_utils.h"
#include "Peripferals/rtcc.h"
#include "fonts/fonts.h"

void Adjusting__AdjustParametr(RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode, int16_t direction) {

    switch (adjustMode) {
        case AJUST_MODE__HOURS:
            if (direction == 1) {
                if (dataTime->hour < 23) {
                    dataTime->hour++;
                    RTCC_Initialize(dataTime);
                } else {
                    dataTime->hour = 0;
                }
            } else {
                if (dataTime->hour > 0) {
                    dataTime->hour--;
                    RTCC_Initialize(dataTime);
                } else {
                    dataTime->hour = 23;
                }
            }
            break;
        case AJUST_MODE__MINUTES:
            if (direction == 1) {
                if (dataTime->minute < 59) {
                    dataTime->minute++;
                    RTCC_Initialize(dataTime);
                } else {
                    dataTime->minute = 0;
                }
                
            } else {
                if (dataTime->minute > 0) {
                    dataTime->minute--;
                    RTCC_Initialize(dataTime);
                } else {
                    dataTime->minute = 59;
                }
            }
            break;
        case AJUST_MODE__SECONDS:
            dataTime->second = 0;
            RTCC_Initialize(dataTime);
            break;
        case AJUST_MODE__WEEKS:
            if (direction == 1) {
                if (dataTime->weekday < 7) {
                    dataTime->weekday++;
                    RTCC_Initialize(dataTime);
                } else {
                    dataTime->weekday = 1;
                }
            } else {
                if (dataTime->weekday > 1) {
                    dataTime->weekday--;
                    RTCC_Initialize(dataTime);
                } else {
                    dataTime->weekday = 7;
                }
            }
            break;
        case AJUST_MODE__DAYS:
            if (direction == 1) {
                if (dataTime->day < 31) {
                    dataTime->day++;
                    RTCC_Initialize(dataTime);
                } else {
                    dataTime->day = 1;
                }
            } else {
                if (dataTime->day > 1) {
                    dataTime->day--;
                    RTCC_Initialize(dataTime);
                } else {
                    dataTime->day = 31;
                }
            }
            break;
        case AJUST_MODE__MONTHS:
            if (direction == 1) {
                if (dataTime->month < 12) {
                    dataTime->month++;
                    RTCC_Initialize(dataTime);
                } else {
                    dataTime->month = 1;
                }
            } else {
                if (dataTime->month > 1) {
                    dataTime->month--;
                    RTCC_Initialize(dataTime);
                } else {
                    dataTime->month = 12;
                }
            }
            break;
        case AJUST_MODE__YEARS:
            if (direction == 1) {
                if (dataTime->year < 2100) {
                    dataTime->year++;
                    RTCC_Initialize(dataTime);
                }
            } else {
                if (dataTime->year > 2022) {
                    dataTime->year--;
                    RTCC_Initialize(dataTime);
                }
            }
            break;
        default:
            break;
    }
}


void Adjusting__ShowAdjustingParam(DisplayFrame_t* displayFrame, RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode) {

    Font_SetCurrentFont(FONT_ANTONIO_REGULAR_SIZE16);

    switch (adjustMode) {
        case AJUST_MODE__HOURS:
            Display_Printf(displayFrame, 15, 5, "Adjust Hours!");
            break;
        case AJUST_MODE__MINUTES:
            Display_Printf(displayFrame, 15, 5, "Adjust Minutes!");
            break;
        case AJUST_MODE__SECONDS:
            Display_Printf(displayFrame, 15, 5, "Adjust Seconds!");
            break;
        case AJUST_MODE__WEEKS:
            Display_Printf(displayFrame, 15, 5, "Adjust Weeks!");
            break;
        case AJUST_MODE__DAYS:
            Display_Printf(displayFrame, 15, 5, "Adjust Days!");
            break;
        case AJUST_MODE__MONTHS:
            Display_Printf(displayFrame, 15, 5, "Adjust Months!");
            break;
        case AJUST_MODE__YEARS:
            Display_Printf(displayFrame, 15, 5, "Adjust Years!");
            break;
        default:
            Display_Printf(displayFrame, 15, 5, "Adjust Finish!");
            break;
    }
}

//******************************************************************************

//  end of file
//******************************************************************************

