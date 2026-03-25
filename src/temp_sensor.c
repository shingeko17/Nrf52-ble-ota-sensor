/**
 * @file temp_sensor.c
 * @brief nRF52840 onboard temperature sensor (TEMP peripheral)
 * @author Buzz - HCMUT
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include "app_config.h"
#include "temp_sensor.h"

LOG_MODULE_REGISTER(temp_sensor, LOG_LEVEL_INF);

static const struct device *temp_dev;

int temp_sensor_init(void)
{
	temp_dev = DEVICE_DT_GET(DT_NODELABEL(temp));

	if (!device_is_ready(temp_dev)) {
		LOG_ERR("Temperature sensor not ready");
		return -ENODEV;
	}

	LOG_INF("Temperature sensor initialized");
	return 0;
}

int16_t temp_sensor_read(void)
{
	struct sensor_value val;
	int ret;

	if (!temp_dev) {
		return 0;
	}

	ret = sensor_sample_fetch(temp_dev);
	if (ret) {
		LOG_ERR("Sensor fetch failed: %d", ret);
		return 0;
	}

	ret = sensor_channel_get(temp_dev, SENSOR_CHAN_DIE_TEMP, &val);
	if (ret) {
		LOG_ERR("Sensor get failed: %d", ret);
		return 0;
	}

	/* Convert to int16 (°C × 100) */
	int16_t temp = (int16_t)(val.val1 * TEMP_SCALE_FACTOR +
				  val.val2 / (1000000 / TEMP_SCALE_FACTOR));

	LOG_DBG("Temperature: %d.%02d C", temp / 100, temp % 100);
	return temp;
}
