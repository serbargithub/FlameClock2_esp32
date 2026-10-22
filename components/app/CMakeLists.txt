# Explicit source list: PIC24 register drivers and busy-wait delays are
# deliberately excluded. ESP-IDF implementations supply those interfaces.
idf_component_register(
    SRCS
        "../peripferals/esp32_c3_periferals/rtcc_DS3231.c"
        "../peripferals/esp32_c3_periferals/i2c_esp.c"
        "../peripferals/esp32_c3_periferals/uart_console_esp.c"
        "../peripferals/esp32_c3_periferals/peripherals_esp.c"
        "../peripferals/esp32_c3_periferals/spi_esp.c"
        "interrupts_esp.c"
        "clock_manager.c"
        "clock_task.c"
        "debugging.c"
        "sound.c"
    INCLUDE_DIRS ".." "."
    REQUIRES common delays esp_driver_i2c
    PRIV_REQUIRES screens logger fonts images graphics freertos esp_rom esp_adc esp_driver_gpio esp_driver_gptimer
                  esp_driver_ledc esp_driver_spi esp_driver_uart esp_timer
)
