/**
 * @file button_handler.c
 * @brief Button press/release detection with BLE notification
 * @author Buzz - HCMUT
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "button_handler.h"
#include "ble_service.h"

LOG_MODULE_REGISTER(button_handler, LOG_LEVEL_INF);

/* DK has 4 buttons */
#define NUM_BUTTONS 4

static const struct gpio_dt_spec buttons[NUM_BUTTONS] = {
	GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(sw2), gpios),
	GPIO_DT_SPEC_GET(DT_ALIAS(sw3), gpios),
};

static struct gpio_callback button_cb_data[NUM_BUTTONS];

/* Work item to handle button in system workqueue (not ISR context) */
struct button_work {
	struct k_work work;
	uint8_t button_id;
	bool pressed;
};

static struct button_work btn_work[NUM_BUTTONS];

static void button_work_handler(struct k_work *work)
{
	struct button_work *bw = CONTAINER_OF(work, struct button_work, work);

	LOG_INF("Button %d %s", bw->button_id,
		bw->pressed ? "pressed" : "released");

	ble_service_notify_button(bw->button_id, bw->pressed);
}

static void button_isr(const struct device *dev, struct gpio_callback *cb,
		       uint32_t pins)
{
	for (int i = 0; i < NUM_BUTTONS; i++) {
		if (pins & BIT(buttons[i].pin)) {
			int val = gpio_pin_get_dt(&buttons[i]);
			btn_work[i].pressed = (val > 0);
			k_work_submit(&btn_work[i].work);
			break;
		}
	}
}

int button_handler_init(void)
{
	int ret;

	for (int i = 0; i < NUM_BUTTONS; i++) {
		if (!gpio_is_ready_dt(&buttons[i])) {
			LOG_ERR("Button %d GPIO not ready", i);
			return -ENODEV;
		}

		ret = gpio_pin_configure_dt(&buttons[i], GPIO_INPUT);
		if (ret) {
			LOG_ERR("Button %d config failed: %d", i, ret);
			return ret;
		}

		ret = gpio_pin_interrupt_configure_dt(&buttons[i],
						      GPIO_INT_EDGE_BOTH);
		if (ret) {
			LOG_ERR("Button %d interrupt config failed: %d", i, ret);
			return ret;
		}

		gpio_init_callback(&button_cb_data[i], button_isr,
				   BIT(buttons[i].pin));
		gpio_add_callback(buttons[i].port, &button_cb_data[i]);

		/* Init work item */
		btn_work[i].button_id = i;
		k_work_init(&btn_work[i].work, button_work_handler);
	}

	LOG_INF("Button handler initialized (%d buttons)", NUM_BUTTONS);
	return 0;
}
