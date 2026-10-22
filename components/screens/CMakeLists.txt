idf_component_register(
    SRCS
        "screens.c"
        "screens_common.c"
        "screen_analog_clock.c"
        "screen_flame_clock.c"
    INCLUDE_DIRS "." ".."
    REQUIRES common
    PRIV_REQUIRES graphics fonts images
)
