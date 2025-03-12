/*
 * Copyright (c) 2025 Grovety Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <zephyr/bluetooth/bluetooth.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *  @brief GATT Characteristic IAQ UUID Value
 */
#define BT_UUID_GATT_IAQ_VAL 0xDA, 0xA7, 0xBD, 0x48, 0x12, 0x12, 0xBA, 0x82, \
                             0xD6, 0x43, 0x86, 0x12, 0x98, 0x05, 0x89, 0xE2
/**
 *  @brief GATT Characteristic IAQ
 */
#define BT_UUID_GATT_IAQ \
    BT_UUID_DECLARE_128(BT_UUID_GATT_IAQ_VAL)

    /**
     * @brief Initializes the Bluetooth, starts advertising.
     *
     * @return 0 if success, error code if failure.
     */
    int bt_init();

    /**
     * @brief Sets the temperature value for Bluetooth transmission.
     *
     * @param temp Temperature value to set, in degrees Celsius.
     */
    void bt_set_temperature(float temp);

    /**
     * @brief Sets the humidity value for Bluetooth transmission.
     *
     * @param humidity Humidity value to set, in percentage.
     */
    void bt_set_humidity(float humidity);

    /**
     * @brief Sets the pressure value for Bluetooth transmission.
     *
     * @param pressure Pressure value to set, in Pa (Pascals).
     */
    void bt_set_pressure(float pressure);

    /**
     * @brief Sets the CO2 level for Bluetooth transmission.
     *
     * @param co2 CO2 level to set, in ppm (parts per million).
     */
    void bt_set_co2(float co2);

    /**
     * @brief Sets the VOC level for Bluetooth transmission.
     *
     * @param voc VOC level to set, in ppb (parts per billion).
     */
    void bt_set_voc(float voc);

    /**
     * @brief Sets the Index for Air Quality (IAQ) for Bluetooth transmission.
     *
     * @param iaq IAQ to set.
     */
    void bt_set_iaq(uint16_t iaq);

    /**
     * @brief Sets the battery level for Bluetooth transmission.
     *
     * @param batt Battery level to set, in percentage.
     */
    void bt_set_battery(uint8_t batt);

    /**
     * @brief Checks if a Bluetooth connection exists.
     *
     * @return true if a connection exists, false otherwise.
     */
    bool bt_connection_exists();

    /**
     * @brief Updates the Bluetooth advertising data.
     */
    void update_advertise_data();

#ifdef __cplusplus
}
#endif
