idf_component_register(
    SRCS
        "artstrade_100x100_mono_100x100_L1.c"
        "blank_screens_100x100.c"
        "clock1_100x100_L1.c"
        "flame_animation1_100x500_L1.c"
        "HelloWorld_100x100_L1.c"
        "Pepsi_100x100_L1.c"
        "triden_100x100_L1.c"
    INCLUDE_DIRS "." ".."
    REQUIRES common
)

# The image converter emits flat initializers for 2D byte arrays. This is valid
# C; keep the generated assets unchanged and silence only this formatting warning.
set_source_files_properties(
    "artstrade_100x100_mono_100x100_L1.c"
    "clock1_100x100_L1.c"
    "flame_animation1_100x500_L1.c"
    "HelloWorld_100x100_L1.c"
    "Pepsi_100x100_L1.c"
    "triden_100x100_L1.c"
    PROPERTIES COMPILE_OPTIONS "-Wno-missing-braces"
)
