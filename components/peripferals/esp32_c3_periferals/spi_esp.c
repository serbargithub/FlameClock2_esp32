#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "driver/spi_master.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "../spi_HAL.h"
#include "io_ports_names_esp.h"

#define DISPLAY_SPI_HOST               SPI2_HOST
#define DISPLAY_SPI_FREQUENCY          8000000
#define DISPLAY_SPI_MAX_TRANSFER_SIZE  32

static spi_device_handle_t s_display_spi;
static SemaphoreHandle_t s_display_spi_lock;

void HAL_SPI__Init(void)
{
    if (s_display_spi_lock == NULL) {
        s_display_spi_lock = xSemaphoreCreateMutex();
        ESP_ERROR_CHECK(s_display_spi_lock != NULL ? ESP_OK : ESP_ERR_NO_MEM);
    }

    (void)xSemaphoreTake(s_display_spi_lock, portMAX_DELAY);
    if (s_display_spi != NULL) {
        (void)xSemaphoreGive(s_display_spi_lock);
        return;
    }

    const spi_bus_config_t bus_config = {
        .mosi_io_num = DISPLAY_SPI_MOSI_GPIO,
        .miso_io_num = -1,
        .sclk_io_num = DISPLAY_SPI_CLK_GPIO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = DISPLAY_SPI_MAX_TRANSFER_SIZE,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(DISPLAY_SPI_HOST,
                                       &bus_config,
                                       SPI_DMA_DISABLED));

    const spi_device_interface_config_t device_config = {
        .clock_speed_hz = DISPLAY_SPI_FREQUENCY,
        .mode = 0,
        .spics_io_num = -1,
        .queue_size = 1,
    };
    ESP_ERROR_CHECK(spi_bus_add_device(DISPLAY_SPI_HOST,
                                       &device_config,
                                       &s_display_spi));
    (void)xSemaphoreGive(s_display_spi_lock);
}

void HAL_SPI__TurnOff(void)
{
    if (s_display_spi_lock == NULL) {
        return;
    }

    (void)xSemaphoreTake(s_display_spi_lock, portMAX_DELAY);
    if (s_display_spi == NULL) {
        (void)xSemaphoreGive(s_display_spi_lock);
        return;
    }

    ESP_ERROR_CHECK(spi_bus_remove_device(s_display_spi));
    s_display_spi = NULL;
    ESP_ERROR_CHECK(spi_bus_free(DISPLAY_SPI_HOST));
    (void)xSemaphoreGive(s_display_spi_lock);
}

void HAL_SPI__SendBuffer(const uint8_t *data, size_t length)
{
    if (data == NULL || length == 0 || s_display_spi_lock == NULL) {
        return;
    }
    ESP_ERROR_CHECK(length <= DISPLAY_SPI_MAX_TRANSFER_SIZE
                        ? ESP_OK
                        : ESP_ERR_INVALID_SIZE);

    (void)xSemaphoreTake(s_display_spi_lock, portMAX_DELAY);
    spi_transaction_t transaction = {
        .length = length * 8U,
        .tx_buffer = data,
    };
    const esp_err_t result = s_display_spi == NULL
                                 ? ESP_ERR_INVALID_STATE
                                 : spi_device_polling_transmit(s_display_spi,
                                                               &transaction);
    (void)xSemaphoreGive(s_display_spi_lock);
    ESP_ERROR_CHECK(result);
}

uint8_t HAL_SPI__SendByte(uint8_t value)
{
    HAL_SPI__SendBuffer(&value, sizeof(value));
    return 0;
}

uint8_t HAL_SPI__GetByte(void)
{
    return HAL_SPI__SendByte(0);
}
