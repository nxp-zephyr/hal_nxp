/**
 *  Copyright 2023-2024 NXP
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 */
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <string.h>

#define BUFFER_SIZE 32

/* uart RX/TX state structure */
struct uart_rtos_recv_state {
	uint8_t buffer[BUFFER_SIZE];
	uint16_t bufferSize;
	uint16_t bufferHead;
	uint16_t bufferTail;
	struct k_sem sem;
};

/* uart state structure */
struct uart_rtos_state {
	const struct device *dev;
	struct uart_rtos_recv_state rx;
	struct uart_rtos_recv_state tx;
};

int uart_rtos_init(struct uart_rtos_state *uart_handle);
int uart_rtos_recv(struct uart_rtos_state *handle, void *buf, uint32_t length, size_t *received);
int uart_rtos_send(struct uart_rtos_state *handle, void *buf, uint32_t length);
