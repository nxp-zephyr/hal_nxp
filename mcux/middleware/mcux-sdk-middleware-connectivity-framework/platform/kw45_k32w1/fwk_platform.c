/*
 * Copyright 2020 - 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/

#include <stdarg.h>
#include "fwk_platform.h"
#include "fwk_platform_ics.h"
/* NOTE: board.h inclusion is commented to avoid porting board specific files into HAL */
//#include "board.h" // Get BOARD_32MHZ_XTAL_TRIM_DEFAULT value if defined in board.h
#include "FunctionLib.h"
#include "fsl_adapter_flash.h"
#include "HWParameter.h"
#include "fsl_component_timer_manager.h"
#include "fsl_os_abstraction.h"
#include "fsl_ltc.h"
#include "fsl_adapter_rpmsg.h"

#if defined(gMWS_Enabled_d) && (gMWS_Enabled_d > 0)
#include "fwk_platform_mws.h"
#endif

#if defined(HDI_MODE) && (HDI_MODE == 1)
#include "fsl_port.h"
#include "hdi.h"
#endif

#include "fwk_debug.h"

#include "mcmgr_imu_internal.h"

#if defined(HDI_MODE) && (HDI_MODE == 1)
extern void BOARD_InitHDI();
#endif
/************************************************************************************
*************************************************************************************
* Private type definitions and macros
*************************************************************************************
************************************************************************************/

/*! @brief Max timestamp counter value (56 bits) */
#define PLATFORM_TIMESTAMP_MAX_VALUE ((uint64_t)0xFFFFFFFFFFFFFFU)

/*! @brief Default trimming value for 32MHz crystal,
      can be overidden from board.h,
      user shall define this flag in board.h file to set an other default value  */
#if !defined(BOARD_32MHZ_XTAL_TRIM_DEFAULT)
#define BOARD_32MHZ_XTAL_TRIM_DEFAULT 12U
#endif

/*! @brief Default load capacitance config for 32KHz crystal,
      can be overidden from board.h,
      user shall define this flag in board.h file to set an other default value
      Values must be adjust to minimize the jitter on the crystal. This is to avoid
      a shift of 31.25us on the link layer timebase in NBU.
*/
#if !defined(BOARD_32KHZ_XTAL_CLOAD_DEFAULT)
#define BOARD_32KHZ_XTAL_CLOAD_DEFAULT (CCM32K_OSC32K_CTRL_XTAL_CAP_SEL(8U) | CCM32K_OSC32K_CTRL_EXTAL_CAP_SEL(8U))
#endif

#if !defined(FSL_FEATURE_TSTMR_CLOCK_FREQUENCY_1MHZ) || !FSL_FEATURE_TSTMR_CLOCK_FREQUENCY_1MHZ
#warning "Warning: TSTMR freq is not 1MHz !"
#endif

/*! @brief XTAL 32Mhz clock source start up timeout */
#ifndef FWK_PLATFORM_XTAL32M_STARTUP_TIMEOUT
#define FWK_PLATFORM_XTAL32M_STARTUP_TIMEOUT 1000000
#endif /* FWK_PLATFORM_XTAL32M_STARTUP_TIMEOUT */

/*! @brief XTAL 32Khz clock source start up timeout */
#ifndef FWK_PLATFORM_XTAL32K_STARTUP_TIMEOUT
#define FWK_PLATFORM_XTAL32K_STARTUP_TIMEOUT 10000000
#endif /* FWK_PLATFORM_XTAL32K_STARTUP_TIMEOUT */

/* Raise error with status update , shift previous status by 4 bits and OR with new error code.
 * the returned status will be negative */
#define RAISE_ERROR(__st, __error_code) -(int)((uint32_t)(((uint32_t)(__st) << 4) | (uint32_t)(__error_code)));

/************************************************************************************
 * Private memory declarations
 ************************************************************************************/
static uint8_t Xtal32MhzTrim = BOARD_32MHZ_XTAL_TRIM_DEFAULT;

extern const scg_sys_clk_config_t g_sysClkConfig_SOC_ClockLowPower;

const scg_sys_clk_config_t g_sysClkConfig_SOC_ClockLowPower = {
    .divSlow = (uint32_t)kSCG_SysClkDivBy3,  /* Slow Clock Divider: divided by 3 */
    .divBus  = (uint32_t)kSCG_SysClkDivBy1,  /* Bus Clock Divider: divided by 1 */
    .divCore = (uint32_t)kSCG_SysClkDivBy16, /* Core Clock Divider: divided by 16 */
    .src     = (uint32_t)kSCG_SysClkSrcFirc, /* Fast IRC is selected as System Clock Source */
};

#if defined(gUseIpcTransport_d) && (gUseIpcTransport_d == 1)
SERIAL_MANAGER_HANDLE_DEFINE(g_IpcSerialHandle);
#define SERIAL_MANAGER_IPC_RING_BUFFER_SIZE (128U)
static uint8_t s_ipcRingBuffer[SERIAL_MANAGER_IPC_RING_BUFFER_SIZE];

static const hal_rpmsg_config_t ipcRpmsgConfig = {
    .local_addr  = 60,
    .remote_addr = 50,
};

const static serial_manager_config_t s_ipcSerialManagerConfig = {
    .type           = kSerialPort_Rpmsg,
    .ringBuffer     = &s_ipcRingBuffer[0],
    .ringBufferSize = SERIAL_MANAGER_IPC_RING_BUFFER_SIZE,
    .portConfig     = (hal_rpmsg_config_t *)&ipcRpmsgConfig,
};
#endif /* gUseIpcTransport_d */

static volatile int timer_manager_initialized = 0;

/****************** LOWPOWER ***********************/
/* Number of request for CM3 to remain active */
static int8_t active_request_nb = 0;

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

#if defined(HDI_MODE) && (HDI_MODE == 1)
void BOARD_InitPinsRFUART(void)
{
    /* RFUART_SIN */
    PORT_SetPinMux(PORTD, 1, kPORT_MuxAlt8);
    /* RFUART_SOUT */
    PORT_SetPinMux(PORTD, 2, kPORT_MuxAlt8);
}

void BOARD_InitPinsSPI0(void)
{
    /* Warning: current SPI0 pinmux can interfere with LPUART0 */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* SPI0_PCS0 */
    PORT_SetPinMux(PORTA, 16, kPORT_MuxAlt2);
    /* SPI0 SIN */
    PORT_SetPinMux(PORTA, 17, kPORT_MuxAlt2);
    /* SPI0 SOUT */
    PORT_SetPinMux(PORTA, 18, kPORT_MuxAlt2);
    /* SPI0 SCK */
    PORT_SetPinMux(PORTA, 19, kPORT_MuxAlt2);
}

static void BOARD_InitPinsRF(void)
{
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortC);

    /* ipp_do_rf_fpga [0..5] */
    // PORT_SetPinMux(PORTA, 16U, kPORT_MuxAlt8); // pinmux bypassed
    // PORT_SetPinMux(PORTA, 18U, kPORT_MuxAlt8);
    // PORT_SetPinMux(PORTA, 19U, kPORT_MuxAlt8);
    // PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt9);
    // PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt9);
    // PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt9);

    /* DTEST [0..12] */
    PORT_SetPinMux(PORTC, 0U, kPORT_MuxAlt8);
    PORT_SetPinMux(PORTC, 1U, kPORT_MuxAlt8);
    PORT_SetPinMux(PORTC, 2U, kPORT_MuxAlt8);
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt8);
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt8);
    PORT_SetPinMux(PORTC, 5U, kPORT_MuxAlt8);
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt8);
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt8);
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt8);
    PORT_SetPinMux(PORTB, 2U, kPORT_MuxAlt8);
    PORT_SetPinMux(PORTB, 3U, kPORT_MuxAlt8);
    PORT_SetPinMux(PORTB, 4U, kPORT_MuxAlt8);
    PORT_SetPinMux(PORTB, 5U, kPORT_MuxAlt8);

    /* PTD3 is used for Phy switch */
    // PORT_SetPinMux(PORTD, 3U, kPORT_MuxAsGpio);
}
#endif
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

int PLATFORM_InitOsc32K(void)
{
    uint32_t osc32k_ctrl;

    /* Clear OSC32K configuration and disable crystal */
    osc32k_ctrl = CCM32K->OSC32K_CTRL;
    osc32k_ctrl &=
        ~(CCM32K_OSC32K_CTRL_OSC_EN_MASK | CCM32K_OSC32K_CTRL_SOX_EN_MASK | CCM32K_OSC32K_CTRL_CAP_SEL_EN_MASK |
          CCM32K_OSC32K_CTRL_XTAL_CAP_SEL_MASK | CCM32K_OSC32K_CTRL_EXTAL_CAP_SEL_MASK);
    CCM32K->OSC32K_CTRL = osc32k_ctrl;

    /* Set Cload and trimming config to achieve 32768 Hz
     * Tests showed about 20 - 70 ppm with this config on EVK boards
     * This MUST be done BEFORE enabling the crystal */
    osc32k_ctrl |= BOARD_32KHZ_XTAL_CLOAD_DEFAULT;
    CCM32K->OSC32K_CTRL = osc32k_ctrl;

    /* Enable OSC32K in High Power mode
     * TODO: determine the impact on low power performance, could it be optimized and stay reliable ? */
    osc32k_ctrl |=
        CCM32K_OSC32K_CTRL_CAP_SEL_EN(0x1U) | CCM32K_OSC32K_CTRL_OSC_EN(0x1U) | CCM32K_OSC32K_CTRL_SOX_EN(0x1U);
    CCM32K->OSC32K_CTRL = osc32k_ctrl;

    /* Set amplifier gain adjustement to select external crystal ESR range */
    osc32k_ctrl &= ~(CCM32K_OSC32K_CTRL_COARSE_AMP_GAIN_MASK);
    osc32k_ctrl |= CCM32K_OSC32K_CTRL_COARSE_AMP_GAIN(0x3U);
    CCM32K->OSC32K_CTRL = osc32k_ctrl;

    return 0;
}

int PLATFORM_SwitchToOsc32k(void)
{
    int status = 0;
    int cnt    = 0;

    assert((CCM32K->OSC32K_CTRL & CCM32K_OSC32K_CTRL_OSC_EN_MASK) != 0);

    do
    {
        /* wait for the 32k OSC to be ready */
        while ((cnt++ < FWK_PLATFORM_XTAL32K_STARTUP_TIMEOUT) &&
               ((CCM32K->STATUS & CCM32K_STATUS_OSC32K_RDY_MASK) == 0U))
        {
        }
        if (cnt > FWK_PLATFORM_XTAL32K_STARTUP_TIMEOUT)
        {
            status = RAISE_ERROR(status, 1);
            break;
        }

        /* Select OSC32K as 32k clock source */
        CCM32K->CGC32K &= ~CCM32K_CGC32K_CLK_SEL_32K_MASK;
        CCM32K->CGC32K |= CCM32K_CGC32K_CLK_SEL_32K(1U);

        /* wait for the 32k OSC to be active */
        while ((cnt++ < FWK_PLATFORM_XTAL32K_STARTUP_TIMEOUT) &&
               ((CCM32K->STATUS & CCM32K_STATUS_OSC32K_ACTIVE_MASK) == 0U))
        {
        }
        if (cnt > FWK_PLATFORM_XTAL32K_STARTUP_TIMEOUT)
        {
            status = RAISE_ERROR(status, 2);
            break;
        }

        /* Disable FRO32K (to save some power) */
        CCM32K->FRO32K_CTRL &= ~(CCM32K_FRO32K_CTRL_FRO_EN_MASK);

#if 0
        /* Debug only : output 32k clock on ptc7 for frequency measurement
         * (need to include fsl_port.h) */
        CLOCK_EnableClock(kCLOCK_PortC);
        PORT_SetPinMux(PORTC, 7u, kPORT_MuxAlt7);
        SCG0->CLKOUTCNFG = SCG_CLKOUTCNFG_CLKOUTSEL(0x4);
#endif
    } while (false);

    return status;
}

void PLATFORM_UninitOsc32K(void)
{
    if ((CCM32K->OSC32K_CTRL & CCM32K_OSC32K_CTRL_OSC_EN_MASK) != 0U)
    {
        /* Disable 32k OSC */
        CCM32K->OSC32K_CTRL &= ~(CCM32K_OSC32K_CTRL_OSC_EN_MASK);
    }
}

int PLATFORM_InitNbu(void)
{
    int        status   = 0;
    static int nbu_init = 0;

    if (nbu_init == 0)
    {
        uint32_t rfmc_ctrl;
        int      cnt = 0;

        rfmc_ctrl = RFMC->RF2P4GHZ_CTRL;

        /* Enables BLE Power Controller on NBU side AND sets LP mode to DeepSleep */
        rfmc_ctrl &= ~(RFMC_RF2P4GHZ_CTRL_LP_WKUP_DLY_MASK | RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK);
        rfmc_ctrl |= (RFMC_RF2P4GHZ_CTRL_LP_MODE(0x3) | RFMC_RF2P4GHZ_CTRL_LP_WKUP_DLY(0x10U) |
                      RFMC_RF2P4GHZ_CTRL_BLE_LP_EN(0x1U));

        RFMC->RF2P4GHZ_CTRL = rfmc_ctrl;

        /* Enabling BLE Power Controller (BLE_LP_EN) will automatically start the XTAL
         * According to RM, we need to wait for the XTAL to be ready before accessing
         * Radio domain ressources.
         * Here, we make sure the XTAL is ready before releasing the CM3 from reset
         * it will prevent any access issue when the CM3 is starting up.
         * CM3 is released in HAL_RpmsgMcmgrInit */
        BOARD_DBGLPIOSET(2, 1);
        while ((cnt++ < FWK_PLATFORM_XTAL32M_STARTUP_TIMEOUT) && ((RFMC->XO_STAT & RFMC_XO_STAT_XTAL_RDY_MASK) == 0U))
        {
            BOARD_DBGLPIOSET(2, 0);
            __ASM("NOP");
            BOARD_DBGLPIOSET(2, 1);
        }
        BOARD_DBGLPIOSET(2, 0);

        if (cnt > FWK_PLATFORM_XTAL32M_STARTUP_TIMEOUT)
        {
            status = RAISE_ERROR(0, 1);
        }
        else
        {
#if defined(HDI_MODE) && (HDI_MODE == 1)
            /* configure pin mux */
            BOARD_InitPinsRF();
            /* Init SPI pins and interface to be used for channel switch */
            BOARD_InitHDI();
            /* Init HDI interface */
            HDI_Init();
#endif /* HDI_MODE */

            /* nbu initialization completed */
            nbu_init = 1;
        }
    }
    else
    {
        /* Initialization already done */
        status = 1;
    }

    return status;
}

int PLATFORM_InitMulticore(void)
{
    int                status = 0;
    hal_rpmsg_status_t rpmsg_status;

    /* Start CM3 core and Initializes the RPMSG adapter module for dual core communication */
    rpmsg_status = HAL_RpmsgMcmgrInit();
    if (rpmsg_status != kStatus_HAL_RpmsgSuccess)
    {
        status = RAISE_ERROR(rpmsg_status, 1);
        assert(0);
    }
    return status;
}

void PLATFORM_LoadHwParams(void)
{
    uint8_t               xtal_32m_trim = BOARD_32MHZ_XTAL_TRIM_DEFAULT;
    hardwareParameters_t *pHWParams     = NULL;
    uint32_t              status;

    /* Load the HW parameters from Flash to RAM */
    status = NV_ReadHWParameters(&pHWParams);
    if ((status == 0U) && (pHWParams->xtalTrim != 0xFFU))
    {
        /* There is an existing trim value */
        xtal_32m_trim = pHWParams->xtalTrim;
    }

    /* Send Xtal trim value to the NBU */
    PLATFORM_SetXtal32MhzTrim(xtal_32m_trim, FALSE);
}

/* get 4 words of information that uniquely identifies the MCU */
void PLATFORM_GetMCUUid(uint8_t *aOutUid16B, uint8_t *pOutLen)
{
    uint32_t uid[4] = {0};

    /* Get the MCU uid */
    uid[0] = MSCM->UID[0];
    uid[1] = MSCM->UID[1];
    uid[2] = MSCM->UID[2];
    uid[3] = MSCM->UID[3];

    FLib_MemCpy(aOutUid16B, (uint8_t *)uid, sizeof(uid));
    /* Get the uid length */
    *pOutLen = (uint8_t)sizeof(uid);

    return;
}

/* get the device MAC ADDRESS of 40 bits */
void PLATFORM_GetMACAddr(uint8_t *aOutMacAddr5B)
{
    aOutMacAddr5B[4] = (uint8_t)(RADIO_CTRL->UID_MSB) & 0xFFU;
    aOutMacAddr5B[3] = (uint8_t)(RADIO_CTRL->UID_LSB >> 24) & 0xFFU;
    aOutMacAddr5B[2] = (uint8_t)(RADIO_CTRL->UID_LSB >> 16) & 0xFFU;
    aOutMacAddr5B[1] = (uint8_t)(RADIO_CTRL->UID_LSB >> 8) & 0xFFU;
    aOutMacAddr5B[0] = (uint8_t)(RADIO_CTRL->UID_LSB) & 0xFFU;
}

uint8_t PLATFORM_GetXtal32MhzTrim(bool_t regRead)
{
    uint8_t retVal;

    /* get the XTAL trim value sent to the NBU */
    retVal = Xtal32MhzTrim;

    return retVal;
}

/* Calling this function assumes HWParameters in flash have been read */
void PLATFORM_SetXtal32MhzTrim(uint8_t trimValue, bool_t saveToHwParams)
{
    uint32_t              status;
    hardwareParameters_t *pHWParams = NULL;
    status                          = NV_ReadHWParameters(&pHWParams);
    if ((TRUE == saveToHwParams) && (status == 0U))
    {
        /* update value only if it changed */
        pHWParams->xtalTrim = trimValue;
        (void)NV_WriteHWParameters();
    }

    /* keep value for future usage */
    Xtal32MhzTrim = trimValue;

    /* Send trim value to NBU so it can be updated */
    (void)PLATFORM_FwkSrvSendPacket(gFwkSrvXtal32MTrimIndication_c, &Xtal32MhzTrim,
                                    (uint16_t)sizeof(pHWParams->xtalTrim));
}

int PLATFORM_InitTimerManager(void)
{
    int status = 0;

    if (timer_manager_initialized == 0)
    {
        timer_status_t tm_st;
        timer_config_t timerConfig;

        timerConfig.instance       = PLATFORM_TM_INSTANCE;
        timerConfig.srcClock_Hz    = PLATFORM_TM_CLK_FREQ;
        timerConfig.clockSrcSelect = PLATFORM_TM_CLK_SELECT;

#if (defined(TM_ENABLE_TIME_STAMP) && (TM_ENABLE_TIME_STAMP > 0U))
        timerConfig.timeStampSrcClock_Hz    = PLATFORM_TM_STAMP_CLK_FREQ;
        timerConfig.timeStampInstance       = PLATFORM_TM_STAMP_INSTANCE;
        timerConfig.timeStampClockSrcSelect = PLATFORM_TM_STAMP_CLK_SELECT;
#endif

        tm_st = TM_Init(&timerConfig);
        if (tm_st != kStatus_TimerSuccess)
        {
            status = RAISE_ERROR(tm_st, 1);
            assert(0);
        }
        else
        {
            /* Timer Manager initialization completed */
            timer_manager_initialized = 1;
        }
    }
    else
    {
        /* Timer Manager already initialized */
        status = 1;
    }
    return status;
}

void PLATFORM_DeinitTimerManager(void)
{
    if (timer_manager_initialized == 1)
    {
        TM_Deinit();
        timer_manager_initialized = 0;
    }
}

void PLATFORM_InitCrypto(void)
{
    CLOCK_EnableClock(kCLOCK_Secsubsys);
    LTC_Init(LTC0);
}

void PLATFORM_GetTimestamp(uint64_t *now)
{
    void *ptr;
    ptr = TSTMR0;
    /* A complete read operation should include both TSTMR LOW and HIGH reads. If a HIGH read does not follow a LOW
     * read, then any other Time Stamp value read will be locked at a fixed value. The TSTMR LOW read should occur
     * first, followed by the TSTMR HIGH read.  */
    *now = *(volatile uint64_t *)ptr;
}

void PLATFORM_WaitTimeout(uint64_t timestamp, uint64_t delay_us)
{
    uint64_t now, then;

    then = timestamp + delay_us;

    if (then > PLATFORM_TIMESTAMP_MAX_VALUE)
    {
        /* Make sure we handle counter wrapping */
        then = then - PLATFORM_TIMESTAMP_MAX_VALUE - 1U;
    }

    do
    {
        PLATFORM_GetTimestamp(&now);
    } while (now < then);
}

void PLATFORM_Delay(uint64_t delay_us)
{
    uint64_t now;

    /* PLATFORM_Delay() is similar to PLATFORM_WaitTimeout() but timestamp is taken righ now */
    PLATFORM_GetTimestamp(&now);
    PLATFORM_WaitTimeout(now, delay_us);
}

void PLATFORM_DisableControllerLowPower(void)
{
    /* Increase active request number so it is always asserted while Controller
     * is not allowed to go to low power
     * This will avoid going through the wake up procedure each time
     * PLATFORM_RemoteActiveReq is called
     * Note: this must be called before accessing RF_CMC, as it is in NBU domain
     */
    PLATFORM_RemoteActiveReq();

    /* Disallow NBU to go to low power
     * NBU FW will check this bit before going to low power
     * if it is set, NBU will go to WFI only
     * Note: If NBU is already in low power, this will apply to next Idle period
     */
    RF_CMC1->RADIO_LP |= RF_CMC1_RADIO_LP_BLE_WKUP_MASK;
}

void PLATFORM_RemoteActiveReq(void)
{
    BOARD_DBGLPIOSET(1, 1);
    BOARD_DBGLPIOSET(0, 1);

    OSA_InterruptDisable();

    if (active_request_nb == 0)
    {
        uint32_t rfmc_ctrl    = RFMC->RF2P4GHZ_CTRL;
        bool     remote_in_lp = false;

        /* CM3 writes to WKUP_TIME register to notify CM33 it's going to low
         * power, this is a software protocol to sync both cores */
        while ((RFMC->RF2P4GHZ_MAN2 & RFMC_RF2P4GHZ_MAN2_WKUP_TIME_MASK) != 0U)
        {
            /* CM3 started low power entry, to workaround HW issues, we need to
             * wait for the radio to fully enter low power before waking it up */
            if ((RFMC->RF2P4GHZ_STAT & RFMC_RF2P4GHZ_STAT_BLE_STATE_MASK) == RFMC_RF2P4GHZ_STAT_BLE_STATE(0x2U))
            {
                /* Radio is in low power, we can exit the loop and wake it up */
                remote_in_lp = true;
                break;
            }
        }

        rfmc_ctrl |= RFMC_RF2P4GHZ_CTRL_BLE_WKUP(0x1U);
        RFMC->RF2P4GHZ_CTRL = rfmc_ctrl;

        __DSB();

        if (remote_in_lp == true)
        {
            /* Wake up time is around 5 periods of 32khz clock (160us)
             * Adding a delay of 120us shouldn't impact waiting time, and will
             * make sure the BLE_STATE is reliable */
            PLATFORM_Delay(120U);
        }

        /* Wait for the NBU to become active */
        while ((RFMC->RF2P4GHZ_STAT & RFMC_RF2P4GHZ_STAT_BLE_STATE_MASK) != RFMC_RF2P4GHZ_STAT_BLE_STATE(0x1U))
        {
            BOARD_DBGLPIOSET(0, 1);
            __ASM("NOP");
            BOARD_DBGLPIOSET(0, 0);
        }
    }
    else
    {
        ;
    }

    active_request_nb++;

    OSA_InterruptEnable();

    BOARD_DBGLPIOSET(0, 0);
}

void PLATFORM_RemoteActiveRel(void)
{
    BOARD_DBGLPIOSET(0, 1);

    OSA_InterruptDisable();

    assert(active_request_nb > 0);
    active_request_nb--;

    if (active_request_nb == 0)
    {
        uint32_t rfmc_ctrl;
        rfmc_ctrl = RFMC->RF2P4GHZ_CTRL;
        rfmc_ctrl &= ~RFMC_RF2P4GHZ_CTRL_BLE_WKUP_MASK;
        RFMC->RF2P4GHZ_CTRL = rfmc_ctrl;
        BOARD_DBGLPIOSET(1, 0);
    }

    OSA_InterruptEnable();

    BOARD_DBGLPIOSET(0, 0);
}

void PLATFORM_GetResetCause(PLATFORM_ResetStatus_t *reset_status)
{
    uint32_t SSRS_value;
    SSRS_value = CMC0->SSRS;

    if (((SSRS_value & CMC_SSRS_WAKEUP_MASK) == CMC_SSRS_WAKEUP_MASK) &&
        !((SSRS_value & CMC_SSRS_PIN_MASK) == CMC_SSRS_PIN_MASK) &&
        !((SSRS_value & CMC_SSRS_DAP_MASK) == CMC_SSRS_DAP_MASK))
    {
        *reset_status = PLATFORM_LowPowerWakeup;
    }
    else
    {
        *reset_status = PLATFORM_DeviceReset;
    }

    CMC0->SSRS = CMC0->SSRS; // clear SSRS
}

void mcmgr_imu_remote_active_rel(void)
{
    PLATFORM_RemoteActiveRel();
}

void mcmgr_imu_remote_active_req(void)
{
    PLATFORM_RemoteActiveReq();
}
