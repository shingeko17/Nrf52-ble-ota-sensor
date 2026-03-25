/**
 * @file button_handler.h
 * @brief Button events handler
 * @author Buzz - HCMUT
 */

#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize button GPIOs with interrupt callbacks
 * @return 0 on success, negative errno on failure
 */
int button_handler_init(void);

#endif /* BUTTON_HANDLER_H */
