#ifndef IO_PORTS_NAMES_ESP_H
#define IO_PORTS_NAMES_ESP_H

#include "driver/gpio.h"
#include "hal/adc_types.h"

/* Board connections from doc/esp32_c3_periferals.md. */
#define POWER_SENSOR_GPIO      GPIO_NUM_0
#define POWER_SENSOR_ADC_UNIT  ADC_UNIT_1
#define POWER_SENSOR_ADC_CHAN  ADC_CHANNEL_0

#define STEP_UP1_GPIO          GPIO_NUM_1
#define BUTTON1_GPIO           GPIO_NUM_2
#define BUTTON2_GPIO           GPIO_NUM_3
#define BUZZER_GPIO            GPIO_NUM_4
#define DISPLAY_LATCH_GPIO     GPIO_NUM_5
#define DISPLAY_SPI_CLK_GPIO   GPIO_NUM_6
#define DISPLAY_SPI_MOSI_GPIO  GPIO_NUM_7
#define DISPLAY_SHIFT_GPIO     DISPLAY_SPI_CLK_GPIO
#define DISPLAY_DATA_GPIO      DISPLAY_SPI_MOSI_GPIO
#define BUTTON3_GPIO           GPIO_NUM_8
#define BUTTON4_GPIO           GPIO_NUM_9

#define I2C1_SDA_GPIO          GPIO_NUM_18
#define I2C1_SCL_GPIO          GPIO_NUM_19

#define UART0_RX_GPIO          GPIO_NUM_20
#define UART0_TX_GPIO          GPIO_NUM_21


/* PIC24 used 800/1023; keep the same ratio with the ESP32-C3 12-bit ADC. */
#define POWER_PRESENT_THRESHOLD 3200

#endif /* IO_PORTS_NAMES_ESP_H */
