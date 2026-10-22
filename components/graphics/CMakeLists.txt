idf_component_register(
    SRCS
        "display_utils.c"
        "draw.c"
    INCLUDE_DIRS "." ".."
    REQUIRES common
    PRIV_REQUIRES fonts
)
