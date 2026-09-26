#include <p24Fxxxx.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "definitions.h" 
#include "interrupts.h"
#include "Peripferals/peripherals_HAL.h"
#include "Peripferals/spi_HAL.h"

#define _ISR_PSV __attribute__((interrupt, auto_psv))
#define _ISR_NO_PSV __attribute__(interrupt, no_auto_psv))

static DisplayFrame_t g_DisplayFrame;
static bool g_EndOfFrameFlag;
static uint16_t g_SoundTimeCounter;

//================================================

void _ISR_PSV _T1Interrupt(void) {

    static uint8_t countV;
    for (int8_t countH = (HORIZONTAL_BYTES_MAX - 1); countH >= 0; countH--) {
        unsigned char byteToSend = g_DisplayFrame.data [countV][countH] / 16;
        if (countH > 0) {
            byteToSend |= g_DisplayFrame.data [countV][countH - 1] * 16;
        }
        HAL_SPI__SendByte(byteToSend);
    }
    uint8_t decHigh = countV / 10;
    uint8_t decLow = countV - decHigh * 10;
    uint8_t sendDec = decHigh * 16 + decLow;
    HAL_SPI__SendByte(sendDec);
    HAL_PIO__DisplayLatch(PIN_ON);
    HAL_PIO__DisplayLatch(PIN_OFF);
    countV++;
    if (countV > VERTICAL_LINES_MAX) {
        countV = 0;
        g_EndOfFrameFlag = true;
    }

    _T1IF = 0;
} //T1Interrupt
//-----------------------------------------

void _ISR_PSV _T2Interrupt(void) //4000 Ãö
{
    //OS_EnterInt();
    //OS_Timer();
    //OS_LeaveInt();
    _T2IF = 0;
} //T2Interrupt

//-----------------------------------------

void _ISR_PSV _T3Interrupt(void) 
{
    if (g_SoundTimeCounter > 0) {
        g_SoundTimeCounter --;
        if (g_SoundTimeCounter & 0x0001) {
            HAL_PIO__SetBuzzerOut(PIN_ON);
        } else {
            HAL_PIO__SetBuzzerOut(PIN_OFF);
        }
    } else {
        HAL_PIO__SetBuzzerOut(PIN_OFF);
    }
    _T3IF = 0;
} //T3Interrupt

static char UART1_RX_Data;

void _ISR_PSV _U1RXInterrupt(void) {
    UART1_RX_Data = U1RXREG;
    _U1RXIF = 0;
}
//--

void _ISR_PSV _CNInterrupt(void) {
    _CNIF = 0;
}

void Interrupt__Setup(void) {

    _T1IP = 4; // this is the default value anyway
    _T2IP = 4; // this is the default value anyway
    _T3IP = 4; // this is the default value anyway
    _U1RXIP = 4; // this is the default value anyway
    _U2RXIP = 4; // this is the default value anyway
    _U3RXIP = 4; // this is the default value anyway
    _U4RXIP = 4; // this is the default value anyway

    TMR1 = 0; // clear the timer
    TMR2 = 0; // clear the timer
    TMR3 = 0; // clear the timer
    //PR1 = 1580 - 1; //16mHz 3200 - 1; // set the period register 2500 Hz 
    PR1 = 4740 - 1; //64 mHz 1580 - 1; //3200 - 1; // set the period register 2500 Hz 
    PR2 = 8000 - 1; // set the period register 1 kHz
    PR3 = 8000 - 1; // set the period register 1 kHz
    T1CON = 0x8000; // enabled, prescaler 1:1, internal clock  
    T2CON = 0x8000; // enabled, prescaler 1:1, internal clock   system tick RTOS
    T3CON = 0x8010; // enabled, prescaler 1:8, internal clock   system tick RTOS
    _T1IF = 0;
    _T2IF = 0;
    _T3IF = 0;
    _T1IE = 1; //enable interrupt TMR1
    //_T2IE = 1; //enable interrupt TMR2
    _T3IE = 1; //enable interrupt TMR3

    _U1RXIF = 0;
    _U1RXIE = 1;

    // 2.3 init the processor priority level
    _IPL = 0; // this is the default value anyway
}

bool Interrupt__DisableAll() {
    _IPL = 7;
}

bool Interrupt__IsFrameEnd() {
    if (g_EndOfFrameFlag == true) {
        g_EndOfFrameFlag = false;
        return true;
    } else {
        return false;
    }
}

void Interrupt__ShowFrame(DisplayFrame_t* displayFrame) {
    while (Interrupt__IsFrameEnd() == false) {
    };
    memcpy(&g_DisplayFrame, displayFrame, sizeof (DisplayFrame_t));
}

char Interrupt__GetUART1RX() {
    char dataRead = UART1_RX_Data;
    UART1_RX_Data = 0;
    return dataRead;
}

void Interrupt__PlaySound(uint16_t freq_Hz, uint16_t time_Ms) {
#define TIMER3_CLOCK (32000000/8)
#define SOUND_FREQ_CLOCK (TIMER3_CLOCK / 4)  
    PR3 = SOUND_FREQ_CLOCK / (uint32_t)freq_Hz - 1; // set the period register Hz
    g_SoundTimeCounter = (uint32_t)time_Ms * (uint32_t)freq_Hz / 250 ;
}
