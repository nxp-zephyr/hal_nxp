/*
 * Copyright 2017-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MFLASH_DRV_H__
#define __MFLASH_DRV_H__

#include "mflash_common.h"

/* Flash constants */

#ifndef MFLASH_SECTOR_SIZE
#define MFLASH_SECTOR_SIZE (4096)
#endif

#ifndef MFLASH_PAGE_SIZE
#define MFLASH_PAGE_SIZE (256)
#endif

#ifndef FLASH_USE_CUSTOM_FCB
#define FLASH_USE_CUSTOM_FCB (0)
#endif

#define MFLASH_PAGE_INTEGRITY_CHECKS (1)

int32_t mflash_drv_is_readable(uint32_t addr);

#endif
