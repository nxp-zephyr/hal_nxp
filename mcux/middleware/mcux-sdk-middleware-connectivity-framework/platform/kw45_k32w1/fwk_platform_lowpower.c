/* -------------------------------------------------------------------------- */
/*                           Copyright 2021-2023 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <stdbool.h>

#include "fwk_platform.h"
#include "fwk_platform_lowpower.h"
#include "fwk_debug.h"

/* SDK drivers */
#include "fsl_spc.h"
#include "fsl_wuu.h"
#include "fsl_cmc.h"

/* SDK components */
#include "fsl_pm_core.h"
#include "fsl_component_mem_manager.h"
#if !defined(CONFIG_PM)
#include "fsl_component_timer_manager.h"
/* Security */
#include "SecLib.h"
#endif
/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

#define PWR_CTCM0_START_ADDR (0x04000000U)
#define PWR_CTCM0_END_ADDR   (0x04001FFFU)
#define PWR_CTCM1_START_ADDR (0x04002000U)
#define PWR_CTCM1_END_ADDR   (0x04003FFFU)
#define PWR_STCM0_START_ADDR (0x20000000U)
#define PWR_STCM0_END_ADDR   (0x20003FFFU)
#define PWR_STCM1_START_ADDR (0x20004000U)
#define PWR_STCM1_END_ADDR   (0x20007FFFU)
#define PWR_STCM2_START_ADDR (0x20008000U)
#define PWR_STCM2_END_ADDR   (0x2000FFFFU)
#define PWR_STCM3_START_ADDR (0x20010000U)
#define PWR_STCM3_END_ADDR   (0x20017FFFU)
#define PWR_STCM4_START_ADDR (0x20018000U)
#define PWR_STCM4_END_ADDR   (0x20019FFFU)
#define PWR_STCM5_START_ADDR (0x2001A000U)
#define PWR_STCM5_END_ADDR   (0x2001BFFFU)

#define PWR_CTCM0_IDX 0U
#define PWR_CTCM1_IDX 1U
#define PWR_STCM0_IDX 2U
#define PWR_STCM1_IDX 3U
#define PWR_STCM2_IDX 4U
#define PWR_STCM3_IDX 5U
#define PWR_STCM4_IDX 6U
#define PWR_STCM5_IDX 7U

#if defined(gPlatformShutdownEccRamInLowPower) && (gPlatformShutdownEccRamInLowPower > 0)
/* In this configuration, all RAM banks can be shutdown during low power if not used
 * The ECC RAM banks can be selectively reinitialized with MEM_ReinitRamBank API
 * This API is also used by the Memory Manager Light */
#define PWR_SELECT_RAM_RET_START_IDX 0U
#define PWR_SELECT_RAM_RET_END_IDX   7U
#else
/* STCM3 and STCM4 only are non-ECC RAM banks */
#define PWR_SELECT_RAM_RET_START_IDX 5U
#define PWR_SELECT_RAM_RET_END_IDX   6U
#endif /* gPlatformShutdownEccRamInLowPower */

#define SPC_VD_STAT_CLEAR_MASK                                                                    \
    (SPC_VD_STAT_COREVDD_LVDF_MASK | SPC_VD_STAT_SYSVDD_LVDF_MASK | SPC_VD_STAT_IOVDD_LVDF_MASK | \
     SPC_VD_STAT_COREVDD_HVDF_MASK | SPC_VD_STAT_SYSVDD_HVDF_MASK | SPC_VD_STAT_IOVDD_HVDF_MASK)

#define PLATFORM_LPTIMESTAMP_MAX_VALUE ((uint64_t)0xFFFFFFFFFFFFFFFFU)
#define PLATFORM_LPWKUP_DELAY_10MHz    (0xAAU)

/* -------------------------------------------------------------------------- */
/*                             Private variables                              */
/* -------------------------------------------------------------------------- */

static const uint32_t TCM_START_ADDR_tab[] = {
    PWR_CTCM0_START_ADDR, PWR_CTCM1_START_ADDR, PWR_STCM0_START_ADDR, PWR_STCM1_START_ADDR,
    PWR_STCM2_START_ADDR, PWR_STCM3_START_ADDR, PWR_STCM4_START_ADDR, PWR_STCM5_START_ADDR,
};

static const uint32_t TCM_END_ADDR_tab[] = {
    PWR_CTCM0_END_ADDR, PWR_CTCM1_END_ADDR, PWR_STCM0_END_ADDR, PWR_STCM1_END_ADDR,
    PWR_STCM2_END_ADDR, PWR_STCM3_END_ADDR, PWR_STCM4_END_ADDR, PWR_STCM5_END_ADDR,
};

#if defined(gPlatformShutdownEccRamInLowPower) && (gPlatformShutdownEccRamInLowPower > 0)
static const bool TCM_IS_ECC[] = {
    TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE,
};

static uint32_t last_bank_mask = 0xFFU;
#endif /* gPlatformShutdownEccRamInLowPower */

static const spc_power_domain_id_t pd_name_2_id[2U] = {
    [PLATFORM_MainDomain]   = kSPC_PowerDomain0, /*!<  Main Power domain containing the main processor        */
    [PLATFORM_WakeupDomain] = kSPC_PowerDomain1, /*!<  Wake up Power domain containing the wake up peripheral */
};

static const PLATFORM_PowerDomainState_t spc_lp_name_2_pd_state[9U] = {
    [kSPC_SleepWithSYSClockRunning] = PLATFORM_NO_LOWPOWER, /*!<   Power domain has not been in Low power mode       */
    [kSPC_SleepWithSysClockOff]     = PLATFORM_SLEEP_MODE,  /*!<   Power domain has not been in Sleep mode           */
    [kSPC_DeepSleepSysClockOff] = PLATFORM_DEEP_SLEEP_MODE, /*!<   Power domain has not been in Deep Sleep mode      */
    [kSPC_PowerDownWithSysClockOff] = PLATFORM_POWER_DOWN_MODE, /*!<   Power domain has not been in Power Down mode */
    [kSPC_DeepPowerDownWithSysClockOff] =
        PLATFORM_DEEP_POWER_DOWN_MODE, /*!<   Power domain has not been in Deep Power Down mode */
};
#if !defined(CONFIG_PM)
static TIMER_MANAGER_HANDLE_DEFINE(wakeUpTimerId);
#endif
/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Initializes wake up sources
 *
 */
static void PLATFORM_InitWakeUpSources(void);

/*!
 * \brief Shutdown Radio domain
 *        To be used when the current application is ready to go to RAMOFF
 *
 */
static void PLATFORM_ShutdownRadio(void);

void WUU0_IRQHandler(void);

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */
void PLATFORM_LowPowerInit(void)
{
#if !defined(CONFIG_PM)
    timer_status_t timer_status;
#endif
    /* Initialize required wake up sources */
    PLATFORM_InitWakeUpSources();

    /* Clear voltage detect status */
    SPC_ClearVoltageDetectStatusFlag(SPC0, SPC_VD_STAT_CLEAR_MASK);

    /* Set number of SPC timer clock cycles to wait on exit from lowpower mode */
    SPC_SetLowPowerWakeUpDelay(SPC0, PLATFORM_LPWKUP_DELAY_10MHz);
#if !defined(CONFIG_PM)
    timer_status = TM_Open((timer_handle_t)wakeUpTimerId);
    assert(timer_status == kStatus_TimerSuccess);
    (void)timer_status;
#endif
}
#if !defined(CONFIG_PM)
void PLATFORM_StartWakeUpTimer(uint64_t timeoutUs)
{
    TM_EnterTickless((timer_handle_t)wakeUpTimerId, timeoutUs);
}

void PLATFORM_StopWakeUpTimer(void)
{
    TM_ExitTickless((timer_handle_t)wakeUpTimerId);
}

uint64_t PLATFORM_GetLowPowerTimestampUs(void)
{
    uint64_t timestamp;

#if defined(TM_ENABLE_TIME_STAMP) && (TM_ENABLE_TIME_STAMP == 1)
    timestamp = TM_GetTimestamp();
#else
    timestamp = 0U;
#endif

    return timestamp;
}

uint64_t PLATFORM_GetLowPowerDurationUs(uint64_t enterLowPowerTimestamp, uint64_t exitLowPowerTimestamp)
{
    uint64_t lowPowerDurationUs;

    if (exitLowPowerTimestamp < enterLowPowerTimestamp)
    {
        /* Handle counter wrapping */
        lowPowerDurationUs = PLATFORM_LPTIMESTAMP_MAX_VALUE - enterLowPowerTimestamp + exitLowPowerTimestamp;
    }
    else
    {
        lowPowerDurationUs = exitLowPowerTimestamp - enterLowPowerTimestamp;
    }

    return lowPowerDurationUs;
}
#endif
void PLATFORM_EnterLowPower(void)
{
#if defined(KW45_A0_SUPPORT) && (KW45_A0_SUPPORT > 0)
    /* Disable ACTIVE_CFG LVD to avoid reset while going to low power
     * when Core LDO voltage is higher than 1.0V (KFOURWONE-1703) */
    (void)SPC_EnableActiveModeCoreLowVoltageDetect(SPC0, false);
#endif
    return;
}

void PLATFORM_ExitLowPower(void)
{
#if defined(KW45_A0_SUPPORT) && (KW45_A0_SUPPORT > 0)
    /* Re-enable ACTIVE_CFG LVD when exiting low power (KFOURWONE-1703) */
    (void)SPC_EnableActiveModeCoreLowVoltageDetect(SPC0, true);
#endif
    return;
}

void PLATFORM_EnterPowerDown(void)
{
    /* Warning : For power down, it requires gLowpowerPowerDownEnable_d variable to be set to 1 in linker file.
     *    Generate assertion if not the case */
    extern uint32_t gLowpowerPowerDownEnable_d[];
    assert((uint32_t)&gLowpowerPowerDownEnable_d == 1);
    (void)gLowpowerPowerDownEnable_d;
#if !defined(CONFIG_PM)
    /* Allow the the cryptographic HW acceleration to reinitialize next time we will need it*/
    SecLib_DeInit();
#endif
}

void PLATFORM_ExitPowerDown(void)
{
    return; /* Exit power down procedures can be called here */
}

void PLATFORM_EnterDeepPowerDown(void)
{
    /* Put NBU domain in Deep Power Down */
    PLATFORM_ShutdownRadio();
}

void WUU0_IRQHandler(void)
{
    status_t status;

    // PWR_DBG_LOG("");

    /* The Power Manager will parse all the registered and enabled wake up sources
     * and determine if they triggered the WUU IRQ. If so, it will call each
     * wake up source's callback */
    status = PM_HandleWakeUpEvent();
    assert(status == kStatus_PMSuccess);
    (void)status;

    /* Clear WUU pin status flag */
    WUU_ClearExternalWakeUpPinsFlag(WUU0, WUU_GetExternalWakeUpPinsFlag(WUU0));
}

PLATFORM_status_t PLATFORM_GetLowpowerMode(PLATFORM_PowerDomain_t power_domain, PLATFORM_PowerDomainState_t *pd_state)
{
    spc_power_domain_id_t pd_id  = pd_name_2_id[power_domain];
    PLATFORM_status_t     status = PLATFORM_Successful;

    if (SPC_CheckPowerDomainLowPowerRequest(SPC0, pd_id) == true)
    {
        spc_power_domain_low_power_mode_t spcLowPowermode;
        spcLowPowermode = SPC_GetPowerDomainLowPowerMode(SPC0, pd_id);

        if ((uint32_t)spcLowPowermode >= sizeof(spc_lp_name_2_pd_state))
        {
            status = PLATFORM_Error;
        }
        else
        {
            *pd_state = spc_lp_name_2_pd_state[spcLowPowermode];
        }
    }
    else
    {
        *pd_state = PLATFORM_NO_LOWPOWER;
    }

    return status;
}

uint8_t PLATFORM_GetDefaultRamBanksRetained(void)
{
    uint32_t          ram_upper_limit;
    uint32_t          ram_lower_limit;
    uint8_t           bank_mask = 0xFFU; // Retain everything by default
    volatile uint32_t heap_end;

    /* Those symbols should be exported by the Linker Script */
    extern uint32_t firmware_ram_upper_limit[];
    extern uint32_t firmware_ram_lower_limit[];
    extern uint32_t __HEAP_end__[];

    /* Some compilers can try to optimize the comparison between two external addresses because they will think that
     * they come from two global values and therefore that it is impossible for them to have the same address, but in
     * our case these values come from our linker. That's why we used a local volatile value to make sure it won't be
     * optimized */
    heap_end = (uint32_t)&__HEAP_end__;

    /* Lower ram limit is not necessarily in the first RAM bank
     * On KW45, the 2 first banks are Code TCM banks, but usually not used */
    ram_lower_limit = (uint32_t)&firmware_ram_lower_limit;

    /* Optimized linker script and memory manager, the heap is placed on the top of the RAM */
    if ((uint32_t)&firmware_ram_upper_limit == heap_end)
    {
        ram_upper_limit = MEM_GetHeapUpperLimit();
    }
    else
    {
        ram_upper_limit = (uint32_t)&firmware_ram_upper_limit;
    }

    assert(ram_upper_limit > ram_lower_limit);

    /* Go through each selectables RAM banks and check if we can shutdown some
     * of them */
    for (uint8_t i = PWR_SELECT_RAM_RET_START_IDX; i <= PWR_SELECT_RAM_RET_END_IDX; i++)
    {
        if ((TCM_START_ADDR_tab[i] >= ram_upper_limit) || (TCM_END_ADDR_tab[i] <= ram_lower_limit))
        {
            /* This RAM bank is outside current used RAM range, so we can
             * shut it down during low power */
            bank_mask &= ~(1U << i);
        }
        else
        {
            continue;
        }
    }

    return bank_mask;
}

void PLATFORM_SetRamBanksRetained(uint8_t bank_mask)
{
    /* Set SRAM retention config handled by the CMC module */
    CMC_PowerOffSRAMLowPowerOnly(CMC0, ~bank_mask);

    /* STCM5 is handled by VBAT module */
    uint32_t vbat_ldoramc = VBAT0->LDORAMC;
    vbat_ldoramc &= ~VBAT_LDORAMC_RET_MASK;
    vbat_ldoramc |= VBAT_LDORAMC_RET((uint32_t)(~((uint32_t)bank_mask & ((uint32_t)1U << PWR_STCM5_IDX)) >> 7));
    VBAT0->LDORAMC = vbat_ldoramc;

#if defined(gPlatformShutdownEccRamInLowPower) && (gPlatformShutdownEccRamInLowPower > 0)
    last_bank_mask = bank_mask;
#endif
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Initializes wake up sources
 *
 */
static void PLATFORM_InitWakeUpSources(void)
{
    /* Enable low power timers (LPTMR) as wake up source */
    WUU_SetInternalWakeUpModulesConfig(WUU0, 0U, kWUU_InternalModuleInterrupt);

    /* Enable Radio Async Wakeup request
     * This is needed to correctly wake up SOC XBAR bus when CM3 needs it */
    WUU_SetInternalWakeUpModulesConfig(WUU0, 2U, kWUU_InternalModuleDMATrigger);

    /* WUU Irq interrupt is required for proper wakeup functionality from lowpower */
    NVIC_EnableIRQ(WUU0_IRQn);
}

static void PLATFORM_ShutdownRadio(void)
{
    uint32_t rfmc2p4ghzCtrl = RFMC->RF2P4GHZ_CTRL;

    /* Clear RF IRQ before shutting down radio */
    for (int IRQ_Type = ((int)RF_IMU0_IRQn); IRQ_Type <= ((int)RF_WOR_IRQn); IRQ_Type++)
    {
        __NVIC_DisableIRQ((IRQn_Type)IRQ_Type);
        __NVIC_ClearPendingIRQ((IRQn_Type)IRQ_Type);
    }

    /* Wait for radio to be in low power */
    while ((RFMC->RF2P4GHZ_STAT & RFMC_RF2P4GHZ_STAT_BLE_STATE_MASK) != RFMC_RF2P4GHZ_STAT_BLE_STATE(0x2U))
    {
    }

    /* Set the RFMC low power mode to Deep Power Down */
    rfmc2p4ghzCtrl &= RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK;
    rfmc2p4ghzCtrl |= (RFMC_RF2P4GHZ_CTRL_LP_MODE(0xFU) | RFMC_RF2P4GHZ_CTRL_LP_ENTER(0x1U));

    /* In case the NBU is already in low power, the new RFMC low power mode won't be applied
     * So we need to wake up the radio domain, apply the new low power mode and release
     * the wake up request so the radio can re-enter low power */
    PLATFORM_RemoteActiveReq();

    RFMC->RF2P4GHZ_CTRL = rfmc2p4ghzCtrl;

    PLATFORM_RemoteActiveRel();
}

#if defined(gPlatformShutdownEccRamInLowPower) && (gPlatformShutdownEccRamInLowPower > 0)
void MEM_ReinitRamBank(uint32_t startAddress, uint32_t endAddress)
{
    bool     indexFound = false;
    uint32_t startIndex = 0xFFU;
    uint32_t endIndex   = 0xFFU;

    /* This for loop will find the RAM banks which cover the requested address range.
     * The range could be large enough to contain more than one bank */
    for (uint32_t i = PWR_SELECT_RAM_RET_START_IDX; i <= PWR_SELECT_RAM_RET_END_IDX; i++)
    {
        if ((startAddress >= TCM_START_ADDR_tab[i]) && (startAddress <= TCM_END_ADDR_tab[i]))
        {
            /* The start address is contained in this bank */
            startIndex = i;
        }

        if ((endAddress >= TCM_START_ADDR_tab[i]) && (endAddress <= TCM_END_ADDR_tab[i]))
        {
            /* The end address is contained in this bank */
            endIndex = i;
        }

        if ((startIndex != 0xFFU) && (endIndex != 0xFFU))
        {
            /* Both indexes have be found, we can proceed further */
            indexFound = true;
            break;
        }
    }

    if (indexFound == true)
    {
        /* This loop will go through the different RAM banks based on the indexes found
         * right before. If the current bank is an ECC bank and if it has been shutdown
         * during last low power period, then we proceed with the memset to force ECC
         * calculation */
        for (uint32_t i = startIndex; i <= endIndex; i++)
        {
            if (TCM_IS_ECC[i] == true)
            {
                if ((last_bank_mask & ((uint32_t)1U << i)) == 0U)
                {
                    /* Write to ECC RAM bank to force ECC calculation */
                    (void)memset((uint32_t *)TCM_START_ADDR_tab[i], 0,
                                 TCM_END_ADDR_tab[i] - TCM_START_ADDR_tab[i] + 1U);

                    /* Remove this bank from the last shutdown during low power as
                     * it has been reinitialized */
                    last_bank_mask |= ((uint32_t)1U << i);
                }
            }
        }
    }
}
#endif /* gPlatformShutdownEccRamInLowPower */
