/**
 * @file app_config.h
 * @brief Application configuration defines
 * @author Buzz - HCMUT
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* Application version */
#define APP_VERSION_MAJOR 1
#define APP_VERSION_MINOR 0
#define APP_VERSION_PATCH 0
#define APP_VERSION_STRING "1.0.0"

/* BLE Configuration */
#define BLE_DEVICE_NAME "nRF52_OTA_Sensor"
#define BLE_ADV_INTERVAL_MIN 160  /* 100ms (160 * 0.625ms) */
#define BLE_ADV_INTERVAL_MAX 320  /* 200ms (320 * 0.625ms) */

/* Custom Service UUIDs */
#define CUSTOM_SERVICE_UUID_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0)
#define TEMP_CHAR_UUID_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1)
#define BUTTON_CHAR_UUID_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef2)

/* Temperature sensor */
#define TEMP_SAMPLE_INTERVAL_MS 2000  /* Read every 2 seconds */
#define TEMP_SCALE_FACTOR       100   /* °C × 100 for int16 */

/* LED Configuration */
#define LED_ADV_BLINK_MS     1000  /* Slow blink: advertising */
#define LED_OTA_BLINK_MS     200   /* Fast blink: OTA in progress */

/* Power Management */
#define IDLE_TIMEOUT_MS      30000  /* Enter low power after 30s idle */

#endif /* APP_CONFIG_H */
