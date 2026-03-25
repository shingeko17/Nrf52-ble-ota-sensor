/**
 * @file main.c
 * @brief Application entry point - nRF52840 BLE Sensor with OTA
 * @author Buzz - HCMUT
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "app_config.h"
#include "ble_service.h"
#include "temp_sensor.h"
#include "button_handler.h"
#include "led_status.h"
#include "power_mgmt.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	int ret;

	LOG_INF("nRF52 BLE OTA Sensor v%s starting...", APP_VERSION_STRING);

	/* Initialize subsystems */
	ret = led_status_init();
	if (ret) {
		LOG_ERR("LED init failed: %d", ret);
		return ret;
	}

	ret = button_handler_init();
	if (ret) {
		LOG_ERR("Button init failed: %d", ret);
		return ret;
	}

	ret = temp_sensor_init();
	if (ret) {
		LOG_ERR("Temp sensor init failed: %d", ret);
		return ret;
	}

	ret = ble_service_init();
	if (ret) {
		LOG_ERR("BLE init failed: %d", ret);
		return ret;
	}

	ret = power_mgmt_init();
	if (ret) {
		LOG_ERR("Power mgmt init failed: %d", ret);
		return ret;
	}

	/* Start BLE advertising */
	ret = ble_service_start_adv();
	if (ret) {
		LOG_ERR("BLE advertising failed: %d", ret);
		return ret;
	}

	LOG_INF("All systems initialized. Advertising as \"%s\"",
		BLE_DEVICE_NAME);

	/* Main loop: read temperature + update BLE */
	while (1) {
		int16_t temp = temp_sensor_read();

		if (ble_service_is_connected()) {
			ble_service_notify_temp(temp);
		}

		k_msleep(TEMP_SAMPLE_INTERVAL_MS);
	}

	return 0;
}
