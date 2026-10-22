/*
 * icm42607.h - Register-level driver for TDK InvenSense ICM-42607-P (I2C)
 * Register-level driver for ICM-42607-P sensor
 * Protocol : I2C
 * Target   : ESP32-S3
 */
#ifndef ICM42607_H
#define ICM42607_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "driver/i2c_master.h"

// ============== I2C address (Pulled-low) & WhoAmI value ==============

#define ICM42607_I2C_ADDR_LOW              0x68
#define ICM42607_WHO_AM_I_VAL              0x60


// ============== Register map ==============

#define ICM42607_REG_SIGNAL_PATH_RESET      0x02
#define ICM42607_REG_ACCEL_DATA_X1          0x0B
#define ICM42607_REG_PWR_MGMT0              0x1F
#define ICM42607_REG_ACCEL_CONFIG0          0x21
#define ICM42607_REG_WHO_AM_I               0x75


// ============== Bit masks ==============

// Gyrosensor mode - PWR_MGMT0
#define ICM42607_GYRO_MODE_OFF              (0x00 << 2)

// Accelerometer modes - ACCEL_CONFIG0
#define ICM42607_ACCEL_MODE_LN              (0x03 << 0) // Low-Noise Mode

// Configuration of accelerometer - ACCEL_CONFIG0 (0x21)
#define ICM42607_ACCEL_FS_8G                (0x01 << 5)
#define ICM42607_ACCEL_ODR_1600HZ           (0x05)

// Reset mask - SIGNAL_PATH_RESET (0x02)
#define ICM42607_SOFT_RESET_BIT             (1 << 4)


// ============== Data structures ==============

typedef struct
{
    i2c_master_dev_handle_t i2c_dev;
} icm42607_dev_t;

typedef struct
{
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    int16_t temp;
} icm42607_raw_data_t;


// ============== Function prototypes ==============

// Read a single value from 'reg_addr'
esp_err_t icm42607_read_reg(icm42607_dev_t *dev, uint8_t reg_addr, uint8_t *data);

// Write a single value at 'reg_addr'
esp_err_t icm42607_write_reg(icm42607_dev_t *dev, uint8_t reg_addr, uint8_t data);

// Read 'len' consecutive registers starting from 'start_reg'
esp_err_t icm42607_read_bytes(icm42607_dev_t *dev, uint8_t start_reg, uint8_t *buffer, size_t len);

// Initialize ICM-42607-P, verify WHO_AM_I, and soft reset device.
esp_err_t icm42607_init(icm42607_dev_t *dev);

// Enable Accelerometer and turn off Gyrosensor
esp_err_t icm42607_enable_accelerometer(icm42607_dev_t *dev);

// Read raw Accelerometer data
esp_err_t icm42607_read_raw_data(icm42607_dev_t *dev, icm42607_raw_data_t *raw_data);

#endif /* ICM42607_H */