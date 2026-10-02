#include <stdbool.h>
#include <stdint.h>
#include <stdatomic.h>
#include <string.h>

#include "driver/gptimer.h"
#include "driver/ledc.h"
#include "esp_attr.h"
#include "esp_err.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "definitions.h"
#include "interrupts.h"
#include "peripferals/esp32_c3_periferals/io_ports_names_esp.h"
#include "peripferals/peripherals_HAL.h"
#include "peripferals/spi_HAL.h"
#include "peripferals/uart_HAL.h"

#define DISPLAY_TIMER_RESOLUTION_HZ  1000000U
#define DISPLAY_LINE_PERIOD_TICKS    148U
#define DISPLAY_TASK_STACK_SIZE      3072U
#define DISPLAY_TASK_PRIORITY        (configMAX_PRIORITIES - 2)
#define DISPLAY_TASK_NOTIFICATION    (1UL << 0)

#define BUZZER_LEDC_MODE             LEDC_LOW_SPEED_MODE
#define BUZZER_LEDC_TIMER            LEDC_TIMER_0
#define BUZZER_LEDC_CHANNEL          LEDC_CHANNEL_0
#define BUZZER_LEDC_DUTY_RESOLUTION  LEDC_TIMER_10_BIT
#define BUZZER_LEDC_HALF_DUTY        512U
#define BUZZER_INITIAL_FREQUENCY_HZ  1000U

static DisplayFrame_t s_display_frames[2];
static _Atomic uint8_t s_active_frame;
static atomic_bool s_frame_pending;
static atomic_bool s_frame_end;
static atomic_bool s_display_enabled;
static atomic_bool s_reset_scan;

static TaskHandle_t s_display_task;
static SemaphoreHandle_t s_display_lock;
static gptimer_handle_t s_display_timer;
static esp_timer_handle_t s_sound_stop_timer;
static bool s_initialized;
static bool s_display_timer_running;

static void Display_SendLine(uint8_t line)
{
    uint8_t transfer[HORIZONTAL_BYTES_MAX + 1];
    const uint8_t active_frame = atomic_load_explicit(&s_active_frame,
                                                       memory_order_acquire);
    const uint8_t *source = s_display_frames[active_frame].data[line];
    size_t transfer_index = 0;

    for (int count_h = HORIZONTAL_BYTES_MAX - 1; count_h >= 0; --count_h) {
        uint8_t byte_to_send = source[count_h] >> 4;
        if (count_h > 0) {
            byte_to_send |= (uint8_t)(source[count_h - 1] << 4);
        }
        transfer[transfer_index++] = byte_to_send;
    }

    transfer[transfer_index] = (uint8_t)(((line / 10U) << 4) | (line % 10U));
    HAL_SPI__SendBuffer(transfer, sizeof(transfer));
    HAL_PIO__DisplayLatch(PIN_ON);
    HAL_PIO__DisplayLatch(PIN_OFF);
}

static void Display_Task(void *context)
{
    (void)context;
    uint8_t line = 0;

    while (true) {
        uint32_t notification = 0;
        (void)xTaskNotifyWait(0, UINT32_MAX, &notification, portMAX_DELAY);

        if (!atomic_load_explicit(&s_display_enabled, memory_order_acquire)) {
            continue;
        }
        if (atomic_exchange_explicit(&s_reset_scan, false,
                                     memory_order_acq_rel)) {
            line = 0;
        }

        (void)xSemaphoreTake(s_display_lock, portMAX_DELAY);
        if (atomic_load_explicit(&s_display_enabled, memory_order_acquire)) {
            Display_SendLine(line);
            ++line;

            if (line >= VERTICAL_LINES_MAX) {
                line = 0;
                if (atomic_exchange_explicit(&s_frame_pending, false,
                                             memory_order_acq_rel)) {
                    const uint8_t active_frame =
                        atomic_load_explicit(&s_active_frame,
                                             memory_order_relaxed);
                    atomic_store_explicit(&s_active_frame,
                                          (uint8_t)(active_frame ^ 1U),
                                          memory_order_release);
                }
                atomic_store_explicit(&s_frame_end, true,
                                      memory_order_release);
            }
        }
        (void)xSemaphoreGive(s_display_lock);
    }
}

static bool IRAM_ATTR Display_TimerAlarm(gptimer_handle_t timer,
                                        const gptimer_alarm_event_data_t *event,
                                        void *context)
{
    (void)timer;
    (void)event;
    BaseType_t high_priority_task_woken = pdFALSE;

    (void)xTaskNotifyFromISR((TaskHandle_t)context,
                            DISPLAY_TASK_NOTIFICATION,
                            eSetBits,
                            &high_priority_task_woken);
    return high_priority_task_woken == pdTRUE;
}

static void Sound_Stop(void *context)
{
    (void)context;
    (void)ledc_stop(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL, 0);
}

static bool Sound_StopBlocking(void)
{
    if (s_sound_stop_timer == NULL) {
        return true;
    }

    bool success = esp_timer_stop_blocking(s_sound_stop_timer,
                                           portMAX_DELAY) == ESP_OK;
    if (ledc_stop(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL, 0) != ESP_OK) {
        success = false;
    }
    return success;
}

void Interrupt__Setup(void)
{
    if (s_initialized) {
        if (s_display_timer_running) {
            return;
        }

        atomic_store_explicit(&s_frame_end, false, memory_order_release);
        atomic_store_explicit(&s_frame_pending, false, memory_order_release);
        atomic_store_explicit(&s_reset_scan, true, memory_order_release);
        atomic_store_explicit(&s_display_enabled, true, memory_order_release);
        ESP_ERROR_CHECK(gptimer_set_raw_count(s_display_timer, 0));
        ESP_ERROR_CHECK(gptimer_start(s_display_timer));
        s_display_timer_running = true;
        return;
    }

    s_display_lock = xSemaphoreCreateMutex();
    ESP_ERROR_CHECK(s_display_lock != NULL ? ESP_OK : ESP_ERR_NO_MEM);

    const BaseType_t task_result = xTaskCreate(Display_Task,
                                               "display_scan",
                                               DISPLAY_TASK_STACK_SIZE,
                                               NULL,
                                               DISPLAY_TASK_PRIORITY,
                                               &s_display_task);
    ESP_ERROR_CHECK(task_result == pdPASS ? ESP_OK : ESP_ERR_NO_MEM);

    const gptimer_config_t display_timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = DISPLAY_TIMER_RESOLUTION_HZ,
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&display_timer_config, &s_display_timer));

    const gptimer_event_callbacks_t display_timer_callbacks = {
        .on_alarm = Display_TimerAlarm,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(s_display_timer,
                                                      &display_timer_callbacks,
                                                      s_display_task));

    const gptimer_alarm_config_t display_alarm_config = {
        .alarm_count = DISPLAY_LINE_PERIOD_TICKS,
        .reload_count = 0,
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(s_display_timer,
                                              &display_alarm_config));
    ESP_ERROR_CHECK(gptimer_enable(s_display_timer));

    const ledc_timer_config_t buzzer_timer_config = {
        .speed_mode = BUZZER_LEDC_MODE,
        .duty_resolution = BUZZER_LEDC_DUTY_RESOLUTION,
        .timer_num = BUZZER_LEDC_TIMER,
        .freq_hz = BUZZER_INITIAL_FREQUENCY_HZ,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&buzzer_timer_config));

    const ledc_channel_config_t buzzer_channel_config = {
        .gpio_num = BUZZER_GPIO,
        .speed_mode = BUZZER_LEDC_MODE,
        .channel = BUZZER_LEDC_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = BUZZER_LEDC_TIMER,
        .duty = 0,
        .hpoint = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&buzzer_channel_config));

    const esp_timer_create_args_t sound_timer_config = {
        .callback = Sound_Stop,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "sound_stop",
    };
    ESP_ERROR_CHECK(esp_timer_create(&sound_timer_config, &s_sound_stop_timer));

    atomic_store_explicit(&s_frame_end, false, memory_order_release);
    atomic_store_explicit(&s_frame_pending, false, memory_order_release);
    atomic_store_explicit(&s_reset_scan, true, memory_order_release);
    atomic_store_explicit(&s_display_enabled, true, memory_order_release);
    ESP_ERROR_CHECK(gptimer_start(s_display_timer));

    s_display_timer_running = true;
    s_initialized = true;
}

bool Interrupt__DisableAll(void)
{
    if (!s_initialized) {
        return true;
    }

    bool success = true;
    atomic_store_explicit(&s_display_enabled, false, memory_order_release);

    if (s_display_timer_running) {
        const esp_err_t timer_result = gptimer_stop(s_display_timer);
        if (timer_result != ESP_OK && timer_result != ESP_ERR_INVALID_STATE) {
            success = false;
        }
        s_display_timer_running = false;
    }

    (void)xTaskNotify(s_display_task, DISPLAY_TASK_NOTIFICATION, eSetBits);
    if (xSemaphoreTake(s_display_lock, portMAX_DELAY) == pdTRUE) {
        (void)xSemaphoreGive(s_display_lock);
    } else {
        success = false;
    }

    if (!Sound_StopBlocking()) {
        success = false;
    }
    atomic_store_explicit(&s_frame_end, true, memory_order_release);
    return success;
}

bool Interrupt__IsFrameEnd(void)
{
    return atomic_exchange_explicit(&s_frame_end, false,
                                    memory_order_acq_rel);
}

void Interrupt__ShowFrame(DisplayFrame_t *display_frame)
{
    if (display_frame == NULL) {
        return;
    }

    while (atomic_load_explicit(&s_display_enabled, memory_order_acquire) &&
           !Interrupt__IsFrameEnd()) {
        vTaskDelay(1);
    }
    if (!atomic_load_explicit(&s_display_enabled, memory_order_acquire)) {
        return;
    }

    const uint8_t active_frame = atomic_load_explicit(&s_active_frame,
                                                       memory_order_acquire);
    memcpy(&s_display_frames[active_frame ^ 1U], display_frame,
           sizeof(*display_frame));
    atomic_store_explicit(&s_frame_pending, true, memory_order_release);
}

char Interrupt__GetUART1RX(void)
{
    return getch();
}

void Interrupt__PlaySound(uint16_t freq_hz, uint16_t time_ms)
{
    if (!s_initialized) {
        return;
    }

    if (esp_timer_stop_blocking(s_sound_stop_timer, portMAX_DELAY) != ESP_OK) {
        (void)ledc_stop(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL, 0);
        return;
    }
    if (freq_hz == 0 || time_ms == 0) {
        (void)ledc_stop(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL, 0);
        return;
    }

    if (ledc_set_freq(BUZZER_LEDC_MODE, BUZZER_LEDC_TIMER, freq_hz) != ESP_OK ||
        ledc_set_duty(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL,
                      BUZZER_LEDC_HALF_DUTY) != ESP_OK ||
        ledc_update_duty(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL) != ESP_OK) {
        (void)ledc_stop(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL, 0);
        return;
    }

    const uint64_t duration_us = (uint64_t)time_ms * 1000U;
    if (esp_timer_start_once(s_sound_stop_timer, duration_us) != ESP_OK) {
        (void)ledc_stop(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL, 0);
    }
}
