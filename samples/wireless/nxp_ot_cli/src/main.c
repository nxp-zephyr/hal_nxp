/**
 *  Copyright 2024, NXP
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  @file  main.c
 *  @brief main file
 */

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(nxp_ot_cli, CONFIG_OT_COMMAND_LINE_INTERFACE_LOG_LEVEL);

int main(void)
{
	LOG_INF("nxp_ot_cli entry");

	return 0;
}
