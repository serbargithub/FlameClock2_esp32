# Explicit source list: PIC24 register drivers, interrupts and busy-wait delays
# are deliberately excluded. Software stubs supply their public interfaces.
idf_component_register(
    SRCS
        "hardware_stubs.c"
        "delays_esp.c"
        "../app/adjusting.c"
        "../app/clock_task.c"
        "../app/debugging.c"
        "../app/display_utils.c"
        "../app/log.c"
        "../app/sound.c"
        "../graphics/draw.c"
        "../screens/screens.c"
        "../screens/screen_analog_clock.c"
        "../screens/screen_flame_clock.c"
        "../fonts/fonts.c"
        "../fonts/Antonio-Regular_16_L1.c"
        "../fonts/BebasNeue-Bold_16_L1.c"
        "../fonts/BebasNeue-Regular_12_L1.c"
        "../fonts/BebasNeue-Regular_30_L1.c"
        "../fonts/m12_16_L1.c"
        "../fonts/m12_20_L1.c"
        "../images/artstrade_100x100_mono_100x100_L1.c"
        "../images/blank_screens_100x100.c"
        "../images/clock1_100x100_L1.c"
        "../images/flame_animation1_100x500_L1.c"
        "../images/HelloWorld_100x100_L1.c"
        "../images/Pepsi_100x100_L1.c"
        "../images/triden_100x100_L1.c"
        "../peripferals/log_HAL.c"
    INCLUDE_DIRS ".." "../app"
    PRIV_REQUIRES freertos esp_rom
)

# The image converter emits flat initializers for 2D byte arrays. This is valid
# C; keep the generated assets unchanged and silence only this formatting warning.
set_source_files_properties(
    "../images/artstrade_100x100_mono_100x100_L1.c"
    "../images/clock1_100x100_L1.c"
    "../images/flame_animation1_100x500_L1.c"
    "../images/HelloWorld_100x100_L1.c"
    "../images/Pepsi_100x100_L1.c"
    "../images/triden_100x100_L1.c"
    PROPERTIES COMPILE_OPTIONS "-Wno-missing-braces"
)
