/* -------------------------------------------------------------------------- */
/*                           Copyright 2020-2022 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

#ifndef _FWK_PLATFORM_H_
#define _FWK_PLATFORM_H_

/*!
 * @addtogroup FWK_Platform_module
 * The FWK_Platform module
 *
 * FWK_Platform module provides APIs to set platform parameters.
 * @{
 */
/*!
 * @addtogroup FWK_Platform
 * The FWK_Platform main module
 *
 * FWK_Platform main module provides APIs to set main platform parameters.
 * @{
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "EmbeddedTypes.h"

/* -------------------------------------------------------------------------- */
/*                                 Definitions                                */
/* -------------------------------------------------------------------------- */

#ifndef gBoardUseFro32k_d
#define gBoardUseFro32k_d 0
#endif

/*! @brief The configuration of timer. */
#ifndef PLATFORM_TM_INSTANCE
#define PLATFORM_TM_INSTANCE 0U
#endif
#ifndef PLATFORM_TM_CLK_FREQ
#define PLATFORM_TM_CLK_FREQ 32768U
#endif

#ifndef PLATFORM_TM_CLK_SELECT
#define PLATFORM_TM_CLK_SELECT 2U /*Lptmr timer use (kLPTMR_PrescalerClock_2) 32k clock*/
#endif

/*! @brief The configuration of timer stamp. */
#ifndef PLATFORM_TM_STAMP_INSTANCE
#define PLATFORM_TM_STAMP_INSTANCE 1U
#endif
#ifndef PLATFORM_TM_STAMP_CLK_FREQ
#define PLATFORM_TM_STAMP_CLK_FREQ 32768U
#endif

#ifndef PLATFORM_TM_STAMP_CLK_SELECT
#define PLATFORM_TM_STAMP_CLK_SELECT 2U /*Lptmr timer use (kLPTMR_PrescalerClock_2) 32k clock*/
#endif

#define PLATFORM_TM_STAMP_MAX_US COUNT_TO_USEC(LPTMR_CNR_COUNTER_MASK, PLATFORM_TM_STAMP_CLK_FREQ)

/*!
 * \brief  type definition for the list of reset status
 *
 */
typedef enum
{
    PLATFORM_LowPowerWakeup,
    PLATFORM_DeviceReset,
} PLATFORM_ResetStatus_t;

/* -------------------------------------------------------------------------- */
/*                         Public memory declarations                         */
/* -------------------------------------------------------------------------- */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/* -------------------------------------------------------------------------- */
/*                        Public functions declaration                        */
/* -------------------------------------------------------------------------- */

/*!
 * \brief  Initialize 32K oscillator but doesn't select it as clock source
 *         To switch to 32K crystal, you need to call PLATFORM_SwitchToOsc32k()
 *         This allows to perform other initialization before the 32k clock is
 *         actually needed. Should be called as early as possible.
 *
 * \return int 0 if success, negative value if error.
 */
int PLATFORM_InitOsc32K(void);

/*!
 * \brief Waits for the osc32k to be ready and then selects it as 32k clock
 *        source. It is mandatory to call PLATFORM_InitOsc32K before,
 *        otherwise this function never returns.
 *
 * \return int 0 if success, 1 if already initialized, negative value if error.
 */
int PLATFORM_SwitchToOsc32k(void);

/*!
 * \brief  Uninitialize 32K oscillator
 *
 */
void PLATFORM_UninitOsc32K(void);

/*!
 * \brief  Initialize NBU
 *
 * \return int 0 if success, 1 if already initialized, negative value if error.
 */
int PLATFORM_InitNbu(void);

/*!
 * \brief  Initialize of the multicore
 *
 * \return int 0 if success, negative if error.
 */
int PLATFORM_InitMulticore(void);

/*!
 * \brief  get 4 words of information that uniquely identifies the MCU
 *
 * \param[out] aOutUid16B pointer to UID bytes
 * \param[out] pOutLen pointer to UID length
 */
void PLATFORM_GetMCUUid(uint8_t *aOutUid16B, uint8_t *pOutLen);

/*!
 * \brief get the device MAC ADDRESS of 40 bits
 *
 * \param[out] aOutMacAddr5B pointer to MAC ADDRESS
 */
void PLATFORM_GetMACAddr(uint8_t *aOutMacAddr5B);

/*!
 * \brief get the XTAL trim value
 *
 * \param[in] regRead boolean to read value from Radio register or from HW parameters
 *
 * \return XTAL trim value
 */
uint8_t PLATFORM_GetXtal32MhzTrim(bool_t regRead);

/*!
 * \brief Set the XTAL trim value
 *        Calling this function assumes HWParameters in flash have been read
 *
 * \param[in] trimValue Trim value to be set
 * \param[in] saveToHwParams boolean to update value in HW parameters
 */
void PLATFORM_SetXtal32MhzTrim(uint8_t trimValue, bool_t saveToHwParams);

/*!
 * \brief Load the HW parameters from Flash to RAM
 *
 */
void PLATFORM_LoadHwParams(void);

/*!
 * \brief  Initialize Timer Manager
 *
 *    This API will initialize the Timer Manager and the required clocks
 *
 * \return int 0 if success, 1 if already initialized, negative value if error.
 */
int PLATFORM_InitTimerManager(void);

/*!
 * \brief  Deinitialize Timer Manager
 *
 *    This API will deinitialize the Timer Manager
 *
 */
void PLATFORM_DeinitTimerManager(void);

/*!
 * \brief  Initialize Security subsystem
 *
 */
void PLATFORM_InitCrypto(void);

/*!
 * \brief Disable Controller low power entry
 *        Depending on the platform, this can concern multiple controllers
 *        Controller low power is always enabled by default, so this should be
 *        called mainly for debug purpose
 *
 */
void PLATFORM_DisableControllerLowPower(void);

/*!
 * \brief  Request Radio domain to be active
 *
 *  On return from this function, the Radio domain and all its HW ressources can be accessed safely
 *    until PLATFORM_RemoteActiveRel() is called
 *
 */
void PLATFORM_RemoteActiveReq(void);

/*!
 * \brief  Release Radio domain from being active
 *
 *  On return from this function, the Radio domain and all its HW ressources can not be accessed
 *    if the readio domain has turned into lowpower,
 *   Need to call PLATFORM_RemoteActiveReq() for accessing safely to the ressources it contains
 *
 */
void PLATFORM_RemoteActiveRel(void);

/*!
 * \brief Get the reset cause, the reason why the chip is awake
 *
 * \param[out] reset_status Reason of the last reset
 *
 */
void PLATFORM_GetResetCause(PLATFORM_ResetStatus_t *reset_status);

/*!
 * \brief  Get Timestamp in us
 *
 * \param[out] now Current timestamp
 *
 */
void PLATFORM_GetTimestamp(uint64_t *now);

/*!
 * \brief  wait for the given delay in us
 *
 * \param[in] delay_us time delay in us
 *
 */
void PLATFORM_Delay(uint64_t delay_us);

/*!
 * \brief  wait for the given delay in us starting from
 *  given Timestamp. Timestamp shall be get from PLATFORM_GetTimestamp()
 *
 * \param[in] timestamp in us
 * \param[in] delay_us time delay in us
 *
 */
void PLATFORM_WaitTimeout(uint64_t timestamp, uint64_t delay_us);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*!
 * @}  end of FWK_Platform addtogroup
 */
/*!
 * @}  end of FWK_Platform_module addtogroup
 */
#endif /* _FWK_PLATFORM_H_ */
