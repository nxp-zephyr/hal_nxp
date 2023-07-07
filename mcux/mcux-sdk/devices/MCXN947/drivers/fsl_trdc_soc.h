/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_TRDC_SOC_H_
#define _FSL_TRDC_SOC_H_

#include "fsl_common.h"

/*!
 * @addtogroup trdc_soc
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @name Driver version */
/*@{*/
/*! @brief Driver version 2.0.0. */
#define FSL_TRDC_SOC_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/

/*!@brief TRDC base table */
#define TRDC_BASE_PTRS \
    {                  \
        TRDC           \
    }
/* @} */

#define TRDC_MBC_MEM_GLBCFG_NBLKS_MASK      TRDC_MBC_INDEX_MBC_MEM_GLBCFG_NBLKS_MASK
#define TRDC_MBC_MEM_GLBCFG_SIZE_LOG2_MASK  TRDC_MBC_INDEX_MBC_MEM_GLBCFG_SIZE_LOG2_MASK
#define TRDC_MBC_MEM_GLBCFG_SIZE_LOG2_SHIFT TRDC_MBC_INDEX_MBC_MEM_GLBCFG_SIZE_LOG2_SHIFT
#define TRDC_MBC_NSE_BLK_CLR_ALL_MEMSEL     TRDC_MBC_INDEX_MBC_NSE_BLK_CLR_ALL_MEMSEL
#define TRDC_MBC_NSE_BLK_CLR_ALL_DID_SEL    TRDC_MBC_INDEX_MBC_NSE_BLK_CLR_ALL_DID_SEL0

/*!@brief TRDC feature */
#define FSL_FEATURE_TRDC_DOMAIN_COUNT 1
#define FSL_FEATURE_TRDC_HAS_MBC 1
#define FSL_FEATURE_TRDC_HAS_MRC 0

/*!@brief TRDC base address convert macro */
#define TRDC_MBC_OFFSET(x)  0x1000
#define TRDC_MBC_ARRAY_STEP 0U

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*!
 * @}
 */

#endif /* _FSL_TRDC_SOC_H_ */
