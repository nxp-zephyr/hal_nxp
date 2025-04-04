/*
 * Copyright 1997-2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file S32Z2_RTU_SEMA42.h
 * @version 2.3
 * @date 2024-05-03
 * @brief Peripheral Access Layer for S32Z2_RTU_SEMA42
 *
 * This file contains register definitions and macros for easy access to their
 * bit fields.
 *
 * This file assumes LITTLE endian system.
 */

/**
* @page misra_violations MISRA-C:2012 violations
*
* @section [global]
* Violates MISRA 2012 Advisory Rule 2.3, local typedef not referenced
* The SoC header defines typedef for all modules.
*
* @section [global]
* Violates MISRA 2012 Advisory Rule 2.5, local macro not referenced
* The SoC header defines macros for all modules and registers.
*
* @section [global]
* Violates MISRA 2012 Advisory Directive 4.9, Function-like macro
* These are generated macros used for accessing the bit-fields from registers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.1, identifier clash
* The supported compilers use more than 31 significant characters for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.2, identifier clash
* The supported compilers use more than 31 significant characters for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.4, identifier clash
* The supported compilers use more than 31 significant characters for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.5, identifier clash
* The supported compilers use more than 31 significant characters for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 21.1, defined macro '__I' is reserved to the compiler
* This type qualifier is needed to ensure correct I/O access and addressing.
*/

/* Prevention from multiple including the same memory map */
#if !defined(S32Z2_RTU_SEMA42_H_)  /* Check if memory map has not been already included */
#define S32Z2_RTU_SEMA42_H_

#include "S32Z2_COMMON.h"

/* ----------------------------------------------------------------------------
   -- RTU_SEMA42 Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RTU_SEMA42_Peripheral_Access_Layer RTU_SEMA42 Peripheral Access Layer
 * @{
 */

/** RTU_SEMA42 - Size of Registers Arrays */
#define RTU_SEMA42_GATE_COUNT                     16u

/** RTU_SEMA42 - Register Layout Typedef */
typedef struct {
  __IO uint8_t GATE[RTU_SEMA42_GATE_COUNT];        /**< Gate, array offset: 0x0, array step: 0x1 */
  uint8_t RESERVED_0[50];
  union {                                          /* offset: 0x42 */
    __I  uint16_t R;                                 /**< Reset Gate Read, offset: 0x42 */
    __O  uint16_t W;                                 /**< Reset Gate Write, offset: 0x42 */
  } RSTGT;
} RTU_SEMA42_Type, *RTU_SEMA42_MemMapPtr;

/** Number of instances of the RTU_SEMA42 module. */
#define RTU_SEMA42_INSTANCE_COUNT                (2u)

/* RTU_SEMA42 - Peripheral instance base addresses */
/** Peripheral RTU0__SEMA42 base address */
#define IP_RTU0__SEMA42_BASE                     (0x76110000u)
/** Peripheral RTU0__SEMA42 base pointer */
#define IP_RTU0__SEMA42                          ((RTU_SEMA42_Type *)IP_RTU0__SEMA42_BASE)
/** Peripheral RTU1__SEMA42 base address */
#define IP_RTU1__SEMA42_BASE                     (0x76910000u)
/** Peripheral RTU1__SEMA42 base pointer */
#define IP_RTU1__SEMA42                          ((RTU_SEMA42_Type *)IP_RTU1__SEMA42_BASE)
/** Array initializer of RTU_SEMA42 peripheral base addresses */
#define IP_RTU_SEMA42_BASE_ADDRS                 { IP_RTU0__SEMA42_BASE, IP_RTU1__SEMA42_BASE }
/** Array initializer of RTU_SEMA42 peripheral base pointers */
#define IP_RTU_SEMA42_BASE_PTRS                  { IP_RTU0__SEMA42, IP_RTU1__SEMA42 }

/* ----------------------------------------------------------------------------
   -- RTU_SEMA42 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RTU_SEMA42_Register_Masks RTU_SEMA42 Register Masks
 * @{
 */

/*! @name GATE - Gate */
/*! @{ */

#define RTU_SEMA42_GATE_GTFSM_MASK               (0xFU)
#define RTU_SEMA42_GATE_GTFSM_SHIFT              (0U)
#define RTU_SEMA42_GATE_GTFSM_WIDTH              (4U)
#define RTU_SEMA42_GATE_GTFSM(x)                 (((uint8_t)(((uint8_t)(x)) << RTU_SEMA42_GATE_GTFSM_SHIFT)) & RTU_SEMA42_GATE_GTFSM_MASK)
/*! @} */

/*! @name R - Reset Gate Read */
/*! @{ */

#define RTU_SEMA42_R_RSTGTN_MASK                 (0xFFU)
#define RTU_SEMA42_R_RSTGTN_SHIFT                (0U)
#define RTU_SEMA42_R_RSTGTN_WIDTH                (8U)
#define RTU_SEMA42_R_RSTGTN(x)                   (((uint16_t)(((uint16_t)(x)) << RTU_SEMA42_R_RSTGTN_SHIFT)) & RTU_SEMA42_R_RSTGTN_MASK)

#define RTU_SEMA42_R_RSTGMS_MASK                 (0xF00U)
#define RTU_SEMA42_R_RSTGMS_SHIFT                (8U)
#define RTU_SEMA42_R_RSTGMS_WIDTH                (4U)
#define RTU_SEMA42_R_RSTGMS(x)                   (((uint16_t)(((uint16_t)(x)) << RTU_SEMA42_R_RSTGMS_SHIFT)) & RTU_SEMA42_R_RSTGMS_MASK)

#define RTU_SEMA42_R_RSTGSM_MASK                 (0x3000U)
#define RTU_SEMA42_R_RSTGSM_SHIFT                (12U)
#define RTU_SEMA42_R_RSTGSM_WIDTH                (2U)
#define RTU_SEMA42_R_RSTGSM(x)                   (((uint16_t)(((uint16_t)(x)) << RTU_SEMA42_R_RSTGSM_SHIFT)) & RTU_SEMA42_R_RSTGSM_MASK)
/*! @} */

/*! @name W - Reset Gate Write */
/*! @{ */

#define RTU_SEMA42_W_RSTGTN_MASK                 (0xFFU)
#define RTU_SEMA42_W_RSTGTN_SHIFT                (0U)
#define RTU_SEMA42_W_RSTGTN_WIDTH                (8U)
#define RTU_SEMA42_W_RSTGTN(x)                   (((uint16_t)(((uint16_t)(x)) << RTU_SEMA42_W_RSTGTN_SHIFT)) & RTU_SEMA42_W_RSTGTN_MASK)

#define RTU_SEMA42_W_RSTGDP_MASK                 (0xFF00U)
#define RTU_SEMA42_W_RSTGDP_SHIFT                (8U)
#define RTU_SEMA42_W_RSTGDP_WIDTH                (8U)
#define RTU_SEMA42_W_RSTGDP(x)                   (((uint16_t)(((uint16_t)(x)) << RTU_SEMA42_W_RSTGDP_SHIFT)) & RTU_SEMA42_W_RSTGDP_MASK)
/*! @} */

/*!
 * @}
 */ /* end of group RTU_SEMA42_Register_Masks */

/*!
 * @}
 */ /* end of group RTU_SEMA42_Peripheral_Access_Layer */

#endif  /* #if !defined(S32Z2_RTU_SEMA42_H_) */
