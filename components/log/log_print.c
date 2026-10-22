
#include <stdio.h>
#include <stdarg.h>
#include "log_print.h"

void log_print_msg(char *text, va_list args ) {
    vprintf(text, args);
}


