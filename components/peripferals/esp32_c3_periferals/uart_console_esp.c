/* Legacy UART_CH1 maps to the ESP-IDF UART console (UART0 on this board).
 * The console pins are configured by ESP-IDF before app_main. Preserve them
 * so boot messages and application logs use the same USB-to-UART connection.
 */
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_err.h"
#include "driver/uart.h"
#include "driver/uart_vfs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "interrupts.h"
#include "peripferals/uart_HAL.h"

#if !CONFIG_ESP_CONSOLE_UART
#error "UART logging requires an ESP-IDF UART console in menuconfig"
#endif

static const uart_port_t s_port = CONFIG_ESP_CONSOLE_UART_NUM;
static QueueHandle_t s_events;
static GETCHAR_CALLBACK s_getchar;
static bool s_initialized;

void HAL_UART__SetExternGetch(GETCHAR_CALLBACK callback)
{
    s_getchar = callback;
}

void HAL_UART__SerialSetup(UART_Speed_t speed, UART_Channel_t channel)
{
    static const int baud_rates[] = {2400, 4800, 9600, 19200, 38400, 57600, 115200};
    ESP_ERROR_CHECK(channel == UART_CH1 ? ESP_OK : ESP_ERR_NOT_SUPPORTED);
    ESP_ERROR_CHECK((unsigned)speed < sizeof(baud_rates) / sizeof(baud_rates[0])
                    ? ESP_OK : ESP_ERR_INVALID_ARG);
    if (s_initialized) {
        ESP_ERROR_CHECK(uart_set_baudrate(s_port, baud_rates[speed]));
        return;
    }

    fflush(stdout);
    const uart_config_t config = {
        .baud_rate = baud_rates[speed],
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    ESP_ERROR_CHECK(uart_param_config(s_port, &config));
    ESP_ERROR_CHECK(uart_set_pin(s_port, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE,
                                UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(s_port, 1024, 0, 20, &s_events, 0));
    uart_vfs_dev_use_driver(s_port);
    ESP_ERROR_CHECK(uart_vfs_dev_port_set_tx_line_endings(s_port, ESP_LINE_ENDINGS_CRLF)
                    == 0 ? ESP_OK : ESP_FAIL);
    setvbuf(stdout, NULL, _IONBF, 0);
    s_initialized = true;
}

bool HAL_UART__CheckAndResetErrors(UART_Channel_t channel)
{
    if (channel != UART_CH1 || !s_initialized) {
        return false;
    }
    bool error = false;
    uart_event_t event;
    while (xQueueReceive(s_events, &event, 0) == pdTRUE) {
        if (event.type == UART_FIFO_OVF || event.type == UART_BUFFER_FULL) {
            ESP_ERROR_CHECK(uart_flush_input(s_port));
            error = true;
        } else if (event.type == UART_FRAME_ERR || event.type == UART_PARITY_ERR) {
            error = true;
        }
    }
    return error;
}

static char GetUART_CH1_RX(void)
{
    char value = 0;
    if (s_initialized) {
        HAL_UART__CheckAndResetErrors(UART_CH1);
        if (uart_read_bytes(s_port, &value, 1, 0) != 1) {
            value = 0;
        }
    }
    return value;
}

char getch(void)
{
    return s_getchar ? s_getchar() : GetUART_CH1_RX();
}

void putch(char value)
{
    if (s_initialized) {
        uart_write_bytes(s_port, &value, 1);
    }
}

void HAL_UART__TurnOff(UART_Channel_t channel)
{
    if (channel != UART_CH1 || !s_initialized) {
        return;
    }
    fflush(stdout);
    ESP_ERROR_CHECK(uart_wait_tx_done(s_port, pdMS_TO_TICKS(1000)));
    /* Keep the boot console available after releasing the buffered driver. */
    uart_vfs_dev_use_nonblocking(s_port);
    ESP_ERROR_CHECK(uart_driver_delete(s_port));
    s_events = NULL;
    s_initialized = false;
}
