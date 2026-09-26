#include <p24Fxxxx.h>
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "delays.h"
#include "definitions.h"
#include "interrupts.h"
#include "adjusting.h"
#include "sound.h"
#include "Peripferals/peripherals_HAL.h"
#include "Peripferals/uart_HAL.h"
#include "Peripferals/rtcc.h"
#include "screens/screens.h"


#define SCREEN_NUMBER_MAX 2

static RTCC_DATETIME g_DataTime;
static DisplayFrame_t g_ClockFrame;
static uint16_t g_CurrentScreenNumber = 0;
static AdjustingMode_t g_AdjustMode;
static uint16_t g_TimerAdjustMode;

void HandleAjustTimer();
void CheckButtons(void);
bool IsCurrentTimeValid(void);

//****************************************************************************

void Clock_Init(void) {

    RTCC_Start();
    RTCC_TimeGet(&g_DataTime);

    if (IsCurrentTimeValid() == false) {
        RTCC_BuildTimeGet(&g_DataTime);
        RTCC_Initialize(&g_DataTime);
    }
    g_AdjustMode = AJUST_MODE__OFF;
    g_TimerAdjustMode = 0;
    g_CurrentScreenNumber = 0;
    Screens__SetCurrentScreen(SCREEN__FLAME_CLOCK_ONE);
    //Screens__SetCurrentScreen(SCREEN__ANALOG_CLOCK_ONE);
    Screens__DrawCurrentScreen(&g_ClockFrame, &g_DataTime, g_AdjustMode);
}

void Clock_Task(void) {

    static uint8_t previousSecond = 0;

    RTCC_TimeGet(&g_DataTime);

    if (previousSecond != g_DataTime.second) {
        previousSecond = g_DataTime.second;
        Interrupt__ShowFrame(&g_ClockFrame);
        HandleAjustTimer();
        CheckButtons();
        Screens__DrawCurrentScreen(&g_ClockFrame, &g_DataTime, g_AdjustMode);
    }
}

void SetNextScreen(void) {

    g_CurrentScreenNumber++;
    if (g_CurrentScreenNumber >= SCREEN_NUMBER_MAX) {
        g_CurrentScreenNumber = 0;
    }
    switch (g_CurrentScreenNumber) {
        case 0:
            Screens__SetCurrentScreen(SCREEN__FLAME_CLOCK_ONE);
            break;
        case 1:
            Screens__SetCurrentScreen(SCREEN__ANALOG_CLOCK_ONE);
            break;
        default:
            break;
    }
}

void HandleAjustTimer() {

    if (g_TimerAdjustMode > 0) {
        g_TimerAdjustMode--;
    } else {
        g_AdjustMode = AJUST_MODE__OFF;
    }
}

bool CheckButtonState(ButtonsName_t button) {

    if (!HAL_PIO__GetButtonState(button)) {
        DelayMs(100);
        if (!HAL_PIO__GetButtonState(button)) {
            return 0;
        }
    }
    return 1;
}

void CheckButtons(void) {

    if (!CheckButtonState(BUTTON1)) {
        Sound__Beep(1200, 200);
        SetNextScreen();
    }
    if (!CheckButtonState(BUTTON2)) {
        Sound__Beep(1000, 50);
        g_TimerAdjustMode = 10;
        g_AdjustMode = Screens__GetNextAdjustMode(g_AdjustMode);
        if (g_AdjustMode >= AJUST_MODE__THE_LAST) {
            g_AdjustMode = AJUST_MODE__OFF;
        }
    }
    if (g_AdjustMode > AJUST_MODE__OFF) {
        if (!CheckButtonState(BUTTON3)) {
            Sound__Beep(1000, 50);
            g_TimerAdjustMode = 10;
            Adjusting__AdjustParametr(&g_DataTime, g_AdjustMode, 1);
        }
        if (!CheckButtonState(BUTTON4)) {
            Sound__Beep(1000, 50);
            g_TimerAdjustMode = 10;
            Adjusting__AdjustParametr(&g_DataTime, g_AdjustMode, -1);
        }
    }
}

bool IsCurrentTimeValid(void) {

    RTCC_DATETIME buildTime;
    RTCC_BuildTimeGet(&buildTime);

    if ((g_DataTime.year < buildTime.year) || (g_DataTime.year > 99)) {
        return false;
    }
    if ((g_DataTime.year == buildTime.year) &&
            (g_DataTime.month < buildTime.month)) {
        return false;
    }
    if ((g_DataTime.year == buildTime.year) &&
            (g_DataTime.month == buildTime.month) &&
            (g_DataTime.day < buildTime.day)) {
        return false;
    }
    if ((g_DataTime.year == buildTime.year) &&
            (g_DataTime.month == buildTime.month) &&
            (g_DataTime.day == buildTime.day) &&
            (g_DataTime.hour < buildTime.hour)) {
        return false;
    }
    if ((g_DataTime.year == buildTime.year) &&
            (g_DataTime.month == buildTime.month) &&
            (g_DataTime.day == buildTime.day) &&
            (g_DataTime.hour == buildTime.hour) &&
            (g_DataTime.minute < buildTime.minute)) {
        return false;
    }
    return true;
}
//******************************************************************************

//  end of file
//******************************************************************************

