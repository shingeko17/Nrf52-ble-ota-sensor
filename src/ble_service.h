/**
 * @file ble_service.h
 * @brief BLE GATT service for sensor data
 * @author Buzz - HCMUT
 */

#ifndef BLE_SERVICE_H
#define BLE_SERVICE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize BLE subsystem and register GATT services
 * @return 0 on success, negative errno on failure
 */
int ble_service_init(void);

/**
 * @brief Start BLE advertising
 * @return 0 on success, negative errno on failure
 */
int ble_service_start_adv(void);

/**
 * @brief Check if a BLE central is connected
 * @return true if connected
 */
bool ble_service_is_connected(void);

/**
 * @brief Send temperature notification to connected central
 * @param temp_value Temperature in °C × 100
 * @return 0 on success, negative errno on failure
 */
int ble_service_notify_temp(int16_t temp_value);

/**
 * @brief Send button event notification
 * @param button_id Button number (0-3)
 * @param pressed true if pressed, false if released
 * @return 0 on success, negative errno on failure
 */
int ble_service_notify_button(uint8_t button_id, bool pressed);

#endif /* BLE_SERVICE_H */
