/**
 *  Copyright 2023-2024 NXP
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  @file  usart.c
 *  @brief This file provides uart API
 */

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(mfg_bridge);

#include "uart_rtos.h"

const struct device *uart_rtos_dev = DEVICE_DT_GET(DT_NODELABEL(flexcomm3));

struct uart_config uart_rtos_cfg = {
	.baudrate = 115200,
	.parity = UART_CFG_PARITY_NONE,
	.stop_bits = UART_CFG_STOP_BITS_1,
	.data_bits = UART_CFG_DATA_BITS_8,
	.flow_ctrl = UART_CFG_FLOW_CTRL_NONE,
};

static void uart_rtos_cb(const struct device *dev, void *user_data);

int uart_rtos_init(struct uart_rtos_state *handle)
{
	int ret = 0;

	if (handle == NULL) {
		return -1;
	}

	/* check uart device */
	if (!device_is_ready(uart_rtos_dev)) {
		LOG_ERR("UART device not found!");
		return -1;
	}

	if (uart_configure(uart_rtos_dev, &uart_rtos_cfg) != 0) {
		LOG_ERR("UART config error!");
		return -1;
	}

	(void)memset(handle, 0, sizeof(struct uart_rtos_state));
	handle->dev = uart_rtos_dev;
	handle->rx.bufferSize = BUFFER_SIZE;
	handle->tx.bufferSize = BUFFER_SIZE;
	k_sem_init(&handle->rx.sem, 0, K_SEM_MAX_LIMIT);
	k_sem_init(&handle->tx.sem, BUFFER_SIZE - 1, K_SEM_MAX_LIMIT);

	/* configure interrupt and callback to receive data */
	ret = uart_irq_callback_user_data_set(uart_rtos_dev, uart_rtos_cb, handle);
	if (ret < 0) {
		if (ret == -ENOTSUP) {
			LOG_ERR("Interrupt-driven UART API support not enabled");
		} else if (ret == -ENOSYS) {
			LOG_ERR("UART device does not support interrupt-driven API");
		} else {
			LOG_ERR("Error setting UART callback: %d", ret);
		}
		return -1;
	}

	uart_irq_rx_enable(uart_rtos_dev);

	return 0;
}

static void uart_rtos_cb(const struct device *dev, void *user_data)
{
	uint8_t rxdata;
	uint8_t rx_index;
	struct uart_rtos_state *handle = (struct uart_rtos_state *)user_data;

	if (dev == NULL) {
		return;
	}

	if (!uart_irq_update(dev)) {
		return;
	}

	/* Receive */
	if (uart_irq_rx_ready(dev)) {
		if (uart_fifo_read(dev, &rxdata, 1)) {
			rx_index = handle->rx.bufferHead + 1;
			if (rx_index >= handle->rx.bufferSize) {
				rx_index = 0;
			}

			if (rx_index == handle->rx.bufferTail) {
				goto send;
			}

			handle->rx.buffer[handle->rx.bufferHead] = rxdata;
			handle->rx.bufferHead = rx_index;
			k_sem_give(&handle->rx.sem);
		}
	}

send:
	/* Send */
	if (uart_irq_tx_ready(dev)) {
		if (handle->tx.bufferHead == handle->tx.bufferTail) {
			uart_irq_tx_disable(dev);
		} else {
			uart_fifo_fill(dev, &handle->tx.buffer[handle->tx.bufferTail++], 1);
			if (handle->tx.bufferTail >= handle->tx.bufferSize) {
				handle->tx.bufferTail = 0;
			}
			k_sem_give(&handle->tx.sem);
		}
	}
}

static uint8_t uart_rtos_transfer_recv(struct uart_rtos_state *handle)
{
	unsigned int irq_key = 0;
	uint8_t data = 0;

	k_sem_take(&handle->rx.sem, K_FOREVER);

	/* Disable IRQ, protect ring buffer. */
	irq_key = irq_lock();

	data = handle->rx.buffer[handle->rx.bufferTail++];

	if (handle->rx.bufferTail >= handle->rx.bufferSize) {
		handle->rx.bufferTail = 0U;
	}

	irq_unlock(irq_key);

	return data;
}

int uart_rtos_recv(struct uart_rtos_state *handle, void *buf, uint32_t length, size_t *received)
{
	size_t n = 0;
	uint8_t *data = buf;

	if (handle == NULL || buf == NULL) {
		return -1;
	}

	if (length == 0) {
		if (received != NULL) {
			*received = n;
		}
		return 0;
	}

	while (length) {
		*data++ = uart_rtos_transfer_recv(handle);
		++n;
		--length;
	}

	/* Prevent repetitive NULL check */
	if (received != NULL) {
		*received = n;
	}

	return 0;
}

static int uart_rtos_transfer_send(struct uart_rtos_state *handle, uint8_t data)
{
	unsigned int irq_key = 0;
	uint16_t tx_index = 0;

	k_sem_take(&handle->tx.sem, K_FOREVER);

	/* Disable IRQ, protect ring buffer. */
	irq_key = irq_lock();

	tx_index = handle->tx.bufferHead + 1;

	if (tx_index >= handle->tx.bufferSize) {
		tx_index = 0;
	}

	if (tx_index == handle->tx.bufferTail) {
		irq_unlock(irq_key);
		return -1;
	}

	handle->tx.buffer[handle->tx.bufferHead] = data;
	handle->tx.bufferHead = tx_index;

	irq_unlock(irq_key);
	uart_irq_tx_enable(uart_rtos_dev);

	return 0;
}

int uart_rtos_send(struct uart_rtos_state *handle, void *buf, uint32_t length)
{
	uint8_t *data = buf;

	if (handle == NULL || buf == NULL) {
		return -1;
	}

	if (length == 0) {
		return 0;
	}

	while (length) {
		uart_rtos_transfer_send(handle, *data++);
		--length;
	}

	return 0;
}
