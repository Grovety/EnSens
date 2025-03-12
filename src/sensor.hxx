/*
 * Copyright (c) 2025 Grovety Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

class CSensor
{
public:
   CSensor();
   ~CSensor();

   /**
    * @brief Initializes the hardware sensor used for performing measurements.
    *
    * @return 0 if success, error code if failure.
    */
   int init();

   /**
    * @brief Updates and stores internally the data measured by the sensor
    *  (temperature, pressure, humidity, CO2, VOC, IAQ).
    *
    * @return 0 if success, error code if failure.
    */
   int update_measurements();

   /**
    * @brief Provides the last measured value of temperature.
    *
    * @note Call update_measurements() to update the value.
    *
    * @return Temperature value in degrees Celsius.
    */
   float get_temperature() const;

   /**
    * @brief Provides the last measured value of humidity.
    *
    * @note Call update_measurements() to update the value.
    *
    * @return Relative humidity value in percentage.
    */
   float get_humidity() const;

   /**
    * @brief Provides the last measured value of pressure.
    *
    * @note Call update_measurements() to update the value.
    *
    * @return Pressure value in Pascals.
    */
   float get_pressure() const;

   /**
    * @brief Provides the last measured value of carbon dioxide (CO2).
    *
    * @note Call update_measurements() to update the value.
    *
    * @return CO2 concentration in parts per million (ppm).
    */
   float get_co2() const;

   /**
    * @brief Provides the last measured value of volatile organic compounds (VOC).
    *
    * @note Call update_measurements() to update the value.
    *
    * @return VOC concentration in parts per billion (ppb).
    */
   float get_voc() const;

   /**
    * @brief Provides the last measured value of index for air quality (IAQ).
    *
    * @note Call update_measurements() to update the value.
    *
    * @return Value of the index for air quality.
    */
   uint16_t get_iaq() const;

   /**
    * @brief Provides the battery percent.
    *
    * @return Battery percent.
    */
   float get_battery_percent() const;

private:
   const struct device *bme_sensor;
   struct sensor_value temp, press, humidity, iaq, co2, voc;
};
