#include "delays.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void DelayMks(uint16_t timeMks)
{
    esp_rom_delay_us(timeMks);
}

void DelayMs(uint16_t timeMs)
{
    if (timeMs != 0) {
        /* Round up so short delays still yield to the scheduler. */
        TickType_t ticks = ((uint32_t)timeMs * configTICK_RATE_HZ + 999) / 1000;
        vTaskDelay(ticks);
    }
}
