#include <p24Fxxxx.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "peripherals_HAL.h"
#include "io_ports_names.h"
#include "peripherals_map.h"

void HAL_ConrolReg__HighPerfomance(void) {
    __builtin_write_OSCCONH(0x03); //02 - 16mHz 03 - 64 mHz
    __builtin_write_OSCCONL(OSCCON | 0x01);
}

void HAL_ConrolReg__LowPerfomance(void) {
    __builtin_write_OSCCONH(0x05);
    __builtin_write_OSCCONL(OSCCON | 0x01);
}

void HAL_PIO__Init_IOPorts(void) {
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    PORTG = 0;
    PORTF = 0;

    TRISB = MASK_PORTB;
    TRISC = MASK_PORTC;
    TRISD = MASK_PORTD;
    TRISE = MASK_PORTE;
    TRISG = MASK_PORTG;
    TRISF = MASK_PORTF;
}

void HAL_PIO__TurnOff_IOPorts(void) {

    TRISB = MASK_OFF;
    TRISC = MASK_OFF;
    TRISD = MASK_OFF;
    TRISE = MASK_OFF;
    TRISG = MASK_OFF;
    TRISF = MASK_OFF;

}

void HAL_PIO__SetInformLed(PinValue_t value) {
    INFORM_LED_OUT = value;
}

void HAL_PIO__SetBuzzerOut(PinValue_t value) {
    BUZZER_OUT = value;
}

void HAL_PIO__BuckUp1Out(PinValue_t value) {
    BUCK_UP1_OUT = value;
}

void HAL_PIO__BuckUp2Out(PinValue_t value) {
    BUCK_UP2_OUT = value;
}

void HAL_PIO__DisplayLatch(PinValue_t value) {
    DISPLAY_LATCH = value;
}

bool HAL_PIO__GetButtonState(ButtonsName_t button) {

    switch (button) {
        case BUTTON1:
            return BUTTON1_IN;
        case BUTTON2:
            return BUTTON2_IN;
        case BUTTON3:
            return BUTTON3_IN;
        case BUTTON4:
            return BUTTON4_IN;
        default:
            return false;
    }
}

bool HAL_ADC__GetPowerState() {
    AD1CON1bits.SAMP = 1;
    while (!AD1CON1bits.DONE) {
    };
    return (ADC1BUF0 > 800);
}

void HAL_ADC__InitADC(void) {
    AD1PCFG = 0xEFFF;
    AD1CON1 = 0x00E0; // turn on the ADC
    AD1CHS = 0x000C;
    AD1CSSL = 0x0000;
    AD1CON3 = 0x1F02;
    AD1CON2 = 0x0000;
    AD1CON1bits.ADON = 1;

}

void HAL_MAP__GeneralPeripheralsMapping(void) {
    PeripheralsMap__GeneralMapping();
}
