#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "log.h"
#include "Peripferals/log_HAL.h"

void LogMsg_lev0(char *text, ...) {
#ifdef LOGGING_LEVEL0_ENABLED
    va_list args;
    HalLog_print_msg("##Log Lev0\n\r", args);
    va_start(args, text);
    HalLog_print_msg(text, args);
    va_end (args);
    HalLog_print_msg("\n\r", args);
#endif // LOGGING_LEVEL0_ENABLED
}

void LogMsg_lev1(char *text, ...) {
#ifdef LOGGING_LEVEL1_ENABLED
    va_list args;
    HalLog_print_msg("##Log Lev1\n\r", args);
    va_start(args, text);
    HalLog_print_msg(text, args);
    va_end (args);
    HalLog_print_msg("\n\r", args);
#endif // LOGGING_LEVEL1_ENABLED
}

void DebugMsg(char *text, ...) {
#ifdef DEBUG_MESSAGES_ENABLED
    va_list args;
    HalLog_print_msg("##Debug msg.\n\r", args);
    va_start(args, text);
    HalLog_print_msg(text, args);
    va_end (args);
    HalLog_print_msg("\n\r", args);
#endif
}


