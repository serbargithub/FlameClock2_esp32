#include <stdbool.h>

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"

#include "../peripherals_HAL.h"
#include "io_ports_names_esp.h"

static adc_oneshot_unit_handle_t s_adc_handle;

void HAL_ConrolReg__HighPerfomance(void)
{
    /* CPU frequency is configured by ESP-IDF power management. */
}

void HAL_ConrolReg__LowPerfomance(void)
{
    /* CPU frequency is configured by ESP-IDF power management. */
}

void HAL_PIO__Init_IOPorts(void)
{
    const gpio_config_t output_config = {
        .pin_bit_mask = (1ULL << STEP_UP1_GPIO) |
                        (1ULL << BUZZER_GPIO) |
                        (1ULL << DISPLAY_LATCH_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&output_config));

    gpio_set_level(STEP_UP1_GPIO, 0);
    gpio_set_level(BUZZER_GPIO, 0);
    gpio_set_level(DISPLAY_LATCH_GPIO, 0);

    const gpio_config_t button_config = {
        .pin_bit_mask = (1ULL << BUTTON1_GPIO) |
                        (1ULL << BUTTON2_GPIO) |
                        (1ULL << BUTTON3_GPIO) |
                        (1ULL << BUTTON4_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&button_config));
}

void HAL_PIO__TurnOff_IOPorts(void)
{
    const gpio_num_t output_pins[] = {
        STEP_UP1_GPIO,
        BUZZER_GPIO,
        DISPLAY_LATCH_GPIO,
    };

    for (unsigned int i = 0; i < sizeof(output_pins) / sizeof(output_pins[0]); ++i) {
        gpio_set_level(output_pins[i], 0);
        gpio_set_direction(output_pins[i], GPIO_MODE_INPUT);
        gpio_set_pull_mode(output_pins[i], GPIO_FLOATING);
    }

    gpio_set_pull_mode(BUTTON1_GPIO, GPIO_FLOATING);
    gpio_set_pull_mode(BUTTON2_GPIO, GPIO_FLOATING);
    gpio_set_pull_mode(BUTTON3_GPIO, GPIO_FLOATING);
    gpio_set_pull_mode(BUTTON4_GPIO, GPIO_FLOATING);
}

void HAL_PIO__SetInformLed(PinValue_t value)
{
    /* No information LED is assigned in the ESP32-C3 board pin map. */
    (void)value;
}

void HAL_PIO__SetBuzzerOut(PinValue_t value)
{
    gpio_set_level(BUZZER_GPIO, value == PIN_ON);
}

void HAL_PIO__BuckUp1Out(PinValue_t value)
{
    gpio_set_level(STEP_UP1_GPIO, value == PIN_ON);
}

void HAL_PIO__BuckUp2Out(PinValue_t value)
{
    /* The ESP32-C3 board has only one step-up output in its pin map. */
    (void)value;
}

void HAL_PIO__DisplayLatch(PinValue_t value)
{
    gpio_set_level(DISPLAY_LATCH_GPIO, value == PIN_ON);
}

bool HAL_PIO__GetButtonState(ButtonsName_t button)
{
    gpio_num_t pin;

    switch (button) {
        case BUTTON1:
            pin = BUTTON1_GPIO;
            break;
        case BUTTON2:
            pin = BUTTON2_GPIO;
            break;
        case BUTTON3:
            pin = BUTTON3_GPIO;
            break;
        case BUTTON4:
            pin = BUTTON4_GPIO;
            break;
        default:
            return false;
    }

    return gpio_get_level(pin) != 0;
}

bool HAL_ADC__GetPowerState(void)
{
    /* TODO: Restore ADC power detection after validating the sensor threshold. */
    return true;
}

void HAL_ADC__InitADC(void)
{
    if (s_adc_handle != NULL) {
        return;
    }

    const adc_oneshot_unit_init_cfg_t unit_config = {
        .unit_id = POWER_SENSOR_ADC_UNIT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_config, &s_adc_handle));

    const adc_oneshot_chan_cfg_t channel_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(s_adc_handle,
                                                POWER_SENSOR_ADC_CHAN,
                                                &channel_config));
}

void HAL_MAP__GeneralPeripheralsMapping(void)
{
    /* SPI and I2C signals are routed by their individual ESP-IDF drivers. */
}
