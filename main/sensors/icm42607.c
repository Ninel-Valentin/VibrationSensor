#include "icm42607.h"
#include "config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

esp_err_t icm42607_read_reg(icm42607_dev_t *dev, uint8_t reg_addr, uint8_t *data)
{
    return icm42607_read_bytes(dev, reg_addr, data, 1);
}

esp_err_t icm42607_write_reg(icm42607_dev_t *dev, uint8_t reg_addr, uint8_t data)
{
    uint8_t write_buf[2] = {reg_addr, data};
    return i2c_master_transmit(dev->i2c_dev, write_buf, sizeof(write_buf), I2C_TIMEOUT_MS);
}

esp_err_t icm42607_read_bytes(icm42607_dev_t *dev, uint8_t start_reg, uint8_t *buffer, size_t len)
{
    return i2c_master_transmit_receive(dev->i2c_dev, &start_reg, 1, buffer, len, I2C_TIMEOUT_MS);
}

esp_err_t icm42607_init(icm42607_dev_t *dev)
{
    esp_err_t ret;
    uint8_t who_am_i = 0;

    // Check WHO_AM_I id
    ret = icm42607_read_reg(dev, ICM42607_REG_WHO_AM_I, &who_am_i);
    if (ret != ESP_OK || who_am_i != ICM42607_WHO_AM_I_VAL) {
        return ESP_ERR_NOT_FOUND;
    }

    // Soft reset the sensor
    ret = icm42607_write_reg(dev, ICM42607_REG_SIGNAL_PATH_RESET, ICM42607_SOFT_RESET_BIT);
    if (ret != ESP_OK) {
        return ret;
    }

    // Wait 2 ms for proper boot
    vTaskDelay(pdMS_TO_TICKS(2));

    return ESP_OK;
}

esp_err_t icm42607_enable_accelerometer(icm42607_dev_t *dev)
{
    esp_err_t ret;

    // Set accelerometer to full-scale range 
    ret = icm42607_write_reg(dev, ICM42607_REG_ACCEL_CONFIG0, ICM42607_ACCEL_FS_8G | ICM42607_ACCEL_ODR_1600HZ);
    if (ret != ESP_OK) return ret;

    // Turn off Gyrosensor and Enable accelerometer in Low-Noise for continuous monitoring
    ret = icm42607_write_reg(dev, ICM42607_REG_PWR_MGMT0, ICM42607_GYRO_MODE_OFF | ICM42607_ACCEL_MODE_LN);
    if (ret != ESP_OK) return ret;

    return ESP_OK;
}

esp_err_t icm42607_read_raw_data(icm42607_dev_t *dev, icm42607_raw_data_t *raw_data)
{
    uint8_t buffer[6];

    // Burst read 6 bytes starting at ACCEL_DATA_X1 (0x0B) up to ACCEL_DATA_Z0 (0x10)
    esp_err_t ret = icm42607_read_bytes(dev, ICM42607_REG_ACCEL_DATA_X1, buffer, 6);
    if (ret != ESP_OK) {
        return ret;
    }

    /* Convert Big-Endian bytes to signed 16-bit values */
    raw_data->accel_x = (int16_t)((buffer[0] << 8) | buffer[1]);
    raw_data->accel_y = (int16_t)((buffer[2] << 8) | buffer[3]);
    raw_data->accel_z = (int16_t)((buffer[4] << 8) | buffer[5]);

    return ESP_OK;
}