/* Build-only hardware backend. No GPIO, ADC, SPI or timer setup.
 * RTC stores a fixed software date; it does not advance or survive a reset.
 * Replace these interfaces with ESP-IDF drivers during hardware bring-up.
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "interrupts.h"
#include "peripferals/peripherals_HAL.h"
#include "peripferals/spi_HAL.h"
#include "peripferals/rtcc.h"

static RTCC_DATETIME s_datetime;

void Interrupt__Setup(void) {}
bool Interrupt__DisableAll(void) { return true; }
void Interrupt__ShowFrame(DisplayFrame_t *frame) { (void)frame; }
bool Interrupt__IsFrameEnd(void) { return true; }
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
