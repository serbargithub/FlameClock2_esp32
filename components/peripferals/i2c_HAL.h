#ifndef I2C_HAL_H
#define I2C_HAL_H

#include <stddef.h>
#include <stdint.h>

#include "driver/i2c_master.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HAL_I2C_DEFAULT_SPEED_HZ  100000U
#define HAL_I2C_DEFAULT_TIMEOUT_MS  100

typedef i2c_master_dev_handle_t HAL_I2C_DeviceHandle_t;

/* Board I2C1: GPIO18 SDA, GPIO19 SCL; ESP32-C3 controller I2C_NUM_0.
 * Call Init explicitly before adding/probing devices. These pins are shared
 * with native USB Serial/JTAG, which cannot be used while I2C owns them.
 * Internal pull-ups are enabled; use external pull-ups to 3.3 V for reliability.
 * All calls are task-context only. Serialize initialization, device add/remove,
 * and shutdown against all other calls. Transfers use the driver's bus lock.
 * Functions return ESP-IDF errors, including NACK/timeout, without aborting.
 */
esp_err_t HAL_I2C__Init(void);

/* Remove every device before TurnOff; otherwise ESP_ERR_INVALID_STATE.
 * Repeated Init/TurnOff calls succeed when already initialized/off.
 */
esp_err_t HAL_I2C__TurnOff(void);

/* address is an unshifted 7-bit device address (0x08..0x77).
 * speed_hz must be nonzero and at most 400000; normally use 100000.
 * AddDevice registers a device; use Probe to check that it acknowledges.
 * RemoveDevice clears the caller's handle on success. Do not reuse copies.
 */
esp_err_t HAL_I2C__AddDevice(uint8_t address, uint32_t speed_hz,
                            HAL_I2C_DeviceHandle_t *device);
esp_err_t HAL_I2C__RemoveDevice(HAL_I2C_DeviceHandle_t *device);
esp_err_t HAL_I2C__Probe(uint8_t address, int timeout_ms);

/* Blocking transfers: buffers must be non-NULL and sizes nonzero.
 * timeout_ms is in milliseconds; -1 waits indefinitely.
 * WriteRead sends a repeated START between write and read (register reads).
 * Use only handles returned by AddDevice; do not register async callbacks.
 */
esp_err_t HAL_I2C__Write(HAL_I2C_DeviceHandle_t device,
                        const uint8_t *data, size_t length, int timeout_ms);
esp_err_t HAL_I2C__Read(HAL_I2C_DeviceHandle_t device,
                       uint8_t *data, size_t length, int timeout_ms);
esp_err_t HAL_I2C__WriteRead(HAL_I2C_DeviceHandle_t device,
                            const uint8_t *write_data, size_t write_length,
                            uint8_t *read_data, size_t read_length,
                            int timeout_ms);

#ifdef __cplusplus
}
#endif

#endif /* I2C_HAL_H */
