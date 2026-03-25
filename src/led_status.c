/**
 * @file led_status.c
 * @brief LED status indicator implementation
 * @author Buzz - HCMUT
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "app_config.h"
#include "led_status.h"

LOG_MODULE_REGISTER(led_status, LOG_LEVEL_INF);

/* nRF52840 DK has 4 LEDs */
#define NUM_LEDS 4

static const struct gpio_dt_spec leds[NUM_LEDS] = {
	GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios),
};

static led_state_t current_state;
static struct k_timer blink_timer;

static void all_leds_off(void)
{
	for (int i = 0; i < NUM_LEDS; i++) {
		gpio_pin_set_dt(&leds[i], 0);
	}
}

static void blink_handler(struct k_timer *timer)
{
	static bool toggle;

	toggle = !toggle;

	switch (current_state) {
	case LED_STATE_ADVERTISING:
		gpio_pin_set_dt(&leds[0], toggle);  /* LED1 blink */
		break;
	case LED_STATE_OTA:
		gpio_pin_set_dt(&leds[2], toggle);  /* LED3 blink */
		break;
	default:
		break;
	}
}

int led_status_init(void)
{
	int ret;

	for (int i = 0; i < NUM_LEDS; i++) {
		if (!gpio_is_ready_dt(&leds[i])) {
			LOG_ERR("LED %d GPIO not ready", i);
			return -ENODEV;
		}

		ret = gpio_pin_configure_dt(&leds[i], GPIO_OUTPUT_INACTIVE);
		if (ret) {
			LOG_ERR("LED %d config failed: %d", i, ret);
			return ret;
		}
	}

	k_timer_init(&blink_timer, blink_handler, NULL);

	LOG_INF("LED status initialized (%d LEDs)", NUM_LEDS);
	return 0;
}

void led_status_set(led_state_t state)
{
	k_timer_stop(&blink_timer);
	all_leds_off();

	current_state = state;

	switch (state) {
	case LED_STATE_IDLE:
		break;
	case LED_STATE_ADVERTISING:
		k_timer_start(&blink_timer,
			      K_MSEC(LED_ADV_BLINK_MS),
			      K_MSEC(LED_ADV_BLINK_MS));
		break;
	case LED_STATE_CONNECTED:
		gpio_pin_set_dt(&leds[1], 1);  /* LED2 solid */
		break;
	case LED_STATE_OTA:
		k_timer_start(&blink_timer,
			      K_MSEC(LED_OTA_BLINK_MS),
			      K_MSEC(LED_OTA_BLINK_MS));
		break;
	case LED_STATE_ERROR:
		gpio_pin_set_dt(&leds[3], 1);  /* LED4 solid */
		break;
	}

	LOG_DBG("LED state: %d", state);
}
