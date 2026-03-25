/**
 * @file temp_sensor.h
 * @brief Onboard temperature sensor interface
 * @author Buzz - HCMUT
 */

#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <stdint.h>

/**
 * @brief Initialize the temperature sensor
 * @return 0 on success, negative errno on failure
 */
int temp_sensor_init(void);

/**
 * @brief Read current temperature
 * @return Temperature in °C × 100 (e.g., 2350 = 23.50°C)
 */
int16_t temp_sensor_read(void);

#endif /* TEMP_SENSOR_H */
