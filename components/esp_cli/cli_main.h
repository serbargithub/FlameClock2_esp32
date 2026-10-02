#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Register commands after initializing esp_console. */
esp_err_t CLI_RegisterCommands(void);

/* Start command input after UART setup and command registration. */
esp_err_t CLI_StartREPL(void);

#ifdef __cplusplus
}
#endif
