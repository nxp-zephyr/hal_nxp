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

#ifndef __WU_H__
#define __WU_H__

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*                               Define                                       */
/* -------------------------------------------------------------------------- */

#define WIRELESS_UART_SERVICE_UUID \
	0xE0, 0x1C, 0x4B, 0x5E, 0x1E, 0xEB, 0xA1, 0x5C, 0xEE, 0xF4, 0x5E, 0xBA, 0x00, 0x01, 0xFF, 0x01
#define WIRELESS_UART_SERVICE BT_UUID_DECLARE_128(WIRELESS_UART_SERVICE_UUID)

#define WIRELESS_UART_RAED_STREAM \
	BT_UUID_DECLARE_128(0xE0, 0x1C, 0x4B, 0x5E, 0x1E, 0xEB, 0xA1, 0x5C, 0xEE, 0xF4, 0x5E, 0xBA, 0x02, 0x01, 0xFF, 0x01)
#define WIRELESS_UART_WRITE_STREAM \
	BT_UUID_DECLARE_128(0xE0, 0x1C, 0x4B, 0x5E, 0x1E, 0xEB, 0xA1, 0x5C, 0xEE, 0xF4, 0x5E, 0xBA, 0x01, 0x01, 0xFF, 0x01)
#define WIRELESS_UART_INFO \
	BT_UUID_DECLARE_128(0xE0, 0x1C, 0x4B, 0x5E, 0x1E, 0xEB, 0xA1, 0x5C, 0xEE, 0xF4, 0x5E, 0xBA, 0x03, 0x01, 0xFF, 0x01)
#define WIRELESS_UART_SERIAL_NO \
	BT_UUID_DECLARE_128(0xE0, 0x1C, 0x4B, 0x5E, 0x1E, 0xEB, 0xA1, 0x5C, 0xEE, 0xF4, 0x5E, 0xBA, 0x04, 0x01, 0xFF, 0x01)

typedef int (*bt_gatt_wu_read_response_t)(void *param, uint8_t *buffer, ssize_t length);
typedef int (*bt_gatt_wu_read_request_t)(struct bt_conn *conn, bt_gatt_wu_read_response_t response, void *param);
typedef int (*bt_gatt_wu_data_received_t)(struct bt_conn *conn, uint8_t *buffer, ssize_t length);

/* -------------------------------------------------------------------------- */
/*                               Structure                                    */
/* -------------------------------------------------------------------------- */

typedef struct _bt_gatt_wu_config {
	bt_gatt_wu_read_request_t  read;
	bt_gatt_wu_data_received_t data_received;
} bt_gatt_wu_config_t;

typedef struct _bt_gatt_wu_peer_state {
	struct bt_conn                *conn;
	struct bt_uuid_128             serivceDiscoveryUuid;
	struct bt_gatt_exchange_params exchangeParams;
	struct bt_gatt_discover_params discoverParams;
	struct bt_gatt_write_params    writeParams;
	volatile uint8_t               discoverWriteHandle;
	volatile uint8_t               discoverPermission;
} bt_gatt_wu_peer_state_t;

typedef struct _bt_gatt_wu_state {
	char                   *name;
	char                   *serial_no;
	bt_gatt_wu_config_t     config;
	bt_gatt_wu_peer_state_t peer;
	volatile uint8_t        read_stream_ccc_cfg;
} bt_gatt_wu_state_t;

typedef struct _bt_gatt_wu_read {
	bt_gatt_wu_peer_state_t   *state;
	const struct bt_gatt_attr *attr;
	void                      *buf;
	uint16_t                   len;
	uint16_t                   offset;
} bt_gatt_wu_read;

/* -------------------------------------------------------------------------- */
/*                             Function prototypes                            */
/* -------------------------------------------------------------------------- */

void bt_gatt_wu_init(char *name, char *serial_no, bt_gatt_wu_config_t *config);
void bt_gatt_wu_connected(struct bt_conn *conn);
void bt_gatt_wu_disconnected(struct bt_conn *conn);
void bt_gatt_wu_notify(void);

#ifdef __cplusplus
}
#endif

#endif /* __WU_H__ */

