#include <p24Fxxxx.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "delays.h"
#include "definitions.h"
#include "Peripferals/peripherals_HAL.h"
#include "Peripferals/uart_HAL.h"
#include "Peripferals/spi_HAL.h"
#include "interrupts.h"
#include "images/screens_static.h"
#include "fonts/fonts.h"
#include "display_utils.h"


static DisplayFrame_t g_DebugFrame;

void SendLine(unsigned char countV);
//****************************************************************************

void debug_task(void) {

    uint8_t line = 0;
    while (1) {

        HAL_PIO__SetInformLed(PIN_ON);
        DelayMs(100);
        HAL_PIO__SetInformLed(PIN_OFF);
        DelayMs(100);

        char rByte = getch();
        if (rByte == '0') {
            line = 0;
            memcpy(g_DebugFrame.data, BlankScreen1, sizeof (BlankScreen1));
            Interrupt__ShowFrame(&g_DebugFrame);
        }
        /*
        if (rByte == '2') {
            line = 2;
            memcpy(g_DebugFrame.data, BlankScreen2, sizeof (BlankScreen2));
            Font_SetCurrentFont(FONT_M12_SIZE40);
            static uint8_t X = 0;
            Screen_PutSymbol(&g_DebugFrame, X, 30, '5');
            Screen_PutNextSymbol(&g_DebugFrame, '4');
            Screen_PutNextSymbol(&g_DebugFrame, '3');
            Interrupt__ShowFrame(&g_DebugFrame);
            X++;
        }
         */ 
        if (rByte == '3') {
            line = 3;
            memcpy(g_DebugFrame.data, Pepsi, sizeof (Pepsi));
            while (Interrupt__IsFrameEnd() == 0) {
            };
            Interrupt__ShowFrame(&g_DebugFrame);
        }

        if (rByte == '4') {
            line = 4;
            memcpy(g_DebugFrame.data, ArtsStrade, sizeof (ArtsStrade));
            Interrupt__ShowFrame(&g_DebugFrame);
        }
        if (rByte == '5') {
            line = 5;
            memcpy(g_DebugFrame.data, Triden, sizeof (Triden));
            Interrupt__ShowFrame(&g_DebugFrame);
        }
        if (rByte == '6') {
            line = 6;
            memcpy(g_DebugFrame.data, FlameAnimation, sizeof (FlameAnimation) / 5);
            Interrupt__ShowFrame(&g_DebugFrame);
        }
        printf("\r\nLine: %i", line);
    }
}



const uint8_t line1[13] = {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
const uint8_t line2[13] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};

void SendLine(uint8_t countV) {

    uint8_t countH;
    for (countH = 0; countH < HORIZONTAL_BYTES_MAX; countH++) {
        if (countV & 0x01) {
            HAL_SPI__SendByte(line1[countH]);
        } else {
            HAL_SPI__SendByte(line2[countH]);
        }
    }
    HAL_SPI__SendByte(countV);
    HAL_PIO__DisplayLatch(PIN_ON);
    HAL_PIO__DisplayLatch(PIN_OFF);

}


//******************************************************************************

//  end of file
//******************************************************************************

