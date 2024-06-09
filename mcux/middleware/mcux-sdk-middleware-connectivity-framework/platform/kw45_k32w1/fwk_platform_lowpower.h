/* -------------------------------------------------------------------------- */
/*                           Copyright 2021-2022 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

#ifndef _FWK_PLATFORM_LOWPOWER_H_
#define _FWK_PLATFORM_LOWPOWER_H_

/*!
 * @addtogroup FWK_Platform_module
 * @{
 */
/*!
 * @addtogroup FWK_Platform_LowPower
 * The FWK_Platform_LowPower module
 *
 * FWK_Platform_LowPower module provides APIs to handle Low Power .
 * @{
 */
/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "EmbeddedTypes.h"
#include "fsl_pm_board.h"

/* -------------------------------------------------------------------------- */
/*                                Public macros                               */
/* -------------------------------------------------------------------------- */

/*! Enable/Disable shutdown of ECC RAM banks during low power period like Deep Sleep or Power Down
 *  Shutting down ECC RAM banks allows to save about 1uA
 *  The RAM banks can be selectively reinitialized by calling MEM_ReinitRamBank API
 *  The MemoryManagerLight will call this API when allocating a new block in the heap
 *  Defining this flag to 0 will make the system shutdown only the non-ecc banks (STCM3 and STCM4) */
#ifndef gPlatformShutdownEccRamInLowPower
#define gPlatformShutdownEccRamInLowPower 1
#endif

#define PLATFORM_WFI_STATE             PM_LP_STATE_SLEEP
#define PLATFORM_SLEEP_STATE           PM_LP_STATE_SLEEP
#define PLATFORM_DEEP_SLEEP_STATE      PM_LP_STATE_DEEP_SLEEP
#define PLATFORM_POWER_DOWN_STATE      PM_LP_STATE_POWER_DOWN
#define PLATFORM_DEEP_POWER_DOWN_STATE PM_LP_STATE_DEEP_POWER_DOWN

/* In order to set WFI constraint, we need to set the SLEEP constraint with the Bus clock ON
 * This will has the effect to keep the main domain and wakeup domain in active    */
#define PLATFORM_WFI_CONSTRAINTS 1, PM_RESC_BUS_SYS_CLK_ON

#define PLATFORM_SLEEP_CONSTRAINTS 0

#if defined(KW45_A0_SUPPORT) && (KW45_A0_SUPPORT == 1)
// A0 version specific code
#define PLATFORM_DEEP_SLEEP_CONSTRAINTS                                                                    \
    9, PM_RESC_CTCM0_DEEPSLEEP, PM_RESC_CTCM1_DEEPSLEEP, PM_RESC_STCM0_DEEPSLEEP, PM_RESC_STCM1_DEEPSLEEP, \
        PM_RESC_STCM2_DEEPSLEEP, PM_RESC_STCM3_DEEPSLEEP, PM_RESC_STCM4_DEEPSLEEP,                         \
        PM_RESC_MAIN_PD_PERI_STATE_RETENTION,                                                              \
        PM_RESC_WAKE_PD_PERI_OPERATIONAL // wake up power domain can not go in retention with XTAL32K on A0: no timer
                                         // Wakeup
#else
#define PLATFORM_DEEP_SLEEP_CONSTRAINTS 0
#endif

#define PLATFORM_POWER_DOWN_CONSTRAINTS 0

#define PLATFORM_DEEP_POWER_DOWN_CONSTRAINTS 0

/* -------------------------------------------------------------------------- */
/*                              Public type definition                        */
/* -------------------------------------------------------------------------- */

/*!
 * \brief  type definition for the list of power domains in the device
 *
 */
typedef enum
{
    PLATFORM_MainDomain,   /*!<  Main Power domain containing the main processor        */
    PLATFORM_WakeupDomain, /*!<  Wake up Power domain containing the wake up peripheral */
} PLATFORM_PowerDomain_t;

/*!
 * \brief  type definition for the list of low power mode that a power domain can go inti
 *
 */
typedef enum
{
    PLATFORM_NO_LOWPOWER,          /*!<   Power domain has not been in Low power mode       */
    PLATFORM_SLEEP_MODE,           /*!<   Power domain has not been in Sleep mode           */
    PLATFORM_DEEP_SLEEP_MODE,      /*!<   Power domain has not been in Deep Sleep mode      */
    PLATFORM_POWER_DOWN_MODE,      /*!<   Power domain has not been in Power Down mode      */
    PLATFORM_DEEP_POWER_DOWN_MODE, /*!<   Power domain has not been in Deep Power Down mode */
} PLATFORM_PowerDomainState_t;

/*!
 * \brief  returned status code
 *
 */
typedef enum
{
    PLATFORM_Successful, /*!<  Function reports success        */
    PLATFORM_Error,      /*!<  Function reports Error          */
} PLATFORM_status_t;
/* -------------------------------------------------------------------------- */
/*                              Public prototypes                             */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Initialize platform specific ressources for low power support
 *
 */
void PLATFORM_LowPowerInit(void);

/*!
 * \brief Start a wake up timer for next tickless idle period
 *        Should be called during low power entry procedure.
 *
 * \param[in] timeoutUs maximum timeout in microsec
 */
void PLATFORM_StartWakeUpTimer(uint64_t timeoutUs);

/*!
 * \brief Stops previously started wake up timer if the tickless period ended
 *         earlier than expected. Should be called after exiting low power.
 *
 */
void PLATFORM_StopWakeUpTimer(void);

/*!
 * \brief Returns current timestamp in us, usually called before and after tickless
 *        period to compute the number of ticks to update RTOS timebase.
 *
 * \return uint64_t Timestamp value in us
 */
uint64_t PLATFORM_GetLowPowerTimestampUs(void);

/*!
 * \brief Converts timestamp values returned by PLATFORM_GetLowPowerTimestamp to
 *        a duration in us and handles counter wrapping.
 *
 * \param[in] enterLowPowerTimestamp Timestamp measured before entering tickless period
 * \param[in] exitLowPowerTimestamp Timestamp measured after exiting tickless period
 * \return uint64_t Computed duration in us
 */
uint64_t PLATFORM_GetLowPowerDurationUs(uint64_t enterLowPowerTimestamp, uint64_t exitLowPowerTimestamp);

/*!
 * \brief Platform module can implement platform specific methods to execute
 *        when entering and exiting any low power mode. \n
 *        Those methods should implement only mandatory procedures for the
 *        platform, compatible with any connectivity protocol.
 */
void PLATFORM_EnterLowPower(void);

/*!
 * \brief Platform specific procedures to execute when exiting any low power mode
 *
 */
void PLATFORM_ExitLowPower(void);

/*!
 * \brief Power gated low power modes often require specific
 *        entry/exit low power procedures, those should be implemented
 *        in the following API.
 *
 */
void PLATFORM_EnterPowerDown(void);

/*!
 * \brief Power gated low power modes often require specific
 *        entry/exit low power procedures, those should be implemented
 *        in the following API.
 *
 */
void PLATFORM_ExitPowerDown(void);

/*!
 * \brief Specific low power entry procedure when going to Deep Power Down mode (RAMOFF)
 *
 */
void PLATFORM_EnterDeepPowerDown(void);

/*!
 * \brief Get the low power state of a power domain in the previous lowpower period
 * \note Shall be called from the lowpower exit callbacks, state returned after the wakeup
 *          process may no longer be accurate
 *
 * \param[in]  power_domain which we want to obtain the lowpower state
 * \param[out] pd_state Lowpower state
 * \return     PLATFORM_status_t error status
 */
PLATFORM_status_t PLATFORM_GetLowpowerMode(PLATFORM_PowerDomain_t power_domain, PLATFORM_PowerDomainState_t *pd_state);

/*!
 * \brief Check which banks need to be retained
 * \note This function is linker script specific.\n
 *       Amelioration handle by this function :\n
 *       - There is a free block at the end of the heap, so if the heap is on
 *       the top of the RAM. No need to retain all banks upper that the last one
 *       used by the heap.
 *
 * \return uint8_t mask of which bank needs to be retained
 */
uint8_t PLATFORM_GetDefaultRamBanksRetained(void);

/*!
 * \brief Set the banks that need to be retained in lowpower
 *
 * \param[in] bank_mask mask of which bank needs to be retained
 */
void PLATFORM_SetRamBanksRetained(uint8_t bank_mask);

/*!
 * @}  end of FWK_Platform_LowPower addtogroup
 */
/*!
 * @}  end of FWK_Platform_module addtogroup
 */

#endif /* _FWK_PLATFORM_LOWPOWER_H_ */
