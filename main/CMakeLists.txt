idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES clock_port esp_cli
                    PRIV_REQUIRES console)
