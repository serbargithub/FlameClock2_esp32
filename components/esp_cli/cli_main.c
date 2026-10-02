#include <stdio.h>
#include <string.h>

#include "cli_main.h"
#include "esp_console.h"
#include "driver/uart_vfs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../app/definitions.h"

static TaskHandle_t s_repl_task;

static int cli_info(int argc, char **argv)
{
    (void)argv;

    if (argc != 1) {
        printf("Usage: info\n");
        return 1;
    }

    printf("Product: %s\n", DEVICE_NAME);
    printf("Version: %s\n", DEVICE_VERSION);
    printf("Build time: %s %s\n", BUILD_DATE, BUILD_TIME);
    return 0;
}

/* Call after esp_console initialization and before starting the REPL. */
esp_err_t CLI_RegisterCommands(void)
{
    const esp_console_cmd_t command = {
        .command = "info",
        .help = "Print product name, version and build time",
        .hint = NULL,
        .func = cli_info,
        .argtable = NULL,
    };

    return esp_console_cmd_register(&command);
}

static void cli_repl_task(void *argument)
{
    (void)argument;
    char line[256];

    setvbuf(stdin, NULL, _IONBF, 0);
    printf("Type 'help' to list commands.\n");

    while (1) {
        printf("%s> ", DEVICE_NAME);
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            clearerr(stdin);
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        if (strchr(line, '\n') == NULL) {
            int character;
            while ((character = getchar()) != '\n' && character != EOF) {
            }
            printf("Command is too long.\n");
            continue;
        }

        line[strcspn(line, "\r\n")] = '\0';
        int command_result = 0;
        esp_err_t error = esp_console_run(line, &command_result);
        if (error == ESP_ERR_NOT_FOUND) {
            printf("Unknown command. Type 'help'.\n");
        } else if (error == ESP_ERR_INVALID_ARG) {
            /* Ignore empty input. */
        } else if (error != ESP_OK) {
            printf("Console error: %s\n", esp_err_to_name(error));
        } else if (command_result != 0) {
            printf("Command returned: %d\n", command_result);
        }
    }
}

esp_err_t CLI_StartREPL(void)
{
    if (s_repl_task != NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    /* UART is already installed by HAL_UART__SerialSetup(). */
    if (uart_vfs_dev_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM,
                                             ESP_LINE_ENDINGS_CR) != 0) {
        return ESP_FAIL;
    }

    esp_err_t error = esp_console_register_help_command();
    if (error != ESP_OK) {
        return error;
    }

    return xTaskCreate(cli_repl_task, "cli_repl", 4096, NULL, 2, &s_repl_task)
           == pdPASS ? ESP_OK : ESP_ERR_NO_MEM;
}
