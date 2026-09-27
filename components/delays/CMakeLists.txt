idf_component_register(
    SRCS "delays_esp.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES freertos esp_rom
)
