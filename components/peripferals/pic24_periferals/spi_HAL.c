#include <p24Fxxxx.h>

//-----------------------------------------

void HAL_SPI__TurnOff(void) {
    SPI1STAT = 0x0000;
}

void HAL_SPI__Init(void) {
    SPI1CON1 = 0x013F; //freq divided 3D by 16  3E by 8
    SPI1STAT = 0x8000;
}

uint8_t HAL_SPI__SendByte(uint8_t c) {
    SPI1BUF = c;
    while (!SPI1STATbits.SPIRBF);
    return SPI1BUF;
}

uint8_t HAL_SPI__GetByte(void) {
    return HAL_SPI__SendByte(0);
}

