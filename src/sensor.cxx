/*
 * Copyright (c) 2025 Grovety Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <drivers/bme68x_iaq.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(app, CONFIG_APP_LOG_LEVEL);

#include "battery.h"
#include "sensor.hxx"

CSensor::CSensor()
{
}

CSensor::~CSensor()
{
}

int CSensor::init()
{
    bme_sensor = DEVICE_DT_GET(DT_INST(0, bosch_bme680));

    if (!bme_sensor)
    {
        LOG_ERR("Failed to get device Bosch BME68X");
        return -1;
    }
    if (!device_is_ready(bme_sensor))
    {
        LOG_ERR("Bosch BME68X is not ready for use");
        return -1;
    }

    int err = battery_measure_enable(true);
    if (err)
    {
        LOG_ERR("Failed to initialize battery measurement: %d", err);
        return err;
    }

    return 0;
}

int CSensor::update_measurements()
{
    if (!bme_sensor)
    {
        LOG_ERR("Sensor Bosch BME68X is not initialized");
        return -1;
    }

    int err = 0;

    err = sensor_sample_fetch(bme_sensor);
    if (err)
    {
        LOG_ERR("Failed to fetch sensor sample: %d", err);
        return err;
    }
    err = sensor_channel_get(bme_sensor, SENSOR_CHAN_AMBIENT_TEMP, &temp);
    if (err)
    {
        LOG_ERR("Failed to fetch temperature sensor data: %d", err);
        return err;
    }
    err = sensor_channel_get(bme_sensor, SENSOR_CHAN_PRESS, &press);
    if (err)
    {
        LOG_ERR("Failed to fetch pressure sensor data: %d", err);
        return err;
    }
    err = sensor_channel_get(bme_sensor, SENSOR_CHAN_HUMIDITY, &humidity);
    if (err)
    {
        LOG_ERR("Failed to fetch humidity sensor data: %d", err);
        return err;
    }
    err = sensor_channel_get(bme_sensor, static_cast<sensor_channel>(SENSOR_CHAN_IAQ), &iaq);
    if (err)
    {
        LOG_ERR("Failed to fetch IAQ sensor data: %d", err);
        return err;
    }
    err = sensor_channel_get(bme_sensor, SENSOR_CHAN_CO2, &co2);
    if (err)
    {
        LOG_ERR("Failed to fetch CO2 sensor data: %d", err);
        return err;
    }
    err = sensor_channel_get(bme_sensor, SENSOR_CHAN_VOC, &voc);
    if (err)
    {
        LOG_ERR("Failed to fetch VOC sensor data: %d", err);
        return err;
    }

    LOG_INF("temp: %d.%06d; press: %d.%06d; humidity: %d.%06d; iaq: %d; CO2: %d.%06d; "
            "VOC: %d.%06d; battery_percent: %.2f",
            temp.val1, temp.val2, press.val1, press.val2, humidity.val1, humidity.val2,
            iaq.val1, co2.val1, co2.val2, voc.val1, voc.val2, get_battery_percent());

    return err;
}

float CSensor::get_temperature() const
{
    float value = sensor_value_to_float(&temp);
    if (value > 85.0f)
    {
        value = 85.0f;
    }
    else if (value < -40.0f)
    {
        value = -40.0f;
    }
    return value;
}

float CSensor::get_humidity() const
{
    float value = sensor_value_to_float(&humidity);
    if (value > 100.0f)
    {
        value = 100.0f;
    }
    else if (value < 0.0f)
    {
        value = 0.0f;
    }
    return value;
}

float CSensor::get_pressure() const
{
    float value = sensor_value_to_float(&press);
    if (value > 110000.0f)
    {
        value = 110000.0f;
    }
    else if (value < 30000.0f)
    {
        value = 30000.0f;
    }
    return value;
}

float CSensor::get_co2() const
{
    return sensor_value_to_float(&co2);
}

float CSensor::get_voc() const
{
    return sensor_value_to_float(&voc);
}

uint16_t CSensor::get_iaq() const
{
    float value = sensor_value_to_float(&iaq);
    if (value > 500.0f)
    {
        value = 500.0f;
    }
    else if (value < 0.0f)
    {
        value = 0.0f;
    }
    return static_cast<uint16_t>(value);
}

float CSensor::get_battery_percent() const
{
    int battery = battery_sample();

    float max = 3000;
    float min = 2000;

    float batteryLevel = static_cast<float>(battery - min) / static_cast<float>(max - min) * 100.0F;
    if (batteryLevel > 100)
        batteryLevel = 100;

    if (batteryLevel < 0)
        batteryLevel = 0;

    return batteryLevel;
}
