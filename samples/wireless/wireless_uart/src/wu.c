/*
 * Copyright 2019~2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * @file
 * @brief Wireless Uart Service Profile
 *
 */

/* -------------------------------------------------------------------------- */
/*                              Includes                                      */
/* -------------------------------------------------------------------------- */

#include <zephyr/bluetooth/gatt.h>
#include "wu.h"


/* -------------------------------------------------------------------------- */
/*                              Variable                                      */
/* -------------------------------------------------------------------------- */

static bt_gatt_wu_state_t s_wu_state;

/* -------------------------------------------------------------------------- */
/*                              Functions                                     */
/* -------------------------------------------------------------------------- */

void bt_gatt_wu_init(char *name, char *serial_no, bt_gatt_wu_config_t *config)
{
	s_wu_state.config              = *config;
	s_wu_state.name                = name;
	s_wu_state.serial_no           = serial_no;
	s_wu_state.read_stream_ccc_cfg = 0;
}

#ifdef CONFIG_WU_CLIENT
static void bt_gatt_wu_write_func(struct bt_conn *conn, uint8_t err, struct bt_gatt_write_params *params)
{
	/* Write response, do nothing */
}

static int bt_gatt_wu_read_response_for_write(void *param, uint8_t *buffer, ssize_t length)
{
	bt_gatt_wu_peer_state_t *state = (bt_gatt_wu_peer_state_t *)param;
	int                      ret   = -1;
	uint16_t                 mtu   = bt_gatt_get_mtu(state->conn);
	uint16_t                 len   = 0;
	int                      sent  = 0;

	while (length > 0U) {
		len    = length > (mtu - 3) ? mtu - 3 : length;
		length = length - len;
		if (state->discoverPermission & BT_GATT_CHRC_WRITE_WITHOUT_RESP) {
			ret = bt_gatt_write_without_response(state->conn, state->discoverWriteHandle, &buffer[sent], len, 0);
			if (ret >= 0) {
				sent += (int)len;
			} else {
				break;
			}
		} else if (state->discoverPermission & BT_GATT_CHRC_WRITE) {
			state->writeParams.data   = (const void *)&buffer[sent];
			state->writeParams.length = len;
			state->writeParams.handle = state->discoverWriteHandle;
			state->writeParams.offset = 0;
			state->writeParams.func   = bt_gatt_wu_write_func;
			ret                       = bt_gatt_write(state->conn, &state->writeParams);
			if (ret >= 0) {
				sent += (int)len;
			} else {
				break;
			}
		} else {
			break;
		}
	}

	return sent;
}

static uint8_t bt_gatt_wu_discovery_service(struct bt_conn                 *conn,
											const struct bt_gatt_attr      *attr,
											struct bt_gatt_discover_params *params)
{
	int err;

	if (!attr) {
		printf("Discover complete\n");
		memset(params, 0, sizeof(*params));
		return BT_GATT_ITER_STOP;
	}

	printf("[ATTRIBUTE] handle %u\n", attr->handle);

	if (!bt_uuid_cmp(s_wu_state.peer.discoverParams.uuid, WIRELESS_UART_SERVICE)) {
		memcpy(&s_wu_state.peer.serivceDiscoveryUuid, WIRELESS_UART_WRITE_STREAM,
			   sizeof(s_wu_state.peer.serivceDiscoveryUuid));
		s_wu_state.peer.discoverParams.uuid         = &s_wu_state.peer.serivceDiscoveryUuid.uuid;
		s_wu_state.peer.discoverParams.start_handle = attr->handle + 1;
		s_wu_state.peer.discoverParams.type         = BT_GATT_DISCOVER_CHARACTERISTIC;
#if defined(CONFIG_BT_GATT_CLIENT)
		err = bt_gatt_discover(conn, &s_wu_state.peer.discoverParams);
		if (err) {
			printf("Discover stream write service failed (err %d)\n", err);
		}
#endif
	} else if (!bt_uuid_cmp(s_wu_state.peer.discoverParams.uuid, WIRELESS_UART_WRITE_STREAM)) {
		struct bt_gatt_chrc *gattChrc = (struct bt_gatt_chrc *)attr->user_data;

		s_wu_state.peer.discoverPermission  = gattChrc->properties;
		s_wu_state.peer.discoverWriteHandle = attr->handle + 1;
	} else {
		/* Do Nothing */
	}

	return BT_GATT_ITER_STOP;
}

static void bt_gatt_wu_start_service_discovery(struct bt_conn *conn)
{
	int     err;

	memcpy(&s_wu_state.peer.serivceDiscoveryUuid, WIRELESS_UART_SERVICE, sizeof(s_wu_state.peer.serivceDiscoveryUuid));
	s_wu_state.peer.discoverParams.uuid         = &s_wu_state.peer.serivceDiscoveryUuid.uuid;
	s_wu_state.peer.discoverParams.func         = bt_gatt_wu_discovery_service;
	s_wu_state.peer.discoverParams.start_handle = 0x0001;
	s_wu_state.peer.discoverParams.end_handle   = 0xffff;
	s_wu_state.peer.discoverParams.type         = BT_GATT_DISCOVER_PRIMARY;

#if defined(CONFIG_BT_GATT_CLIENT)
	err = bt_gatt_discover(conn, &s_wu_state.peer.discoverParams);
	if (err) {
		printf("Discover failed(err %d)\n", err);
		return;
	}
#endif
}
#endif

static void bt_gatt_wu_get_mtu_callback(struct bt_conn *conn, uint8_t err, struct bt_gatt_exchange_params *params)
{
	if (err) {
		printf("GATT MTU exchange response failed (err %u)\n", err);
	} else {
		printf("GATT MTU exchanged: %d\n", bt_gatt_get_mtu(conn));
	}

#ifdef CONFIG_WU_CLIENT
	bt_gatt_wu_start_service_discovery(conn);
#endif
}

void bt_gatt_wu_connected(struct bt_conn *conn)
{
	int err;

	s_wu_state.peer.conn                = conn;
	s_wu_state.peer.exchangeParams.func = bt_gatt_wu_get_mtu_callback;
	err                                 = bt_gatt_exchange_mtu(conn, &s_wu_state.peer.exchangeParams);
	if (err) {
		printf("Exchange GATT mtu failed (err %d)\n", err);
	}
}

void bt_gatt_wu_disconnected(struct bt_conn *conn)
{
	s_wu_state.peer.discoverWriteHandle = 0;
	s_wu_state.peer.discoverPermission  = 0;
	s_wu_state.peer.conn                = NULL;
	s_wu_state.read_stream_ccc_cfg      = 0;
}

static int bt_gatt_wu_read_response_for_read(void *param, uint8_t *buffer, ssize_t length)
{
	bt_gatt_wu_read *read = (bt_gatt_wu_read *)param;
	int                     ret  = -1;
	uint16_t                mtu  = bt_gatt_get_mtu(read->state->conn);
	uint16_t                len  = 0;
	int                     sent = 0;

	while (length > 0) {
		len    = length > (mtu - 3) ? mtu - 3 : length;
		length = length - len;

		ret = bt_gatt_attr_read(read->state->conn, read->attr, read->buf, read->len, read->offset, &buffer[sent], len);
		if (ret >= 0) {
			sent += len;
		} else {
			break;
		}
	}

	return sent;
}

static ssize_t bt_gatt_wu_read_stream(struct bt_conn            *conn,
									  const struct bt_gatt_attr *attr,
									  void                      *buf,
									  uint16_t                   len,
									  uint16_t                   offset)
{
	bt_gatt_wu_read read;
	ssize_t         length = 0;

	read.attr   = attr;
	read.buf    = buf;
	read.len    = len;
	read.offset = offset;
	read.state  = &s_wu_state.peer;

	length = s_wu_state.config.read(s_wu_state.peer.conn, bt_gatt_wu_read_response_for_read, &read);

	return length;
}

static void bt_gatt_wu_read_stream_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	s_wu_state.read_stream_ccc_cfg = (value & BT_GATT_CCC_NOTIFY);
}

static ssize_t bt_gatt_wu_write_stream(struct bt_conn            *conn,
									   const struct bt_gatt_attr *attr,
									   const void                *buf,
									   uint16_t                   len,
									   uint16_t                   offset,
									   uint8_t                    flags)
{
#if 0
	printf("Data received (%d) 0x: ", len);
	for (int i = 0; i < len; i++) {
		printf("%02X ", ((uint8_t *)buf)[i]);
	}
	printf("\r\n");
#endif
	s_wu_state.config.data_received(conn, (uint8_t *)buf, len);

	return 0;
}

static ssize_t bt_gatt_wu_info(struct bt_conn            *conn,
							   const struct bt_gatt_attr *attr,
							   void                      *buf,
							   uint16_t                   len,
							   uint16_t                   offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, s_wu_state.name, strlen(s_wu_state.name));
}

static ssize_t bt_gatt_wu_serial_no(struct bt_conn            *conn,
									const struct bt_gatt_attr *attr,
									void                      *buf,
									uint16_t                   len,
									uint16_t                   offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, s_wu_state.serial_no, strlen(s_wu_state.serial_no));
}

/* Wireless Uart Service Declaration */
BT_GATT_SERVICE_DEFINE(
	wirelessUart,
	BT_GATT_PRIMARY_SERVICE(WIRELESS_UART_SERVICE),

	BT_GATT_CHARACTERISTIC(WIRELESS_UART_WRITE_STREAM,
						   BT_GATT_CHRC_WRITE_WITHOUT_RESP,
						   BT_GATT_PERM_WRITE,
						   NULL,
						   bt_gatt_wu_write_stream,
						   NULL),

	BT_GATT_CHARACTERISTIC(WIRELESS_UART_RAED_STREAM,
						   BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
						   BT_GATT_PERM_READ,
						   bt_gatt_wu_read_stream,
						   NULL,
						   NULL),
	BT_GATT_CCC(bt_gatt_wu_read_stream_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

	BT_GATT_CHARACTERISTIC(WIRELESS_UART_INFO, BT_GATT_CHRC_READ, BT_GATT_PERM_READ, bt_gatt_wu_info, NULL, NULL),

	BT_GATT_CHARACTERISTIC(WIRELESS_UART_SERIAL_NO,
						   BT_GATT_CHRC_READ,
						   BT_GATT_PERM_READ,
						   bt_gatt_wu_serial_no,
						   NULL,
						   NULL),
);

static int bt_gatt_wu_read_response_for_notify(void *param, uint8_t *buffer, ssize_t length)
{
	bt_gatt_wu_peer_state_t *state = (bt_gatt_wu_peer_state_t *)param;
	int                      ret   = -1;
	uint16_t                 mtu   = bt_gatt_get_mtu(state->conn);
	uint16_t                 len   = 0;
	int                      sent  = 0;

	while (length > 0U) {
		len    = length > (mtu - 3) ? mtu - 3 : length;
		length = length - len;
		ret    = bt_gatt_notify(state->conn, &wirelessUart.attrs[3], &buffer[sent], len);
		if (ret >= 0) {
			sent += (int)len;
		} else {
			break;
		}
	}
	return sent;
}

static void bt_gatt_wu_notify_internal(void)
{
	int ret;

	if (s_wu_state.read_stream_ccc_cfg) {
		ret = s_wu_state.config.read(s_wu_state.peer.conn, bt_gatt_wu_read_response_for_notify, &s_wu_state.peer);
	}
#ifdef CONFIG_WU_CLIENT
	else if (s_wu_state.peer.discoverWriteHandle) {
		ret = s_wu_state.config.read(s_wu_state.peer.conn, bt_gatt_wu_read_response_for_write, &s_wu_state.peer);
	}
#endif

	(void)ret;
}

void bt_gatt_wu_notify(void)
{
	bt_gatt_wu_notify_internal();
}
