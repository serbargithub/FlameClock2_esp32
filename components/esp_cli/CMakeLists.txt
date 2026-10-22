idf_component_register(
    SRCS "cli_main.c"
    INCLUDE_DIRS "."
    REQUIRES esp_common
    PRIV_REQUIRES common console freertos esp_driver_uart
)
