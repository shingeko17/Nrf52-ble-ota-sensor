/**
 * @file led_status.h
 * @brief LED status indicators
 * @author Buzz - HCMUT
 */

#ifndef LED_STATUS_H
#define LED_STATUS_H

/** LED state machine */
typedef enum {
	LED_STATE_IDLE,         /**< All LEDs off */
	LED_STATE_ADVERTISING,  /**< LED1 slow blink */
	LED_STATE_CONNECTED,    /**< LED2 solid */
	LED_STATE_OTA,          /**< LED3 fast blink */
	LED_STATE_ERROR,        /**< LED4 solid */
} led_state_t;

/**
 * @brief Initialize LED GPIOs
 * @return 0 on success, negative errno on failure
 */
int led_status_init(void);

/**
 * @brief Set LED state
 * @param state New LED state
 */
void led_status_set(led_state_t state);

#endif /* LED_STATUS_H */
