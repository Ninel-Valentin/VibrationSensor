#include "i2c_master_driver.h"

esp_err_t i2c_master_init_device(i2c_master_dev_handle_t *i2c_dev_out, 
                                 i2c_port_t i2c_port, 
                                 int sda_io, 
                                 int scl_io, 
                                 uint32_t clk_speed, 
                                 uint8_t dev_addr)
{
    if (i2c_dev_out == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    // Configure the I2C master bus
    i2c_master_bus_config_t bus_config = {
        .i2c_port = i2c_port,
        .sda_io_num = sda_io,
        .scl_io_num = scl_io,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t bus_handle = NULL;
    esp_err_t ret = i2c_new_master_bus(&bus_config, &bus_handle);
    if (ret != ESP_OK) {
        return ret;
    }

    // Configure the specific I2C target device (ICM42607)
    i2c_device_config_t device_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = dev_addr,
        .scl_speed_hz = clk_speed,
    };

    return i2c_master_bus_add_device(bus_handle, &device_config, i2c_dev_out);
}