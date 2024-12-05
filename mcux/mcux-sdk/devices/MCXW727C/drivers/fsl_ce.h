/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*==========================================================================
Functional API definitions for ARM drivers for CE
==========================================================================*/
#ifndef FSL_CE_H
#define FSL_CE_H

#include "fsl_common.h"
#include "fsl_ce_basic.h"
#include "fsl_ce_cmsis.h"
#include "fsl_ce_matrix.h"
#include "fsl_ce_transform.h"

/*!
 * @defgroup ce Computer Engine (CE) Driver
 */

/*! @name Driver version */
/*! @{ */
/*! @brief CE driver version. */
#define FSL_CE_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*! @} */

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.ce"
#endif

#endif /*FSL_CE_H*/
