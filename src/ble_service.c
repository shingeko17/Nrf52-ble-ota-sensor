/**
 * @file ble_service.c
 * @brief BLE GATT service implementation
 * @author Buzz - HCMUT
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/logging/log.h>

#include "app_config.h"
#include "ble_service.h"
#include "led_status.h"

LOG_MODULE_REGISTER(ble_service, LOG_LEVEL_INF);

/* Custom Service UUID */
static struct bt_uuid_128 custom_svc_uuid = BT_UUID_INIT_128(CUSTOM_SERVICE_UUID_VAL);
static struct bt_uuid_128 temp_char_uuid = BT_UUID_INIT_128(TEMP_CHAR_UUID_VAL);
static struct bt_uuid_128 button_char_uuid = BT_UUID_INIT_128(BUTTON_CHAR_UUID_VAL);

/* State */
static struct bt_conn *current_conn;
static bool temp_notify_enabled;
static bool button_notify_enabled;
static int16_t current_temp;
static uint8_t button_state[2]; /* [button_id, pressed] */

/* CCC changed callbacks */
static void temp_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	temp_notify_enabled = (value == BT_GATT_CCC_NOTIFY);
	LOG_INF("Temperature notifications %s",
		temp_notify_enabled ? "enabled" : "disabled");
}

static void button_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	button_notify_enabled = (value == BT_GATT_CCC_NOTIFY);
	LOG_INF("Button notifications %s",
		button_notify_enabled ? "enabled" : "disabled");
}

/* Read callback for temperature */
static ssize_t read_temp(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset,
				 &current_temp, sizeof(current_temp));
}

/* GATT Service Definition */
BT_GATT_SERVICE_DEFINE(custom_svc,
	BT_GATT_PRIMARY_SERVICE(&custom_svc_uuid),

	/* Temperature Characteristic: Read + Notify */
	BT_GATT_CHARACTERISTIC(&temp_char_uuid.uuid,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ,
			       read_temp, NULL, &current_temp),
	BT_GATT_CCC(temp_ccc_cfg_changed,
		     BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

	/* Button Characteristic: Notify only */
	BT_GATT_CHARACTERISTIC(&button_char_uuid.uuid,
			       BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_NONE,
			       NULL, NULL, &button_state),
	BT_GATT_CCC(button_ccc_cfg_changed,
		     BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

/* Advertising data */
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, BLE_DEVICE_NAME, sizeof(BLE_DEVICE_NAME) - 1),
};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, CUSTOM_SERVICE_UUID_VAL),
};

/* Connection callbacks */
static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		LOG_ERR("Connection failed: %d", err);
		return;
	}

	LOG_INF("Connected");
	current_conn = bt_conn_ref(conn);
	led_status_set(LED_STATE_CONNECTED);
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected (reason: %d)", reason);

	if (current_conn) {
		bt_conn_unref(current_conn);
		current_conn = NULL;
	}

	temp_notify_enabled = false;
	button_notify_enabled = false;
	led_status_set(LED_STATE_ADVERTISING);

	/* Restart advertising */
	ble_service_start_adv();
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

/* Public API */
int ble_service_init(void)
{
	int ret;

	ret = bt_enable(NULL);
	if (ret) {
		LOG_ERR("Bluetooth init failed: %d", ret);
		return ret;
	}

	LOG_INF("Bluetooth initialized");
	return 0;
}

int ble_service_start_adv(void)
{
	int ret;

	ret = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad),
			      sd, ARRAY_SIZE(sd));
	if (ret) {
		LOG_ERR("Advertising failed to start: %d", ret);
		return ret;
	}

	LOG_INF("Advertising started");
	led_status_set(LED_STATE_ADVERTISING);
	return 0;
}

bool ble_service_is_connected(void)
{
	return (current_conn != NULL);
}

int ble_service_notify_temp(int16_t temp_value)
{
	if (!current_conn || !temp_notify_enabled) {
		return -ENOTCONN;
	}

	current_temp = temp_value;

	return bt_gatt_notify(current_conn, &custom_svc.attrs[1],
			      &current_temp, sizeof(current_temp));
}

int ble_service_notify_button(uint8_t button_id, bool pressed)
{
	if (!current_conn || !button_notify_enabled) {
		return -ENOTCONN;
	}

	button_state[0] = button_id;
	button_state[1] = pressed ? 1 : 0;

	return bt_gatt_notify(current_conn, &custom_svc.attrs[4],
			      &button_state, sizeof(button_state));
}
