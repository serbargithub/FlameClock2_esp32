#include <p24Fxxxx.h>
#include <stddef.h>
#include <stdint.h>

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

void HAL_SPI__SendBuffer(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        HAL_SPI__SendByte(data[i]);
    }
}

uint8_t HAL_SPI__GetByte(void) {
    return HAL_SPI__SendByte(0);
}

