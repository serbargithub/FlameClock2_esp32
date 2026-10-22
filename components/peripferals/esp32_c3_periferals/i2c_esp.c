#include "../i2c_HAL.h"
#include "io_ports_names_esp.h"

static i2c_master_bus_handle_t s_i2c_bus;

esp_err_t HAL_I2C__Init(void)
{
    if (s_i2c_bus != NULL) {
        return ESP_OK;
    }

    const i2c_master_bus_config_t bus_config = {
        /* I2C1 is the board name; ESP32-C3 has only controller 0. */
        .i2c_port = I2C_NUM_0,
        .sda_io_num = I2C1_SDA_GPIO,
        .scl_io_num = I2C1_SCL_GPIO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    i2c_master_bus_handle_t bus = NULL;
    const esp_err_t result = i2c_new_master_bus(&bus_config, &bus);
    if (result == ESP_OK) {
        s_i2c_bus = bus;
    }
    return result;
}

esp_err_t HAL_I2C__TurnOff(void)
{
    if (s_i2c_bus == NULL) {
        return ESP_OK;
    }

    const esp_err_t result = i2c_del_master_bus(s_i2c_bus);
    if (result == ESP_OK) {
        s_i2c_bus = NULL;
    }
    return result;
}

esp_err_t HAL_I2C__AddDevice(uint8_t address, uint32_t speed_hz,
                            HAL_I2C_DeviceHandle_t *device)
{
    if (device == NULL || address < 0x08 || address > 0x77 ||
        speed_hz == 0 || speed_hz > 400000U) {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_i2c_bus == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    const i2c_device_config_t device_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = address,
        .scl_speed_hz = speed_hz,
    };
    HAL_I2C_DeviceHandle_t new_device = NULL;
    const esp_err_t result = i2c_master_bus_add_device(s_i2c_bus,
                                                      &device_config,
                                                      &new_device);
    if (result == ESP_OK) {
        *device = new_device;
    }
    return result;
}

esp_err_t HAL_I2C__RemoveDevice(HAL_I2C_DeviceHandle_t *device)
{
    if (device == NULL || *device == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_i2c_bus == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    const esp_err_t result = i2c_master_bus_rm_device(*device);
    if (result == ESP_OK) {
        *device = NULL;
    }
    return result;
}

esp_err_t HAL_I2C__Probe(uint8_t address, int timeout_ms)
{
    if (address < 0x08 || address > 0x77 || timeout_ms < -1) {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_i2c_bus == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    return i2c_master_probe(s_i2c_bus, address, timeout_ms);
}

esp_err_t HAL_I2C__Write(HAL_I2C_DeviceHandle_t device,
                        const uint8_t *data, size_t length, int timeout_ms)
{
    if (device == NULL || data == NULL || length == 0 || timeout_ms < -1) {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_i2c_bus == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    return i2c_master_transmit(device, data, length, timeout_ms);
}

esp_err_t HAL_I2C__Read(HAL_I2C_DeviceHandle_t device,
                       uint8_t *data, size_t length, int timeout_ms)
{
    if (device == NULL || data == NULL || length == 0 || timeout_ms < -1) {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_i2c_bus == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    return i2c_master_receive(device, data, length, timeout_ms);
}

esp_err_t HAL_I2C__WriteRead(HAL_I2C_DeviceHandle_t device,
                            const uint8_t *write_data, size_t write_length,
                            uint8_t *read_data, size_t read_length,
                            int timeout_ms)
{
    if (device == NULL || write_data == NULL || write_length == 0 ||
        read_data == NULL || read_length == 0 || timeout_ms < -1) {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_i2c_bus == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    return i2c_master_transmit_receive(device, write_data, write_length,
                                       read_data, read_length, timeout_ms);
}
