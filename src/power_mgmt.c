/**
 * @file power_mgmt.c
 * @brief Zephyr power management - automatic idle sleep
 * @author Buzz - HCMUT
 */

#include <zephyr/kernel.h>
#include <zephyr/pm/pm.h>
#include <zephyr/pm/policy.h>
#include <zephyr/logging/log.h>

#include "power_mgmt.h"

LOG_MODULE_REGISTER(power_mgmt, LOG_LEVEL_INF);

int power_mgmt_init(void)
{
	/*
	 * Zephyr handles idle power management automatically when
	 * CONFIG_PM=y is set. The kernel enters low-power states
	 * (System ON sleep / System OFF) when all threads are idle.
	 *
	 * nRF52840 supports:
	 * - System ON (WFI): ~1.5uA base + wake on any interrupt
	 * - System OFF: ~0.3uA, wake on GPIO/NFC/LPCOMP
	 *
	 * We use System ON sleep since BLE needs periodic wakeup.
	 */

	LOG_INF("Power management initialized (automatic idle sleep)");
	return 0;
}
