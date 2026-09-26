#include <p24Fxxxx.h>
#include "interrupts.h"

void Sound__KeyBeep() {
    Interrupt__PlaySound(1000, 100);
}

void Sound__Beep(uint16_t freq_Hz, uint16_t time_ms) {
    Interrupt__PlaySound(freq_Hz, time_ms);
}