/*
 * Copyright 2014-2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __FSL_DEVICE_REGISTERS_H__
#define __FSL_DEVICE_REGISTERS_H__

/*
 * Include the cpu specific register header files.
 *
 * The CPU macro should be declared in the project or makefile.
 */
#if (defined(CPU_MIMX9322CVWXM_ca55) || defined(CPU_MIMX9322DVWXM_ca55) || defined(CPU_MIMX9322XVWXM_ca55))

#define MIMX9322_ca55_SERIES

/* CMSIS-style register definitions */
#include "MIMX9322_ca55.h"
/* CPU specific feature definitions */
#include "MIMX9322_ca55_features.h"

#elif (defined(CPU_MIMX9322CVWXM_cm33) || defined(CPU_MIMX9322DVWXM_cm33) || defined(CPU_MIMX9322XVWXM_cm33))

#define MIMX9322_cm33_SERIES

/* CMSIS-style register definitions */
#include "MIMX9322_cm33.h"
/* CPU specific feature definitions */
#include "MIMX9322_cm33_features.h"

#else
    #error "No valid CPU defined!"
#endif

#endif /* __FSL_DEVICE_REGISTERS_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
