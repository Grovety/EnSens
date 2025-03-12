/*
 * Copyright (c) 2025 Grovety Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(app, CONFIG_APP_LOG_LEVEL);

#include "led.h"

int led_init(const struct gpio_dt_spec *led)
{
	int err = 0;
	if (!gpio_is_ready_dt(led))
	{
		LOG_ERR("LED GPIO port is not ready");
		return -1;
	}
	err = gpio_pin_configure_dt(led, GPIO_OUTPUT_INACTIVE);
	if (err)
	{
		LOG_ERR("Failed to configure LED pin (err %d)", err);
		return err;
	}

	return err;
}

void blink_led(const struct gpio_dt_spec *led, uint32_t turn_on_time)
{
	gpio_pin_set_dt(led, 1);
	k_sleep(K_MSEC(turn_on_time));
	gpio_pin_set_dt(led, 0);
}
