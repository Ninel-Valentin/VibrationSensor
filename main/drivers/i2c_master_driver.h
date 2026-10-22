#ifndef I2C_MASTER_DRIVER_H
#define I2C_MASTER_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "driver/i2c_master.h"

/**
 * @brief Initialize the I2C master bus and add the ICM42607 device to it.
 * 
 * @param[out] i2c_dev_out Pointer to receive the created i2c_master_dev_handle_t
 * @param[in]  i2c_port    I2C port number (e.g., I2C_NUM_0)
 * @param[in]  sda_io      SDA GPIO pin number
 * @param[in]  scl_io      SCL GPIO pin number
 * @param[in]  clk_speed   I2C clock speed in Hz (e.g., 400000)
 * @param[in]  dev_addr    I2C 7-bit device address (e.g., ICM42607_I2C_ADDR_LOW)
 * @return esp_err_t ESP_OK on success, or an error code on failure
 */
esp_err_t i2c_master_init_device(i2c_master_dev_handle_t *i2c_dev_out, 
                                 i2c_port_t i2c_port, 
                                 int sda_io, 
                                 int scl_io, 
                                 uint32_t clk_speed, 
                                 uint8_t dev_addr);

#endif /* I2C_MASTER_DRIVER_H */