/* DS3231M RTC backend. Register map: Analog Devices DS3231M data sheet.
 * Task-context API; initialize before starting any concurrent RTC users.
 */
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "../rtcc.h"
#include "../i2c_HAL.h"

#define DS3231_ADDRESS          0x68
#define DS3231_REG_TIME         0x00
#define DS3231_REG_CONTROL      0x0e
#define DS3231_REG_STATUS       0x0f
#define DS3231_CONTROL_EOSC     0x80
#define DS3231_STATUS_OSF       0x80
#define DS3231_STATUS_ALARMS    0x03
#define DS3231_HOURS_12H        0x40
#define DS3231_HOURS_PM         0x20
#define DS3231_MONTH_CENTURY    0x80

static const char *TAG = "rtcc_ds3231";
static HAL_I2C_DeviceHandle_t s_rtc;
static bool s_started;
static esp_err_t s_last_error = ESP_OK;

/* Avoid flooding the console when Clock_Task polls a disconnected RTC. */
static bool RTCC_CheckResult(esp_err_t result)
{
    if (result != s_last_error) {
        if (result != ESP_OK) {
            ESP_LOGE(TAG, "RTC operation failed: %s", esp_err_to_name(result));
        } else {
            ESP_LOGI(TAG, "RTC communication recovered");
        }
        s_last_error = result;
    }
    return result == ESP_OK;
}

static uint8_t RTCC_DecToBCD(uint8_t value)
{
    return (uint8_t)(((value / 10U) << 4) | (value % 10U));
}

static uint8_t RTCC_BCDToDec(uint8_t value)
{
    return (uint8_t)((value >> 4) * 10U + (value & 0x0f));
}

static bool RTCC_ConvertFormat(RTCC_DATETIME *value, bool bcd_format)
{
    uint8_t *fields[] = {
        &value->year, &value->month, &value->day, &value->weekday,
        &value->hour, &value->minute, &value->second,
    };
    if (value->bcdFormat) {
        for (size_t i = 0; i < sizeof(fields) / sizeof(fields[0]); ++i) {
            if ((*fields[i] & 0x0f) > 9 || (*fields[i] >> 4) > 9) {
                return false;
            }
        }
    }
    if (value->bcdFormat != bcd_format) {
        for (size_t i = 0; i < sizeof(fields) / sizeof(fields[0]); ++i) {
            *fields[i] = bcd_format ? RTCC_DecToBCD(*fields[i])
                                    : RTCC_BCDToDec(*fields[i]);
        }
        value->bcdFormat = bcd_format;
    }
    return true;
}

/* The application represents only years 2000..2099. */
static bool RTCC_IsValid(const RTCC_DATETIME *value)
{
    static const uint8_t days_in_month[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
    };
    if (value->year > 99 || value->month < 1 || value->month > 12 ||
        value->weekday < 1 || value->weekday > 7 ||
        value->hour > 23 || value->minute > 59 || value->second > 59) {
        return false;
    }
    uint8_t max_day = days_in_month[value->month - 1];
    if (value->month == 2 && value->year % 4 == 0) {
        ++max_day;
    }
    return value->day >= 1 && value->day <= max_day;
}

static esp_err_t RTCC_ReadRegisters(uint8_t reg, uint8_t *data, size_t length)
{
    return HAL_I2C__WriteRead(s_rtc, &reg, sizeof(reg), data, length,
                              HAL_I2C_DEFAULT_TIMEOUT_MS);
}

static esp_err_t RTCC_WriteRegister(uint8_t reg, uint8_t data)
{
    const uint8_t buffer[] = {reg, data};
    return HAL_I2C__Write(s_rtc, buffer, sizeof(buffer),
                          HAL_I2C_DEFAULT_TIMEOUT_MS);
}

static esp_err_t RTCC_EnsureStarted(void)
{
    if (s_started) {
        return ESP_OK;
    }
    esp_err_t result;
    if (s_rtc == NULL) {
        result = HAL_I2C__Init();
        if (result != ESP_OK) {
            return result;
        }
        result = HAL_I2C__AddDevice(DS3231_ADDRESS, HAL_I2C_DEFAULT_SPEED_HZ,
                                     &s_rtc);
        if (result != ESP_OK) {
            return result;
        }
    }

    uint8_t control;
    result = RTCC_ReadRegisters(DS3231_REG_CONTROL, &control, sizeof(control));
    if (result == ESP_OK && (control & DS3231_CONTROL_EOSC) != 0) {
        /* Enable battery-backed timekeeping; preserve alarm/SQW settings. */
        result = RTCC_WriteRegister(DS3231_REG_CONTROL,
                                    control & (uint8_t)~DS3231_CONTROL_EOSC);
    }
    s_started = result == ESP_OK;
    return result;
}

void RTCC_Start(void)
{
    (void)RTCC_CheckResult(RTCC_EnsureStarted());
}

void RTCC_Initialize(RTCC_DATETIME *value)
{
    if (value == NULL) {
        (void)RTCC_CheckResult(ESP_ERR_INVALID_ARG);
        return;
    }
    RTCC_DATETIME date = *value;
    if (!RTCC_ConvertFormat(&date, false) || !RTCC_IsValid(&date)) {
        (void)RTCC_CheckResult(ESP_ERR_INVALID_ARG);
        return;
    }
    esp_err_t result = RTCC_EnsureStarted();
    if (result != ESP_OK) {
        (void)RTCC_CheckResult(result);
        return;
    }

    /* One burst writes all seven time registers within the required 1s.
     * Always write 24-hour mode and century=0 (2000..2099).
     */
    const uint8_t buffer[] = {
        DS3231_REG_TIME,
        RTCC_DecToBCD(date.second), RTCC_DecToBCD(date.minute),
        RTCC_DecToBCD(date.hour), date.weekday, RTCC_DecToBCD(date.day),
        RTCC_DecToBCD(date.month), RTCC_DecToBCD(date.year),
    };
    result = HAL_I2C__Write(s_rtc, buffer, sizeof(buffer),
                             HAL_I2C_DEFAULT_TIMEOUT_MS);
    if (result == ESP_OK) {
        uint8_t status;
        result = RTCC_ReadRegisters(DS3231_REG_STATUS, &status, sizeof(status));
        if (result == ESP_OK) {
            /* Clear OSF only after a successful time write. Writing ones to
             * A1F/A2F preserves even an alarm that asserts after this read.
             */
            status = (status & (uint8_t)~DS3231_STATUS_OSF) |
                     DS3231_STATUS_ALARMS;
            result = RTCC_WriteRegister(DS3231_REG_STATUS, status);
        }
    }
    (void)RTCC_CheckResult(result);
}

void RTCC_TimeGet(RTCC_DATETIME *value)
{
    if (value == NULL) {
        (void)RTCC_CheckResult(ESP_ERR_INVALID_ARG);
        return;
    }
    esp_err_t result = RTCC_EnsureStarted();
    uint8_t registers[DS3231_REG_STATUS + 1];
    if (result == ESP_OK) {
        /* DS3231M snapshots time on START, so this burst is rollover-safe. */
        result = RTCC_ReadRegisters(DS3231_REG_TIME, registers,
                                     sizeof(registers));
    }
    if (result != ESP_OK) {
        /* A transient bus failure must not replace the last good reading. */
        (void)RTCC_CheckResult(result);
        return;
    }

    RTCC_DATETIME date = {
        .bcdFormat = true,
        .year = registers[6],
        .month = registers[5] & 0x1f,
        .day = registers[4] & 0x3f,
        .weekday = registers[3] & 0x07,
        .hour = registers[2] & 0x3f,
        .minute = registers[1] & 0x7f,
        .second = registers[0] & 0x7f,
    };
    bool valid = (registers[DS3231_REG_STATUS] & DS3231_STATUS_OSF) == 0 &&
                 (registers[5] & DS3231_MONTH_CENTURY) == 0;
    if ((registers[2] & DS3231_HOURS_12H) != 0) {
        const uint8_t hour_bcd = registers[2] & 0x1f;
        const uint8_t hour = RTCC_BCDToDec(hour_bcd);
        valid = valid && (hour_bcd & 0x0f) <= 9 && hour >= 1 && hour <= 12;
        date.hour = RTCC_DecToBCD((hour % 12U) +
                        ((registers[2] & DS3231_HOURS_PM) != 0 ? 12U : 0U));
    }
    valid = valid && RTCC_ConvertFormat(&date, false) && RTCC_IsValid(&date);
    if (!valid) {
        /* year/month/day zero causes Clock_Init's existing build-time reset.
         * Do not clear OSF here: only setting a valid time acknowledges it.
         */
        *value = (RTCC_DATETIME){.bcdFormat = value->bcdFormat};
        (void)RTCC_CheckResult(ESP_ERR_INVALID_STATE);
        return;
    }
    (void)RTCC_ConvertFormat(&date, value->bcdFormat);
    *value = date;
    (void)RTCC_CheckResult(ESP_OK);
}

void RTCC_BuildTimeGet(RTCC_DATETIME *value)
{
    if (value == NULL) {
        (void)RTCC_CheckResult(ESP_ERR_INVALID_ARG);
        return;
    }
    static const char months[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    static const uint8_t weekday_offsets[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    char month_name[4] = {0};
    unsigned int day = 0, year = 0, hour = 0, minute = 0, second = 0;
    if (sscanf(__DATE__, "%3s %u %u", month_name, &day, &year) != 3 ||
        sscanf(__TIME__, "%u:%u:%u", &hour, &minute, &second) != 3) {
        *value = (RTCC_DATETIME){0};
        (void)RTCC_CheckResult(ESP_ERR_INVALID_STATE);
        return;
    }
    const char *month = strstr(months, month_name);
    if (month == NULL || year < 2000 || year > 2099) {
        *value = (RTCC_DATETIME){0};
        (void)RTCC_CheckResult(ESP_ERR_INVALID_STATE);
        return;
    }
    const unsigned int month_number = (unsigned int)(month - months) / 3U + 1U;
    const unsigned int weekday_year = year - (month_number < 3 ? 1U : 0U);
    const unsigned int weekday = (weekday_year + weekday_year / 4U -
        weekday_year / 100U + weekday_year / 400U +
        weekday_offsets[month_number - 1] + day) % 7U;
    /* Always decimal, matching the ESP32 stub contract. Clock_Init also calls
     * this with an uninitialized local, so never inspect its bcdFormat field.
     */
    *value = (RTCC_DATETIME){
        .bcdFormat = false,
        .year = year % 100U,
        .month = month_number,
        .day = day,
        .weekday = weekday == 0 ? 7 : weekday,
        .hour = hour,
        .minute = minute,
        .second = second,
    };
}
