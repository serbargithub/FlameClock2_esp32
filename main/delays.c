#include <p24Fxxxx.h>



void DelayMks(uint16_t timeMks) {
    timeMks *= 4;
    while (timeMks--) {
        asm("nop");
    }
}

void DelayMs(uint16_t timeMs) {
    while (timeMs--) {
        DelayMks(1000);
    }
}
