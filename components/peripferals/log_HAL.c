
#include <stdio.h>
#include <stdarg.h>

void HalLog_print_msg(char *text, va_list args ) {
    vprintf(text, args);
}



