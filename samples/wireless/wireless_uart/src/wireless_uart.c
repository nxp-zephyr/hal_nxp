/*
 * Copyright 2019~2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* -------------------------------------------------------------------------- */
/*                                Includes                                    */
/* -------------------------------------------------------------------------- */

#include <zephyr/bluetooth/gatt.h>
#include "wu.h"


/* -------------------------------------------------------------------------- */
/*                              Function Prototypes                           */
/* -------------------------------------------------------------------------- */

static void connected(struct bt_conn *conn, uint8_t err);
static void disconnected(struct bt_conn *conn, uint8_t reason);
#if CONFIG_BT_SMP
static void security_changed(struct bt_conn *conn, bt_security_t level, enum bt_security_err err);
static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey);
static void auth_cancel(struct bt_conn *conn);
#endif

typedef struct _scan_dev_info_t {
	const bt_addr_le_t *addr;
	uint8_t             ad_type;
	uint8_t             ad_len;
	int8_t              rssi;
} scan_dev_info_t;

/* -------------------------------------------------------------------------- */
/*                            Variable                                        */
/* -------------------------------------------------------------------------- */

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

#if !defined(CONFIG_WU_CLIENT)
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID128_SOME, WIRELESS_UART_SERVICE_UUID),
};
static const struct bt_data sd[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};
#endif

static struct bt_conn_cb conn_callbacks = {
	.connected    = connected,
	.disconnected = disconnected,
#if CONFIG_BT_SMP
	.security_changed = security_changed,
#endif
};

#if CONFIG_BT_SMP
static struct bt_conn_auth_cb auth_cb_display = {
	.passkey_display = auth_passkey_display,
	.passkey_entry   = NULL,
	.cancel          = auth_cancel,
};
#endif

/* -------------------------------------------------------------------------- */
/*                              Functions                                     */
/* -------------------------------------------------------------------------- */

static void connected(struct bt_conn *conn, uint8_t err)
{
	char                addr[BT_ADDR_LE_STR_LEN];
	struct bt_conn_info info;

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (err) {
		printf("Failed to connect %s (err %u)\n", addr, err);
	} else {
		err = bt_conn_get_info(conn, &info);
		if (err) {
			printf("Failed to get info");
			return;
		}

		switch (info.role) {
		case BT_HCI_ROLE_PERIPHERAL:

			break;
		case BT_HCI_ROLE_CENTRAL:

			break;
		}

		bt_gatt_wu_connected(conn);

		printf("Connected to %s\n", addr);

#if CONFIG_BT_SMP
		if (info.role == BT_HCI_ROLE_PERIPHERAL) {
			if (bt_conn_set_security(conn, BT_SECURITY_L2)) {
				printf("Failed to set security\n");
			}
		}
#endif
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_gatt_wu_disconnected(conn);

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printf("Disconnected from %s (reason %u)\n", addr, reason);
}

#if CONFIG_BT_SMP
static void security_changed(struct bt_conn *conn, bt_security_t level, enum bt_security_err err)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printf("Security changed: %s level %u (error %d)\n", addr, level, err);
}

static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printf("Passkey for %s: %06u\n", addr, passkey);
}

static void auth_cancel(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printf("Pairing cancelled: %s\n", addr);
}
#endif

static int wireless_uart_data_received(struct bt_conn *conn, uint8_t *buffer, ssize_t length)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
	printf("Data received from %s(length %d):", addr, length);
	for (int i = 0; i < length; i++) {
		printf("%c", ((uint8_t *)buffer)[i]);
	}
	printf("\r\n");
	return 0;
}

static int wireless_uart_read(struct bt_conn *conn, bt_gatt_wu_read_response_t response, void *param)
{
	uint8_t buffer[13] = {'w', 'i', 'r', 'e', 'l', 'e', 's', 's', ' ', 'u', 'a', 'r', 't'};
	ssize_t length     = 13;

	int ret = -1;

	if (response != NULL) {
		ret = response(param, buffer, length);
	}

	return ret;
}

#ifdef CONFIG_WU_CLIENT
static bool wu_central_parse_callback(struct bt_data *data, void *user_data)
{
	scan_dev_info_t *deviceInfo = (scan_dev_info_t *)user_data;
	char             dev[BT_ADDR_LE_STR_LEN];
	int              i;
	int              error = -1;

	struct bt_conn *conn;

	switch (data->type) {
	case BT_DATA_UUID128_SOME:
	case BT_DATA_UUID128_ALL:
		if (data->data_len % (sizeof(struct bt_uuid_128) - sizeof(struct bt_uuid)) != 0) {
			printf("AD malformed\n");
			return true;
		}

		for (i = 0; i < data->data_len; i += (sizeof(struct bt_uuid_128) - sizeof(struct bt_uuid))) {
			struct bt_uuid_128 uuid;

			uuid.uuid.type = BT_UUID_TYPE_128;
			memcpy(&uuid.val[0], &data->data[i], sizeof(struct bt_uuid_128) - sizeof(struct bt_uuid));
			if (bt_uuid_cmp((struct bt_uuid *)&uuid, WIRELESS_UART_SERVICE)) {
				continue;
			}

			bt_addr_le_to_str(deviceInfo->addr, dev, sizeof(dev));
			printf("[DEVICE]: %s, AD evt type %u, AD data len %u, RSSI %i\n", dev, deviceInfo->ad_type,
				   deviceInfo->ad_len, deviceInfo->rssi);

			error = bt_le_scan_stop();
			if (error) {
				printf("Stop scan false.\n");
			}

			struct bt_conn_le_create_param param =
				BT_CONN_LE_CREATE_PARAM_INIT(BT_CONN_LE_OPT_NONE, BT_GAP_SCAN_FAST_INTERVAL, BT_GAP_SCAN_FAST_INTERVAL);
			error = bt_conn_le_create(deviceInfo->addr, &param, BT_LE_CONN_PARAM_DEFAULT, &conn);
			if (error) {
				printf("Failed to create connection (err %d)\n", error);
			} else {
				printf("Connection pending");
				/* unref connection obj in advance as app user */
				bt_conn_unref(conn);
			}
		}
	}

	return true;
}

static void wu_central_scan_callback(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad)
{
	scan_dev_info_t deviceInfo;

	deviceInfo.addr    = addr;
	deviceInfo.ad_type = type;
	deviceInfo.ad_len  = ad->len;
	deviceInfo.rssi    = rssi;

#if 0
	char dev[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(addr, dev, sizeof(dev));
	PRINTF("[DEVICE]: %s, AD evt type %u, AD data len %u, RSSI %i\n", deviceInfo.dev, deviceInfo.ad_type, deviceInfo.ad_len, deviceInfo.rssi);
#endif

	if (type == BT_HCI_ADV_IND || type == BT_HCI_ADV_DIRECT_IND) {
		bt_data_parse(ad, wu_central_parse_callback, (void *)&deviceInfo);
	}
}
#endif

static void bt_ready(int error)
{
	bt_gatt_wu_config_t wuConfig;

	if (error) {
		printf("Bluetooth init failed (error %d)\n", error);
		return;
	}

	printf("Bluetooth initialized\n");

	wuConfig.data_received = wireless_uart_data_received;
	wuConfig.read          = wireless_uart_read;

	bt_gatt_wu_init("Wireless Uart Demo", "WU1234567890", &wuConfig);

	bt_conn_cb_register(&conn_callbacks);

#if CONFIG_BT_SMP
	bt_conn_auth_cb_register(&auth_cb_display);
#endif

#ifdef CONFIG_WU_CLIENT
	error = bt_le_scan_start(BT_LE_SCAN_ACTIVE, wu_central_scan_callback);
	if (error) {
		printf("Scan failed to start (error %d)\n", error);
		return;
	}
	printf("Scan successfully started\n");
#else
	error = bt_le_adv_start(BT_LE_ADV_CONN_FAST_2, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (error) {
		printf("Advertising failed to start (error %d)\n", error);
		return;
	}
	printf("Advertising successfully started\n");
#endif
}

/*
 *  Application entry point.
 */
void wireless_uart_start(void)
{
	int err;

	printf("BLE Wireless Uart demo start...\n");

	err = bt_enable(bt_ready);
	if (err) {
		printf("Bluetooth init failed (err %d)\n", err);
	}

	while (1) {
		k_sleep(K_MSEC(2000));

		/* Wireless Uart notification thread */
		bt_gatt_wu_notify();
	}
}

