/*
 * Copyright (c) 2025 Grovety Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <zephyr/drivers/gpio.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

    static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

    /**
     * @brief Initializes an LED.
     *
     * @param led Pointer to the LED to initialize.
     *
     * @return 0 if success, error code if failure.
     */
    int led_init(const struct gpio_dt_spec *led);

    /**
     * @brief Blinks an LED for a specified duration.
     *
     * @param led Pointer to the LED to blink.
     * @param turn_on_time Duration for which the LED stays on, in milliseconds.
     */
    void blink_led(const struct gpio_dt_spec *led, uint32_t turn_on_time);

#ifdef __cplusplus
}
#endif
