/* Build-only hardware backend. No GPIO, ADC, SPI, UART or timer setup.
 * RTC stores a fixed software date; it does not advance or survive a reset.
 * Replace these interfaces with ESP-IDF drivers during hardware bring-up.
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "interrupts.h"
#include "peripferals/peripherals_HAL.h"
#include "peripferals/uart_HAL.h"
#include "peripferals/spi_HAL.h"
#include "peripferals/rtcc.h"

static RTCC_DATETIME s_datetime;
static GETCHAR_CALLBACK s_getchar;

void HAL_ConrolReg__HighPerfomance(void) {}
void HAL_ConrolReg__LowPerfomance(void) {}
void HAL_PIO__Init_IOPorts(void) {}
void HAL_PIO__TurnOff_IOPorts(void) {}
void HAL_ADC__InitADC(void) {}
void HAL_MAP__GeneralPeripheralsMapping(void) {}
bool HAL_PIO__GetButtonState(ButtonsName_t button) { (void)button; return true; }
bool HAL_ADC__GetPowerState(void) { return true; }
void HAL_PIO__SetInformLed(PinValue_t value) { (void)value; }
void HAL_PIO__SetBuzzerOut(PinValue_t value) { (void)value; }
void HAL_PIO__BuckUp1Out(PinValue_t value) { (void)value; }
void HAL_PIO__BuckUp2Out(PinValue_t value) { (void)value; }
void HAL_PIO__DisplayLatch(PinValue_t value) { (void)value; }

void HAL_UART__SetExternGetch(GETCHAR_CALLBACK callback) { s_getchar = callback; }
void HAL_UART__SerialSetup(UART_Speed_t speed, UART_Channel_t channel)
{ (void)speed; (void)channel; }
bool HAL_UART__CheckAndResetErrors(UART_Channel_t channel)
{ (void)channel; return false; }
void HAL_UART__TurnOff(UART_Channel_t channel) { (void)channel; }
void putch(char c) { (void)c; }
char getch(void) { return s_getchar ? s_getchar() : 0; }

void HAL_SPI__TurnOff(void) {}
void HAL_SPI__Init(void) {}
uint8_t HAL_SPI__SendByte(uint8_t value) { (void)value; return 0; }
uint8_t HAL_SPI__GetByte(void) { return 0; }

void Interrupt__Setup(void) {}
bool Interrupt__DisableAll(void) { return true; }
void Interrupt__ShowFrame(DisplayFrame_t *frame) { (void)frame; }
bool Interrupt__IsFrameEnd(void) { return true; }
char Interrupt__GetUART1RX(void) { return 0; }
void Interrupt__PlaySound(uint16_t frequency, uint16_t duration)
{ (void)frequency; (void)duration; }

void RTCC_BuildTimeGet(RTCC_DATETIME *value)
{
    static const char months[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    char month_name[4];
    int day, year, hour, minute, second;
    sscanf(__DATE__, "%3s %d %d", month_name, &day, &year);
    sscanf(__TIME__, "%d:%d:%d", &hour, &minute, &second);
    const char *month = strstr(months, month_name);
    struct tm build = {
        .tm_year = year - 1900,
        .tm_mon = month ? (month - months) / 3 : 0,
        .tm_mday = day,
        .tm_hour = hour,
        .tm_min = minute,
        .tm_sec = second,
        .tm_isdst = -1,
    };
    mktime(&build);
    *value = (RTCC_DATETIME) {
        .bcdFormat = false,
        .year = year % 100,
        .month = build.tm_mon + 1,
        .day = day,
        .weekday = build.tm_wday == 0 ? 7 : build.tm_wday,
        .hour = hour,
        .minute = minute,
        .second = second,
    };
}

void RTCC_Start(void) { RTCC_BuildTimeGet(&s_datetime); }
void RTCC_Initialize(RTCC_DATETIME *value) { s_datetime = *value; }
void RTCC_TimeGet(RTCC_DATETIME *value) { *value = s_datetime; }
