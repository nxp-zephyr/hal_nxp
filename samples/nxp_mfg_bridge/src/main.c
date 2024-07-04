/**
 *  Copyright 2023-2024 NXP
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  @file  main.c
 *  @brief main file
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(mfg_bridge, LOG_LEVEL_ERR);

#include "fsl_common.h"
#include "fsl_adapter_rfimu.h"
#include "fsl_loader.h"
#include "fsl_ocotp.h"
#include "uart_rtos.h"

#ifndef PRINTF
#define PRINTF printk
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define UART_BUF_SIZE           2048
#define LABTOOL_PATTERN_HDR_LEN 4
#define CHECKSUM_LEN            4
#define CRC32_POLY              0x04c11db7

/** Command type: WLAN */
#define TYPE_WLAN     0x0002
#define RET_TYPE_WLAN 1

/** Command type: BT */
#define TYPE_BT     0x0003
#define RET_TYPE_BT 2

/** Command type: 15.4 */
#define TYPE_15_4       0x0004
#define RET_TYPE_ZIGBEE 3

#define MLAN_TYPE_CMD   1
#define INTF_HEADER_LEN 4
#define SDIOPKTTYPE_CMD 0x1
#define BUF_LEN         1024
#define SDIO_OUTBUF_LEN 2048

#define WM_SUCCESS 0
#define WM_FAIL    1

#define UNUSED(x) (void)(x)

#define REMOTE_EPT_ADDR_BT     (40U)
#define LOCAL_EPT_ADDR_BT      (30U)
#define REMOTE_EPT_ADDR_ZIGBEE (20U)
#define LOCAL_EPT_ADDR_ZIGBEE  (10U)

#define WIFI_REG8(x)  (*(volatile unsigned char *)(x))
#define WIFI_REG16(x) (*(volatile unsigned short *)(x))
#define WIFI_REG32(x) (*(volatile unsigned int *)(x))

#define WIFI_WRITE_REG8(reg, val)  (WIFI_REG8(reg) = (val))
#define WIFI_WRITE_REG16(reg, val) (WIFI_REG16(reg) = (val))
#define WIFI_WRITE_REG32(reg, val) (WIFI_REG32(reg) = (val))

/* Set default mode of fw download */
#ifndef CONFIG_SUPPORT_WIFI
#define CONFIG_SUPPORT_WIFI 1
#endif
#ifndef CONFIG_SUPPORT_BLE
#define CONFIG_SUPPORT_BLE 1
#endif
#ifndef CONFIG_SUPPORT_BLE_15D4
#define CONFIG_SUPPORT_BLE_15D4 1
#endif

#define WLAN_CAU_ENABLE_ADDR         (0x45004008U)
#define WLAN_CAU_TEMPERATURE_ADDR    (0x4500400CU)
#define WLAN_CAU_TEMPERATURE_FW_ADDR (0x41382490U)
#define WLAN_FW_WAKE_STATUS_ADDR     (0x40031068U)

enum {
	MLAN_CARD_NOT_DETECTED = 3,
	MLAN_STATUS_FW_DNLD_FAILED,
	MLAN_STATUS_FW_NOT_DETECTED = 5,
	MLAN_STATUS_FW_NOT_READY,
	MLAN_STATUS_FW_XZ_FAILED,
	MLAN_CARD_CMD_TIMEOUT
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define pvPortMalloc k_malloc

struct uart_rtos_state uart_handle;
struct k_timer g_wifi_cau_temperature_timer;

static RPMSG_HANDLE_DEFINE(bt_rpmsg_handle);
static RPMSG_HANDLE_DEFINE(zigbee_rpmsg_handle);
static hal_rpmsg_handle_t rpmsgHandleList[] = {(hal_rpmsg_handle_t)bt_rpmsg_handle,
                                               (hal_rpmsg_handle_t)zigbee_rpmsg_handle};

uint32_t remote_ept_list[] = {REMOTE_EPT_ADDR_BT, REMOTE_EPT_ADDR_ZIGBEE};
uint32_t local_ept_list[] = {LOCAL_EPT_ADDR_BT, LOCAL_EPT_ADDR_ZIGBEE};

struct uart_cb { /* uart control block */
	int uart_fd;
	unsigned int crc32_table[256];
	unsigned char uart_buf[UART_BUF_SIZE]; /* uart buffer */
};

static struct uart_cb uartcb;

/** UART start pattern*/
struct uart_header {
	/** pattern */
	short pattern;
	/** Command length */
	short length;
};

/** Labtool command header */
struct cmd_header {
	/** Command Type */
	short type;
	/** Command Sub-type */
	short sub_type;
	/** Command length (header+payload) */
	short length;
	/** Command status */
	short status;
	/** reserved */
	int reserved;
};

/** HostCmd_DS_COMMAND */
struct HostCmd_DS_COMMAND {
	/** Command Header : Command */
	uint16_t command;
	/** Command Header : Size */
	uint16_t size;
	/** Command Header : Sequence number */
	uint16_t seq_num;
	/** Command Header : Result */
	uint16_t result;
	/** Command Body */
};

/** IMUPkt/SDIOPkt only name difference, same definition */
struct SDIOPkt {
	uint16_t size;
	uint16_t pkttype;
	struct HostCmd_DS_COMMAND hostcmd;
};

static uint8_t *rx_buf;
static struct cmd_header last_cmd_hdr;
uint8_t *local_outbuf;
static struct SDIOPkt *sdiopkt;

#if defined(CONFIG_MONOLITHIC_WIFI)
extern const uint32_t fw_cpu1[];
#define WIFI_FW_ADDRESS  (uint32_t)&fw_cpu1[0]
#else
#define WIFI_FW_ADDRESS  0U
#endif

#if defined(CONFIG_MONOLITHIC_IEEE802154)
extern const uint32_t fw_cpu2_combo[];
#define COMBO_FW_ADDRESS (uint32_t)&fw_cpu2_combo[0]
#else
#define COMBO_FW_ADDRESS   0U
#endif

#if defined(CONFIG_MONOLITHIC_BT) && !defined(CONFIG_MONOLITHIC_IEEE802154)
extern const uint32_t fw_cpu2_ble[];
#define BLE_FW_ADDRESS   (uint32_t)&fw_cpu2_ble[0]
#else
#define BLE_FW_ADDRESS   0U
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#define SDK_VERSION "NXPSDK_v1.3.r13.p1"

static void uart_init_crc32(struct uart_cb *uartcb)
{
	int i, j;
	unsigned int c;

	for (i = 0; i < 256; ++i) {
		for (c = i << 24, j = 8; j > 0; --j) {
			c = c & 0x80000000 ? (c << 1) ^ CRC32_POLY : (c << 1);
		}
		uartcb->crc32_table[i] = c;
	}
}

static uint32_t uart_get_crc32(struct uart_cb *uart, int len, unsigned char *buf)
{
	unsigned int *crc32_table = uart->crc32_table;
	unsigned char *p;
	unsigned int crc;

	crc = 0xffffffff;

	for (p = buf; len > 0; ++p, --len) {
		crc = (crc << 8) ^ (crc32_table[(crc >> 24) ^ *p]);
	}
	return ~crc;
}

/*
 * send_response_to_uart() handles the response from the firmware.
 * This involves
 * 1. replacing the sdio header with the uart header
 * 2. computation of the crc of the payload
 * 3. sending it out to the uart
 */
static int send_response_to_uart(struct uart_cb *uart, const uint8_t *resp, int type)
{
	uint32_t bridge_chksum = 0;
	uint32_t msglen;
	int index;
	uint32_t payloadlen;
	struct uart_header *uart_hdr;
	struct SDIOPkt *sdio = (struct SDIOPkt *)resp;

	int iface_len;

	if (type == 2) {
		/* This is because, the last byte of the sdio header
		 * (packet type) is also required by the labtool, to
		 * understand the type of packet and take appropriate action
		 */
		iface_len = INTF_HEADER_LEN - 1;
	} else {
		iface_len = INTF_HEADER_LEN;
	}

	payloadlen = sdio->size - iface_len;
	(void)memset(rx_buf, 0, BUF_LEN);
	(void)memcpy(rx_buf + sizeof(struct uart_header) + sizeof(struct cmd_header),
		     resp + iface_len, payloadlen);

	/* Added to send correct cmd header len */
	struct cmd_header *cmd_hdr;

	cmd_hdr = &last_cmd_hdr;
	cmd_hdr->length = payloadlen + sizeof(struct cmd_header);

	(void)memcpy(rx_buf + sizeof(struct uart_header), (uint8_t *)&last_cmd_hdr,
		     sizeof(struct cmd_header));

	uart_hdr = (struct uart_header *)rx_buf;
	uart_hdr->length = payloadlen + sizeof(struct cmd_header);
	uart_hdr->pattern = 0x5555;

	/* calculate CRC. The uart_header is excluded */
	msglen = payloadlen + sizeof(struct cmd_header);
	bridge_chksum = uart_get_crc32(uart, msglen, rx_buf + sizeof(struct uart_header));
	index = sizeof(struct uart_header) + msglen;

	rx_buf[index] = bridge_chksum & 0xff;
	rx_buf[index + 1] = (bridge_chksum & 0xff00) >> 8;
	rx_buf[index + 2] = (bridge_chksum & 0xff0000) >> 16;
	rx_buf[index + 3] = (bridge_chksum & 0xff000000) >> 24;

	/* write response to uart */
	uart_rtos_send(&uart_handle, rx_buf,
		       payloadlen + sizeof(struct cmd_header) + sizeof(struct uart_header) + 4);

	(void)memset(rx_buf, 0, BUF_LEN);

	return 0;
}

/*
 * check_command_complete() validates the command from the uart.
 * It checks for the signature in the header and the crc of the
 * payload. This assumes that the uart_buf is circular and data
 * can be wrapped.
 */
int check_command_complete(uint8_t *buf)
{
	struct uart_header *uarthdr;
	uint32_t msglen, endofmsgoffset;
	struct uart_cb *uart = &uartcb;
	int checksum = 0, bridge_checksum = 0;

	uarthdr = (struct uart_header *)buf;

	/* out of sync */
	if (uarthdr->pattern != 0x5555) {
		LOG_ERR("Pattern mismatch");
		return -WM_FAIL;
	}
	/* check crc */
	msglen = uarthdr->length;

	/* add 4 for checksum */
	endofmsgoffset = sizeof(struct uart_header) + msglen + 4;

	(void)memset((uint8_t *)local_outbuf, 0, sizeof(*local_outbuf));
	if (endofmsgoffset < UART_BUF_SIZE) {
		(void)memcpy((uint8_t *)local_outbuf, buf, endofmsgoffset);
	} else {
		(void)memcpy((uint8_t *)local_outbuf, buf, UART_BUF_SIZE);
		/* To do : check if copying method is correct */
		(void)memcpy((uint8_t *)local_outbuf + UART_BUF_SIZE, buf, endofmsgoffset);
	}

	checksum = *(int *)((uint8_t *)local_outbuf + sizeof(struct uart_header) + msglen);

	bridge_checksum =
		uart_get_crc32(uart, msglen, (uint8_t *)local_outbuf + sizeof(struct uart_header));
	if (checksum == bridge_checksum) {
		return WM_SUCCESS;
	}
	/* Reset local outbuf */
	(void)memset(local_outbuf, 0, BUF_LEN);

	return -WM_FAIL;
}

hal_rpmsg_status_t  wifi_send_imu_raw_data(uint8_t *data, uint32_t length)
{
	if (data == NULL || length == 0) {
		return kStatus_HAL_RpmsgError;
	}

	if (kStatus_HAL_RpmsgSuccess != (HAL_ImuSendCommand(kIMU_LinkCpu1Cpu3, data, length))) {
		return kStatus_HAL_RpmsgError;
	}

	return kStatus_HAL_RpmsgSuccess;
}

int rpmsg_raw_packet_send(uint8_t *buf, int m_len, uint8_t t_type)
{
	uint32_t payloadlen;

	struct cmd_header *cmd_hd = (struct cmd_header *)(buf + sizeof(struct uart_header));

	payloadlen = m_len - sizeof(struct uart_header) - sizeof(struct cmd_header) - 4;

	(void)memset(local_outbuf, 0, BUF_LEN);
	(void)memcpy(local_outbuf, buf + sizeof(struct uart_header) + sizeof(struct cmd_header),
		     payloadlen);

	(void)memcpy(&last_cmd_hdr, cmd_hd, sizeof(struct cmd_header));

    if (kStatus_HAL_RpmsgSuccess !=
        (HAL_RpmsgSend((hal_rpmsg_handle_t)rpmsgHandleList[t_type - 2], local_outbuf, payloadlen))) {
		return kStatus_HAL_RpmsgError;
	}

	(void)memset(local_outbuf, 0, BUF_LEN);

	return t_type;
}

/*
 * process_input_cmd() sends command to the wlan
 * card
 */
int process_input_cmd(uint8_t *buf, int m_len)
{
	struct uart_header *uarthdr;
	int i, ret = -WM_FAIL;
	uint8_t *s, *d;
	struct cmd_header *cmd_hd = (struct cmd_header *)(buf + sizeof(uarthdr));

	if (cmd_hd->type == TYPE_WLAN) {
		(void)memset(local_outbuf, 0, BUF_LEN);
		sdiopkt = (struct SDIOPkt *)local_outbuf;

		uarthdr = (struct uart_header *)buf;

		/* sdiopkt = local_outbuf */
		sdiopkt->pkttype = SDIOPKTTYPE_CMD;

		sdiopkt->size = m_len - sizeof(struct cmd_header) + INTF_HEADER_LEN;
		d = (uint8_t *)local_outbuf + INTF_HEADER_LEN;
		s = (uint8_t *)buf + sizeof(struct uart_header) + sizeof(struct cmd_header);

		for (i = 0; i < uarthdr->length - sizeof(struct cmd_header); i++) {
			if (s < buf + UART_BUF_SIZE) {
				*d++ = *s++;
			} else {
				s = buf;
				*d++ = *s++;
			}
		}

		d = (uint8_t *)&last_cmd_hdr;
		s = (uint8_t *)buf + sizeof(struct uart_header);

		for (i = 0; i < sizeof(struct cmd_header); i++) {
			if (s < buf + UART_BUF_SIZE) {
				*d++ = *s++;
			} else {
				s = buf;
				*d++ = *s++;
			}
		}
		wifi_send_imu_raw_data(local_outbuf,
				       (m_len - sizeof(struct cmd_header) + INTF_HEADER_LEN));

		ret = RET_TYPE_WLAN;
	} else if (cmd_hd->type == TYPE_BT) {
		ret = rpmsg_raw_packet_send(buf, m_len, RET_TYPE_BT);
	} else if (cmd_hd->type == TYPE_15_4) {
		ret = rpmsg_raw_packet_send(buf, m_len, RET_TYPE_ZIGBEE);
	}

	return ret;
}

void send_rpmsg_response_to_uart(uint8_t *resp, int msg_len)
{
	uint32_t bridge_chksum = 0;
	uint32_t msglen;
	int index;
	uint32_t payloadlen;
	struct uart_header *uart_hdr;
	struct uart_cb *uart = &uartcb;

	payloadlen = msg_len;

	(void)memset(rx_buf, 0, BUF_LEN);
	(void)memcpy(rx_buf + sizeof(struct uart_header) + sizeof(struct cmd_header), resp,
		     payloadlen);

	/* Added to send correct cmd header len */
	struct cmd_header *cmd_hdr;

	cmd_hdr = &last_cmd_hdr;
	cmd_hdr->length = payloadlen + sizeof(struct cmd_header);

	(void)memcpy(rx_buf + sizeof(struct uart_header), (uint8_t *)&last_cmd_hdr,
		     sizeof(struct cmd_header));

	uart_hdr = (struct uart_header *)rx_buf;
	uart_hdr->length = payloadlen + sizeof(struct cmd_header);
	uart_hdr->pattern = 0x5555;

	/* calculate CRC. The uart_header is excluded */
	msglen = payloadlen + sizeof(struct cmd_header);
	bridge_chksum = uart_get_crc32(uart, msglen, rx_buf + sizeof(struct uart_header));
	index = sizeof(struct uart_header) + msglen;

	rx_buf[index] = bridge_chksum & 0xff;
	rx_buf[index + 1] = (bridge_chksum & 0xff00) >> 8;
	rx_buf[index + 2] = (bridge_chksum & 0xff0000) >> 16;
	rx_buf[index + 3] = (bridge_chksum & 0xff000000) >> 24;

	/* write response to uart */
	uart_rtos_send(&uart_handle, rx_buf,
		       payloadlen + sizeof(struct cmd_header) + sizeof(struct uart_header) + 4);

	(void)memset(rx_buf, 0, BUF_LEN);
}

/*
 * It waits on wlan card interrupts on account
 * of command responses are handled here. The response is
 * read and then sent through the uart to the Mfg application
 */
hal_rpmsg_status_t  read_wlan_resp(IMU_Msg_t *pImuMsg, uint32_t len)
{
	assert(pImuMsg != NULL);
	assert(len != 0);
	assert(pImuMsg->Hdr.type == IMU_MSG_COMMAND_RESPONSE);

	struct uart_cb *uart = &uartcb;

	send_response_to_uart(uart, (uint8_t *)(pImuMsg->PayloadPtr[0]), 1);

	return kStatus_HAL_RpmsgSuccess;
}

hal_rpmsg_return_status_t read_rpmsg_resp(void *param, uint8_t *packet, uint32_t len)
{
	assert(packet != NULL);
	assert(len != 0);

	send_rpmsg_response_to_uart(packet, len);

	return kStatus_HAL_RL_RELEASE;
}

static hal_rpmsg_status_t  imu_wifi_config(void)
{
	hal_rpmsg_status_t  state = kStatus_HAL_RpmsgSuccess;

	/* Assign IMU channel for CPU1-CPU3 communication */
	state = HAL_ImuInit(kIMU_LinkCpu1Cpu3);
	assert(kStatus_HAL_RpmsgSuccess == state);

	HAL_ImuInstallCallback(kIMU_LinkCpu1Cpu3, read_wlan_resp, IMU_MSG_COMMAND_RESPONSE);

	return state;
}

#if (defined(CONFIG_SUPPORT_BLE) && (CONFIG_SUPPORT_BLE == 1)) || \
	(defined(CONFIG_SUPPORT_BLE_15D4) && (CONFIG_SUPPORT_BLE_15D4 == 1))
static hal_rpmsg_status_t  rpmsg_config(uint32_t linkId)
{
	hal_rpmsg_status_t  state = kStatus_HAL_RpmsgSuccess;

	hal_rpmsg_config_t  config = {0};
	/* Init RPMSG/IMU Channel */
	config.local_addr = local_ept_list[linkId];
	config.remote_addr = remote_ept_list[linkId];
	config.imuLink = kIMU_LinkCpu2Cpu3;
    state              = HAL_RpmsgInit((hal_rpmsg_handle_t)rpmsgHandleList[linkId], &config);
    assert(kStatus_HAL_RpmsgSuccess == state);

	/* RPMSG install rx callback */
    state = HAL_RpmsgInstallRxCallback((hal_rpmsg_handle_t)rpmsgHandleList[linkId], read_rpmsg_resp, NULL);
    assert(kStatus_HAL_RpmsgSuccess == state);

	return state;
}
#endif

static hal_rpmsg_status_t  rpmsg_init(void)
{
#if (defined(CONFIG_SUPPORT_BLE) && (CONFIG_SUPPORT_BLE == 1)) ||                                  \
	(defined(CONFIG_SUPPORT_BLE_15D4) && (CONFIG_SUPPORT_BLE_15D4 == 1))
	uint32_t linkId;
#endif
	hal_rpmsg_status_t  state = kStatus_HAL_RpmsgSuccess;

	/* Init RPMSG/IMU Channel */
#if defined(CONFIG_SUPPORT_BLE) && (CONFIG_SUPPORT_BLE == 1)
	linkId = 0;
	state = rpmsg_config(linkId);
#endif
#if defined(CONFIG_SUPPORT_BLE_15D4) && (CONFIG_SUPPORT_BLE_15D4 == 1)
	linkId = 1;
	state = rpmsg_config(linkId);
#endif

	return state;
}

#define RW610_PACKAGE_TYPE_QFN 0
#define RW610_PACKAGE_TYPE_CSP 1
#define RW610_PACKAGE_TYPE_BGA 2

void wifi_cau_temperature_enable(void)
{
	uint32_t val;

	val = WIFI_REG32(WLAN_CAU_ENABLE_ADDR);
	val &= ~(0xC);
	val |= (2 << 2);
	WIFI_WRITE_REG32(WLAN_CAU_ENABLE_ADDR, val);
}

static uint32_t wifi_get_board_type(void)
{
	status_t status;
	static uint32_t wifi_rw610_package_type = 0xFFFFFFFF;

	if (wifi_rw610_package_type == 0xFFFFFFFF) {
		OCOTP_OtpInit();
		status = OCOTP_ReadPackage(&wifi_rw610_package_type);
		if (status != kStatus_Success) {
			/*If status error, use BGA as default type*/
			wifi_rw610_package_type = RW610_PACKAGE_TYPE_BGA;
		}
		OCOTP_OtpDeinit();
	}

	return wifi_rw610_package_type;
}

int32_t wifi_get_temperature(void)
{
	int32_t val = 0;
	uint32_t reg_val = 0;
	uint32_t temp_Cau_Raw_Reading = 0;
	uint32_t board_type = 0;

	reg_val = WIFI_REG32(WLAN_CAU_TEMPERATURE_ADDR);
	temp_Cau_Raw_Reading = ((reg_val & 0XFFC00) >> 10);
	board_type = wifi_get_board_type();

	switch (board_type) {
	case RW610_PACKAGE_TYPE_QFN:
		val = (((((int32_t)(temp_Cau_Raw_Reading)) * 484260) - 220040600) / 1000000);
		break;

	case RW610_PACKAGE_TYPE_CSP:
		val = (((((int32_t)(temp_Cau_Raw_Reading)) * 480560) - 220707000) / 1000000);
		break;

	case RW610_PACKAGE_TYPE_BGA:
		val = (((((int32_t)(temp_Cau_Raw_Reading)) * 480561) - 220707400) / 1000000);
		break;

	default:
		PRINTF("Unknown board type, use BGA temperature \r\n");
		val = (((((int32_t)(temp_Cau_Raw_Reading)) * 480561) - 220707400) / 1000000);
		break;
	}

	return val;
}

void wifi_cau_temperature_write_to_firmware()
{
	int32_t val = 0;

	val = wifi_get_temperature();
	WIFI_WRITE_REG32(WLAN_CAU_TEMPERATURE_FW_ADDR, val);
}

static void wifi_cau_temperature_timer_cb(struct k_timer *timer)
{
	/* write CAU temperature to CPU1 when it is not sleeping */
	if ((WIFI_REG32(WLAN_FW_WAKE_STATUS_ADDR) & 0x0CU) != 0x0CU) {
		wifi_cau_temperature_write_to_firmware();
	}
}

extern void WL_MCI_WAKEUP0_DriverIRQHandler(void);
extern void BLE_MCI_WAKEUP0_DriverIRQHandler(void);

/*
 * task_main() runs in a loop. It polls the uart ring buffer
 * checks it for a complete command and sends the command to the
 * wlan card
 */
static void task_main(void)
{
	int32_t result = 0;
	(void)result;

	/* Enable IMU/RPMSG IRQ */
	IRQ_CONNECT(72, 1, WL_MCI_WAKEUP0_DriverIRQHandler, 0, 0);
	irq_enable(72);

	IRQ_CONNECT(90, 1, BLE_MCI_WAKEUP0_DriverIRQHandler, 0, 0);
	irq_enable(90);

	/* Initialize uart */
	result = uart_rtos_init(&uart_handle);
	if (result < 0) {
		LOG_ERR("Failed to initialize uart");
		return;
	}

	local_outbuf = pvPortMalloc(SDIO_OUTBUF_LEN);

	if (local_outbuf == NULL) {
		LOG_ERR("Failed to allocate buffer");
		return;
	}
	rx_buf = pvPortMalloc(BUF_LEN);
	if (rx_buf == NULL) {
		LOG_ERR("Failed to allocate buffer");
		return;
	}

	struct uart_cb *uart = &uartcb;

	uart_init_crc32(uart);

	/* Download firmware */
#if (CONFIG_SUPPORT_WIFI == 0) && (CONFIG_SUPPORT_BLE_15D4 == 0) && (CONFIG_SUPPORT_BLE == 0)
#error "One of CONFIG_SUPPORT_WIFI CONFIG_SUPPORT_BLE_15D4 and CONFIG_SUPPORT_BLE \
				should be defined, or it will not download any formware!!"
#endif
#if defined(CONFIG_SUPPORT_WIFI) && (CONFIG_SUPPORT_WIFI == 1)
	sb3_fw_download(LOAD_WIFI_FIRMWARE, 1, WIFI_FW_ADDRESS);
#endif

	wifi_cau_temperature_enable();
	wifi_cau_temperature_write_to_firmware();

	/* 15d4 single and 15d4+ble combo */
#if defined(CONFIG_SUPPORT_BLE_15D4) && (CONFIG_SUPPORT_BLE_15D4 == 1)
	sb3_fw_download(LOAD_15D4_FIRMWARE, 1, COMBO_FW_ADDRESS);
#endif
	/* only ble, no 15d4 */
#if defined(CONFIG_SUPPORT_BLE_15D4) && (CONFIG_SUPPORT_BLE_15D4 == 0) && defined(CONFIG_SUPPORT_BLE) &&   \
	(CONFIG_SUPPORT_BLE == 1)
	sb3_fw_download(LOAD_BLE_FIRMWARE, 1, BLE_FW_ADDRESS);
#endif

	/* Initialize WIFI Driver */
	imu_wifi_config();

	/* Initialize rpmsg */
	rpmsg_init();

	k_timer_init(&g_wifi_cau_temperature_timer, wifi_cau_temperature_timer_cb, NULL);
	k_timer_start(&g_wifi_cau_temperature_timer, K_MSEC(5000), K_MSEC(5000));

	size_t uart_rx_len = 0;
	int len = 0;
	int msg_len = 0;

	while (1) {
		len = 0;
		msg_len = 0;
		uart_rx_len = 0;
		(void)memset(uart->uart_buf, 0, sizeof(uart->uart_buf));
		while (len != LABTOOL_PATTERN_HDR_LEN) {
			uart_rtos_recv(&uart_handle, uart->uart_buf + len, LABTOOL_PATTERN_HDR_LEN,
				       &uart_rx_len);
			len += uart_rx_len;
		}

		/* Length of the packet is indicated by byte[2] & byte[3] of
		 * the packet excluding header[4 bytes] + checksum [4 bytes]
		 */
		msg_len = (uart->uart_buf[3] << 8) + uart->uart_buf[2];
		len = 0;
		uart_rx_len = 0;
		while (len != msg_len + CHECKSUM_LEN) {
			uart_rtos_recv(&uart_handle, uart->uart_buf + LABTOOL_PATTERN_HDR_LEN + len,
				       msg_len + CHECKSUM_LEN - len, &uart_rx_len);
			len += uart_rx_len;
		}

		/* validate the command including checksum */
		if (check_command_complete(uart->uart_buf) == WM_SUCCESS) {
			/* send fw cmd over SDIO after
			 * stripping off uart header
			 */
			int ret = process_input_cmd(uart->uart_buf, msg_len + 8);
			(void)memset(uart->uart_buf, 0, sizeof(uart->uart_buf));

			UNUSED(ret);
		} else {
			(void)memset(uart_handle.rx.buffer, 0, BUFFER_SIZE);
			(void)memset(uart_handle.tx.buffer, 0, BUFFER_SIZE);
		}
	}
}

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

int main(void)
{
	task_main();
	return 0;
}
