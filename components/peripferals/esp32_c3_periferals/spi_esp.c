#include <stdbool.h>
#include <stdint.h>

#include "driver/spi_master.h"
#include "esp_err.h"

#include "../spi_HAL.h"
#include "io_ports_names_esp.h"

#define DISPLAY_SPI_HOST       SPI2_HOST
#define DISPLAY_SPI_FREQUENCY  1000000

static spi_device_handle_t s_display_spi;

void HAL_SPI__Init(void)
{
    if (s_display_spi != NULL) {
        return;
    }

    const spi_bus_config_t bus_config = {
        .mosi_io_num = DISPLAY_SPI_MOSI_GPIO,
        .miso_io_num = -1,
        .sclk_io_num = DISPLAY_SPI_CLK_GPIO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 1,
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
}

void HAL_SPI__TurnOff(void)
{
    if (s_display_spi == NULL) {
        return;
    }

    ESP_ERROR_CHECK(spi_bus_remove_device(s_display_spi));
    s_display_spi = NULL;
    ESP_ERROR_CHECK(spi_bus_free(DISPLAY_SPI_HOST));
}

uint8_t HAL_SPI__SendByte(uint8_t value)
{
    if (s_display_spi == NULL) {
        return 0;
    }

    spi_transaction_t transaction = {
        .flags = SPI_TRANS_USE_TXDATA,
        .length = 8,
        .tx_data = {value},
    };
    ESP_ERROR_CHECK(spi_device_transmit(s_display_spi, &transaction));
    return 0;
}

uint8_t HAL_SPI__GetByte(void)
{
    return HAL_SPI__SendByte(0);
}
