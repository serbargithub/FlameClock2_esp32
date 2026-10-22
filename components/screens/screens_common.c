#include "screens_common.h"
#include "graphics/display_utils.h"
#include "fonts/fonts.h"

void ScreensCommon__ShowAdjustingParam(DisplayFrame_t* displayFrame, RTCC_DATETIME* dataTime, AdjustingMode_t adjustMode) {

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

