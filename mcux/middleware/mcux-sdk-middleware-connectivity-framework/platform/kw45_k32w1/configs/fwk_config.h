/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FWK_FRAMEWORK_CONFIG_H_
#define _FWK_FRAMEWORK_CONFIG_H_

#if !defined(gPlatformUseHwParameter_d)
#define gPlatformUseHwParameter_d 1
#endif

/* Defines the calibration duration of the ADC, it will block the task during this time in milisec before trigger the
 * ADC on a channel*/
#define gSensorsAdcCalibrationDurationInMs_c 4U

#define gPlatformRamStartAddress_c (0x20000000U)
#define gPlatformRamEndAddress_c   (0x2001BFFFU)

#define gPlatformFlashStartAddress_c (FSL_FEATURE_FLASH_PFLASH_START_ADDRESS)
#define gPlatformFlashEndAddress_c   (FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE)

#endif /* _FWK_PLATFORM_H_ */
