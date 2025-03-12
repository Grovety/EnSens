/*
 * Copyright (c) 2025 Grovety Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/settings/settings.h>
#include <hw_id.h>
#include <math.h>

#include "ble.h"

#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(app, CONFIG_APP_LOG_LEVEL);

#define SERVICE_DATA_LEN 18
#define SERVICE_UUID 0xfcd2 /* BTHome service UUID */
#define IDX_BATT 4			/* Index of battery data in service data*/
#define IDX_TEMPL 6			/* Index of lo byte of temp in service data*/
#define IDX_TEMPH 7			/* Index of hi byte of temp in service data*/
#define IDX_HUML 9			/* Index of lo byte of humidity in service data*/
#define IDX_HUMH 10			/* Index of hi byte of humidity in service data*/
#define IDX_PRESSUREL 12	/* Index of lo byte of pressure in service data*/
#define IDX_PRESSUREH 14	/* Index of hi byte of pressure in service data*/
#define IDX_CO2L 16			/* Index of lo byte of co2 in service data*/
#define IDX_CO2H 17			/* Index of hi byte of co2 in service data*/

static int16_t last_temp = 0;
static uint16_t last_humidity = 0;
static uint32_t last_pressure = 0;
static uint16_t last_voc = 0;
static uint16_t last_co2 = 0;
static uint16_t last_iaq = 0;
static uint8_t last_batt = 0;
static uint8_t service_data[SERVICE_DATA_LEN] = {
	BT_UUID_16_ENCODE(SERVICE_UUID),
	0x40,
	0x01, /* Battery */
	0x00,
	0x02, /* Temperature */
	0x00, /* Low byte */
	0x00, /* High byte */
	0x03, /* Humidity */
	0x00, /* Low byte */
	0x00, /* High byte */
	0x04, /* Pressure */
	0x00, /* Low byte */
	0x00,
	0x00, /* High byte */
	0x12, /* CO2 */
	0x00, /* Low byte */
	0x00, /* High byte */
};

static char unique_name[sizeof(CONFIG_BT_DEVICE_NAME) + HW_ID_LEN];

static struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
	BT_DATA(BT_DATA_NAME_COMPLETE, unique_name, 6),
	BT_DATA(BT_DATA_SVC_DATA16, service_data, ARRAY_SIZE(service_data))};

#define ADV_PARAM BT_LE_ADV_PARAM(BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_IDENTITY, \
								  BT_GAP_ADV_SLOW_INT_MIN,                                \
								  BT_GAP_ADV_SLOW_INT_MAX, NULL)

static ssize_t read_temp(struct bt_conn *conn, const struct bt_gatt_attr *attr,
						 void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &last_temp,
							 sizeof(last_temp));
}

static ssize_t read_humidity(struct bt_conn *conn, const struct bt_gatt_attr *attr,
							 void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &last_humidity,
							 sizeof(last_humidity));
}

static ssize_t read_pressure(struct bt_conn *conn, const struct bt_gatt_attr *attr,
							 void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &last_pressure,
							 sizeof(last_pressure));
}

static ssize_t read_co2(struct bt_conn *conn, const struct bt_gatt_attr *attr,
						void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &last_co2,
							 sizeof(last_co2));
}

static ssize_t read_voc(struct bt_conn *conn, const struct bt_gatt_attr *attr,
						void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &last_voc,
							 sizeof(last_voc));
}

static ssize_t read_iaq(struct bt_conn *conn, const struct bt_gatt_attr *attr,
						void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &last_iaq,
							 sizeof(last_iaq));
}

/* This function is called whenever the CCCD register has been changed by the client*/
static void on_ccc_cfg_changed(const struct bt_gatt_attr *attr,
							   uint16_t value)
{
	ARG_UNUSED(attr);
	switch (value)
	{
	case BT_GATT_CCC_NOTIFY:
		// Start sending stuff!
		break;

	case BT_GATT_CCC_INDICATE:
		// Start sending stuff via indications
		break;

	case 0:
		// Stop sending stuff
		break;

	default:
		printk("Error, CCCD has been set to an invalid value");
	}
}

BT_GATT_SERVICE_DEFINE(ess_svc,
					   BT_GATT_PRIMARY_SERVICE(BT_UUID_ESS),
					   /* Temperature Sensor */
					   BT_GATT_CHARACTERISTIC(BT_UUID_TEMPERATURE,
											  BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
											  BT_GATT_PERM_READ,
											  read_temp, NULL, NULL),
					   BT_GATT_CCC(on_ccc_cfg_changed,
								   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
					   BT_GATT_CHARACTERISTIC(BT_UUID_HUMIDITY,
											  BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
											  BT_GATT_PERM_READ,
											  read_humidity, NULL, NULL),
					   BT_GATT_CCC(on_ccc_cfg_changed,
								   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
					   BT_GATT_CHARACTERISTIC(BT_UUID_PRESSURE,
											  BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
											  BT_GATT_PERM_READ,
											  read_pressure, NULL, NULL),
					   BT_GATT_CCC(on_ccc_cfg_changed,
								   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
					   BT_GATT_CHARACTERISTIC(BT_UUID_GATT_CO2CONC,
											  BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
											  BT_GATT_PERM_READ,
											  read_co2, NULL, NULL),
					   BT_GATT_CCC(on_ccc_cfg_changed,
								   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
					   BT_GATT_CHARACTERISTIC(BT_UUID_GATT_VOCCONC,
											  BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
											  BT_GATT_PERM_READ,
											  read_voc, NULL, NULL),
					   BT_GATT_CCC(on_ccc_cfg_changed,
								   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
					   BT_GATT_CHARACTERISTIC(BT_UUID_GATT_IAQ,
											  BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
											  BT_GATT_PERM_READ,
											  read_iaq, NULL, NULL),
					   BT_GATT_CCC(on_ccc_cfg_changed,
								   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE), );

int bt_init()
{
	int err = 0;

	// fill ID
	char buf[HW_ID_LEN];
	hw_id_get(buf, HW_ID_LEN);

	memcpy(unique_name, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME));

	if (IS_ENABLED(CONFIG_SETTINGS))
	{
		settings_load();
	}

	memcpy(&unique_name[3], &buf[HW_ID_LEN - 4], 3);

	err = bt_le_adv_start(ADV_PARAM, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err)
	{
		LOG_ERR("Failed to start the Bluetooth advertisement (err %d)", err);
	}
	bt_set_name(unique_name);
	bt_le_adv_update_data(ad, ARRAY_SIZE(ad), NULL, 0);

	return err;
}

void bt_set_temperature(float temp)
{
	int16_t new_value = (int16_t)(temp * 100);
	if (last_temp != new_value)
	{
		last_temp = new_value;
		service_data[IDX_TEMPH] = last_temp >> 8;
		service_data[IDX_TEMPL] = last_temp & 0xff;

		struct bt_gatt_attr *attr;
		attr = bt_gatt_find_by_uuid(ess_svc.attrs, ess_svc.attr_count, BT_UUID_TEMPERATURE);
		bt_gatt_notify(NULL, attr, &last_temp, sizeof(last_temp));
	}
}

void bt_set_humidity(float humidity)
{
	uint16_t new_value = (uint16_t)(humidity * 100);
	if (last_humidity != new_value)
	{
		last_humidity = new_value;
		service_data[IDX_HUMH] = last_humidity >> 8;
		service_data[IDX_HUML] = last_humidity & 0xff;

		struct bt_gatt_attr *attr;
		attr = bt_gatt_find_by_uuid(ess_svc.attrs, ess_svc.attr_count, BT_UUID_HUMIDITY);
		bt_gatt_notify(NULL, attr, &last_humidity, sizeof(last_humidity));
	}
}

void bt_set_pressure(float pressure)
{
	uint32_t new_value = (uint32_t)pressure;
	if (last_pressure != new_value)
	{
		last_pressure = new_value;
		service_data[IDX_PRESSUREH] = last_pressure >> 16 & 0xff;
		service_data[IDX_PRESSUREL + 1] = last_pressure >> 8 & 0xff;
		service_data[IDX_PRESSUREL] = last_pressure & 0xff;

		struct bt_gatt_attr *attr;
		attr = bt_gatt_find_by_uuid(ess_svc.attrs, ess_svc.attr_count, BT_UUID_PRESSURE);
		bt_gatt_notify(NULL, attr, &last_pressure, sizeof(last_pressure));
	}
}

void bt_set_co2(float co2)
{
	uint16_t new_value = (uint16_t)co2;
	if (last_co2 != new_value)
	{
		last_co2 = new_value;
		service_data[IDX_CO2H] = last_co2 >> 8;
		service_data[IDX_CO2L] = last_co2 & 0xff;

		struct bt_gatt_attr *attr;
		attr = bt_gatt_find_by_uuid(ess_svc.attrs, ess_svc.attr_count, BT_UUID_GATT_CO2CONC);
		bt_gatt_notify(NULL, attr, &last_co2, sizeof(last_co2));
	}
}

void bt_set_voc(float voc)
{
	uint16_t new_value = (uint16_t)voc;
	if (last_voc != new_value)
	{
		last_voc = new_value;

		struct bt_gatt_attr *attr;
		attr = bt_gatt_find_by_uuid(ess_svc.attrs, ess_svc.attr_count, BT_UUID_GATT_VOCCONC);
		bt_gatt_notify(NULL, attr, &last_voc, sizeof(last_voc));
	}
}

void bt_set_iaq(uint16_t iaq)
{
	if (last_iaq != iaq)
	{
		last_iaq = iaq;

		struct bt_gatt_attr *attr;
		attr = bt_gatt_find_by_uuid(ess_svc.attrs, ess_svc.attr_count, BT_UUID_GATT_IAQ);
		bt_gatt_notify(NULL, attr, &last_iaq, sizeof(last_iaq));
	}
}

void bt_set_battery(uint8_t batt)
{
	if (last_batt != batt)
	{
		last_batt = batt;
		service_data[IDX_BATT] = batt;
		bt_bas_set_battery_level(batt);
	}
}

static int connectionNumber = 0;

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (!err)
	{
		connectionNumber++;
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	connectionNumber--;
}

bool bt_connection_exists()
{
	return connectionNumber > 0;
}

void update_advertise_data()
{
	int err = bt_le_adv_update_data(ad, ARRAY_SIZE(ad), NULL, 0);
	if (err)
	{
		LOG_ERR("Failed to update the Bluetooth advertisement data (err %d)", err);
	}
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected};
