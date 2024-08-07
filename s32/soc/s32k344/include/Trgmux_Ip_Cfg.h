/*
 * Copyright 2020-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TRGMUX_IP_CFG_H_
#define TRGMUX_IP_CFG_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Trgmux_Ip_Types.h"
#include "Trgmux_Ip_Init_PBcfg.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define TRGMUX_IP_CFG_VENDOR_ID                    43
#define TRGMUX_IP_CFG_MODULE_ID                    255
#define TRGMUX_IP_CFG_AR_RELEASE_MAJOR_VERSION     4
#define TRGMUX_IP_CFG_AR_RELEASE_MINOR_VERSION     7
#define TRGMUX_IP_CFG_AR_RELEASE_REVISION_VERSION  0
#define TRGMUX_IP_CFG_SW_MAJOR_VERSION             3
#define TRGMUX_IP_CFG_SW_MINOR_VERSION             0
#define TRGMUX_IP_CFG_SW_PATCH_VERSION             0

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
/* Check if header file and Trgmux_Ip_Types.h file are of the same vendor */
#if (TRGMUX_IP_CFG_VENDOR_ID != TRGMUX_IP_TYPES_VENDOR_ID)
    #error "Trgmux_Ip_Cfg.h and Trgmux_Ip_Types.h have different vendor ids"
#endif

/* Check if header file and Trgmux_Ip_Types.h file are of the same Autosar version */
#if ((TRGMUX_IP_CFG_AR_RELEASE_MAJOR_VERSION != TRGMUX_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (TRGMUX_IP_CFG_AR_RELEASE_MINOR_VERSION != TRGMUX_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (TRGMUX_IP_CFG_AR_RELEASE_REVISION_VERSION != TRGMUX_IP_TYPES_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Trgmux_Ip_Cfg.h and Trgmux_Ip_Types.h are different"
#endif

/* Check if header file and Trgmux_Ip_Types.h file are of the same Software version */
#if ((TRGMUX_IP_CFG_SW_MAJOR_VERSION != TRGMUX_IP_TYPES_SW_MAJOR_VERSION) || \
     (TRGMUX_IP_CFG_SW_MINOR_VERSION != TRGMUX_IP_TYPES_SW_MINOR_VERSION) || \
     (TRGMUX_IP_CFG_SW_PATCH_VERSION != TRGMUX_IP_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Trgmux_Ip_Cfg.h and Trgmux_Ip_Types.h are different"
#endif
/* Check if header file and Trgmux_Ip_Init_PBcfg.h file are of the same vendor */
#if (TRGMUX_IP_CFG_VENDOR_ID != TRGMUX_IP_INIT_PBCFG_VENDOR_ID)
    #error "Trgmux_Ip_Cfg.h and Trgmux_Ip_Init_PBcfg.h have different vendor ids"
#endif

/* Check if header file and Trgmux_Ip_Init_PBcfg.h file are of the same Autosar version */
#if ((TRGMUX_IP_CFG_AR_RELEASE_MAJOR_VERSION != TRGMUX_IP_INIT_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (TRGMUX_IP_CFG_AR_RELEASE_MINOR_VERSION != TRGMUX_IP_INIT_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (TRGMUX_IP_CFG_AR_RELEASE_REVISION_VERSION != TRGMUX_IP_INIT_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Trgmux_Ip_Cfg.h and Trgmux_Ip_Init_PBcfg.h are different"
#endif

/* Check if header file and Trgmux_Ip_INIT_PBcfg.h file are of the same Software version */
#if ((TRGMUX_IP_CFG_SW_MAJOR_VERSION != TRGMUX_IP_INIT_PBCFG_SW_MAJOR_VERSION) || \
     (TRGMUX_IP_CFG_SW_MINOR_VERSION != TRGMUX_IP_INIT_PBCFG_SW_MINOR_VERSION) || \
     (TRGMUX_IP_CFG_SW_PATCH_VERSION != TRGMUX_IP_INIT_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Trgmux_Ip_Cfg.h and Trgmux_Ip_Init_PBcfg.h are different"
#endif

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

#define MCL_START_SEC_CONST_8
#include "Mcl_MemMap.h"

extern const uint8 Trgmux_Ip_InstanceArr[TRGMUX_IP_NOF_INSTANCE];

#define MCL_STOP_SEC_CONST_8
#include "Mcl_MemMap.h"


/* TRGMUX Init Configuration */
extern const Trgmux_Ip_InitType Trgmux_Ip_xTrgmuxInitPB;

#ifdef __cplusplus
}
#endif

#endif /* TRGMUX_IP_CFG_H_ */

/*==================================================================================================
 *                                        END OF FILE
==================================================================================================*/
