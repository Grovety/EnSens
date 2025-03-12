/*
 * Copyright (c) 2025 Grovety Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/usb/usb_device.h>

#include "ble.h"
#include "led.h"
#include "sensor.hxx"

LOG_MODULE_REGISTER(app, CONFIG_APP_LOG_LEVEL);

int main(void)
{
    int err = 0;

    LOG_INF("Firmware source code can be found here https://github.com/Grovety/EnSens");

    if (IS_ENABLED(CONFIG_USB_DEVICE_STACK))
    {
        err = usb_enable(NULL);
        if (err)
        {
            LOG_ERR("USB enable failed (err %d)", err);
            return err;
        }
    }

    err = led_init(&led);
    if (err)
    {
        return err;
    }

    CSensor sensor;
    err = sensor.init();
    if (err)
    {
        return err;
    }

    err = bt_enable(NULL);
    if (err)
    {
        LOG_ERR("Bluetooth enable failed (err %d)", err);
        return err;
    }
    err = bt_init();
    if (err)
    {
        LOG_ERR("Bluetooth init failed (err %d)", err);
        return err;
    }

    while (true)
    {
        blink_led(&led, 100);
        err = sensor.update_measurements();
        if (err)
        {
            LOG_ERR("Failed to update measurements (err %d)", err);
            continue;
        }
        bt_set_temperature(sensor.get_temperature());
        bt_set_humidity(sensor.get_humidity());
        bt_set_pressure(sensor.get_pressure());
        bt_set_co2(sensor.get_co2());
        bt_set_voc(sensor.get_voc());
        bt_set_iaq(sensor.get_iaq());
        bt_set_battery(sensor.get_battery_percent());
        update_advertise_data();
        k_sleep(K_MSEC(3000)); // 3 s
    }

    return 0;
}
