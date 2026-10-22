#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"

#include "config.h"
#include "i2c_master_driver.h" // Include the new standalone I2C master driver
#include "icm42607.h"

static esp_err_t i2c_master_init(icm42607_dev_t *dev)
{
    return i2c_master_init_device(
        &dev->i2c_dev,
        I2C_MASTER_NUM,
        PIN_SDA,
        PIN_SCL,
        I2C_MASTER_FREQ_HZ,
        ICM42607_I2C_ADDR_LOW
    );
}

void app_main(void)
{
    icm42607_dev_t dev = {0};
    ESP_ERROR_CHECK(i2c_master_init(&dev));

    if (icm42607_init(&dev) == ESP_OK) {
        printf("ICM-42607-P initialized successfully!\n");
        ESP_ERROR_CHECK(icm42607_enable_accelerometer(&dev));

        icm42607_raw_data_t raw;
        for (;;) {
            if (icm42607_read_raw_data(&dev, &raw) == ESP_OK) {
                printf("RAW Accel: X=%d Y=%d Z=%d\n", raw.accel_x, raw.accel_y, raw.accel_z);
            } else 
                printf("Not working!\n");
            vTaskDelay(pdMS_TO_TICKS(200));
        }
    }
    else {
        printf("Failed to find ICM-42607-P!\n");
    }
}