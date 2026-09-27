#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "log.h"
#include "log_HAL.h"

void LogMsg_lev0(char *text, ...) {
#ifdef LOGGING_LEVEL0_ENABLED
    va_list args;
    printf("##Log Lev0\n");
    va_start(args, text);
    HalLog_print_msg(text, args);
    va_end (args);
    printf("\n");
#endif // LOGGING_LEVEL0_ENABLED
}

void LogMsg_lev1(char *text, ...) {
#ifdef LOGGING_LEVEL1_ENABLED
    va_list args;
    printf("##Log Lev1\n");
    va_start(args, text);
    HalLog_print_msg(text, args);
    va_end (args);
    printf("\n");
#endif // LOGGING_LEVEL1_ENABLED
}

void DebugMsg(char *text, ...) {
#ifdef DEBUG_MESSAGES_ENABLED
    va_list args;
    printf("##Debug msg.\n");
    va_start(args, text);
    HalLog_print_msg(text, args);
    va_end (args);
    printf("\n");
#endif
}
