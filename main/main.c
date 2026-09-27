#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "delays.h"
#include "definitions.h"
#include "peripferals/peripherals_HAL.h"
#include "peripferals/uart_HAL.h"
#include "peripferals/spi_HAL.h"
#include "images/screens_static.h"
#include "interrupts.h"
#include "clock_task.h"
#include "display_utils.h"
#include "log/log.h"


// ESP32-C3 bring-up: UART console is active; other hardware uses stubs.

static DisplayFrame_t g_PreparedFrame;

void PowerSavingMode();

//****************************************************************************

void app_main(void) {
    HAL_ConrolReg__HighPerfomance();
    HAL_PIO__Init_IOPorts();
    HAL_ADC__InitADC();
    HAL_MAP__GeneralPeripheralsMapping();
    HAL_UART__SerialSetup(UART_SPEED_115200, UART_CH1);
    HAL_SPI__Init();
    Interrupt__Setup();
    Clock_Init();
    DelayMs(200);
    HAL_PIO__BuckUp1Out(PIN_ON);
    DelayMs(200);
    HAL_PIO__BuckUp2Out(PIN_ON);
    Display_SetImage(&g_PreparedFrame, (const uint8_t*)ArtsStrade, sizeof (ArtsStrade));
    Interrupt__ShowFrame(&g_PreparedFrame);
    DebugMsg("Start. USB-UART logging at 115200 baud; display and sound are inactive.");
    HAL_UART__CheckAndResetErrors(UART_CH1);
    DelayMs(1000);

    while (1) {
        DelayMs(20);
        if (HAL_ADC__GetPowerState() == 0) {
            PowerSavingMode();
            return; //full reset
        } else {
            Clock_Task();
        }
    }
}

void PowerSavingMode() {

    DebugMsg("Enter Power save mode.");
    Interrupt__DisableAll();
    HAL_UART__TurnOff(UART_CH1);
    HAL_SPI__TurnOff();
    HAL_PIO__TurnOff_IOPorts();
    HAL_ConrolReg__LowPerfomance();

    while (HAL_ADC__GetPowerState() == 0) {
        DelayMs(2);
    }
    DelayMs(2);
}


//******************************************************************************

//  end of file
//******************************************************************************

