/* -------------------------------------------------------------------------- */
/*                           Copyright 2021-2022 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "fsl_common.h"
#include "fsl_adapter_rpmsg.h"
#include "fwk_platform_ble.h"
#include "fwk_platform.h"
#include "fwk_platform_ics.h"

#if defined(gBoardUseFro32k_d) && (gBoardUseFro32k_d > 0)
#include "fwk_sfc.h"
#endif

#include "FunctionLib.h"
#if defined(gMWS_Enabled_d) && (gMWS_Enabled_d == 1)
#include "MWS.h"
#endif
#include "HWParameter.h"
#include "RNG_Interface.h"
// #include "fwk_platform_coex.h"
#define connBle_c      (uint8_t)(1U << 0)

#ifdef SERIAL_BTSNOOP
#include "sbtsnoop.h"
#endif

#include "fwk_debug.h"

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */
#define PLATFORM_BLE_BD_ADDR_RAND_PART_SIZE 3U
#define PLATFORM_BLE_BD_ADDR_OUI_PART_SIZE  3U
#define PLATFORM_BLE_BD_ADDR_FULL_SIZE      (PLATFORM_BLE_BD_ADDR_RAND_PART_SIZE + PLATFORM_BLE_BD_ADDR_OUI_PART_SIZE)

#define mBoardUidSize_c 16

#ifndef BD_ADDR_OUI
#define BD_ADDR_OUI 0x37U, 0x60U, 0x00U
#endif

#define HciCommand(opCodeGroup, opCodeCommand) \
    (((uint16_t)(opCodeGroup) & (uint16_t)0x3FU) << (uint16_t)SHIFT10) | (uint16_t)((opCodeCommand)&0x3FFU)

#define BT_USER_BD 254

/* Check if __st is negative,  if true, apply 4 bits shit and add new __error_code,
    assert in debug and break
   Shall be called in a do while(0) bracket */
#define CHECK_AND_RAISE_ERROR(__st, __error_code)                                                  \
    {                                                                                              \
        if ((__st) < 0)                                                                            \
        {                                                                                          \
            assert(0);                                                                             \
            (__st) = -(int)((uint32_t)(((uint32_t)(-(__st)) << 4) | (uint32_t)(-(__error_code)))); \
            break;                                                                                 \
        }                                                                                          \
    }

/* Raise error with status update , shift previous status by 4 bits and OR with new error code.
 * the returned status will be negative */
#define RAISE_ERROR(__st, __error_code) -(int)((uint32_t)(((uint32_t)(__st) << 4) | (uint32_t)(__error_code)));

// /*!
//  * \brief RPMSG Rx callback used to receive HCI messages from Controller
//  *
//  * \param[in] param Usually NULL
//  * \param[in] data pointer to data buffer
//  * \param[in] len size of the data
//  * \return hal_rpmsg_return_status_t tells RPMSG to free or hold the buffer
//  */
// static hal_rpmsg_return_status_t PLATFORM_HciRpmsgRxCallback(void *param, uint8_t *data, uint32_t len);

/* -------------------------------------------------------------------------- */
/*                         Private memory declarations                        */
/* -------------------------------------------------------------------------- */
#if (gPlatformUseUniqueDeviceIdForBdAddr_d == 0)
static const uint8_t gBD_ADDR_OUI_c[PLATFORM_BLE_BD_ADDR_OUI_PART_SIZE] = {BD_ADDR_OUI};
#endif

/* RPMSG related variables */

/* Define hci serial manager handle*/
static RPMSG_HANDLE_DEFINE(hciRpmsgHandle);

/*hci rpmsg configuration*/
static const hal_rpmsg_config_t hciRpmsgConfig = {
    .local_addr  = 40,
    .remote_addr = 30,
};

static void (*hci_rx_callback)(uint8_t packetType, uint8_t *data, uint16_t len);

// static hal_rpmsg_config_t hci_rpmsg_config = {
//     .local_addr  = 30,
//     .remote_addr = 40,
//     // .imuLink     = (uint8_t)kIMU_LinkCpu2Cpu3,
//     .imuLink     = (uint8_t)kIMU_LinkCpu1Cpu2,
//     .callback    = PLATFORM_HciRpmsgRxCallback,
//     .param       = NULL,
// };

static bool              initialized    = false;
static bool              hciInitialized = false;

// static OSA_EVENT_HANDLE_DEFINE(wakeUpEventGroup);
// static OSA_MUTEX_HANDLE_DEFINE(bleMutexHandle);

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Init HCI link with BLE controller
 *
 */
static int PLATFORM_InitHciLink(void);

/*!
 * \brief RPMSG Rx callback used to receive HCI messages from Controller
 *
 * \param[in] param Usually NULL
 * \param[in] data pointer to data buffer
 * \param[in] len size of the data
 * \return hal_rpmsg_return_status_t tells RPMSG to free or hold the buffer
 */
static hal_rpmsg_return_status_t PLATFORM_HciRpmsgRxCallback(void *param, uint8_t *data, uint32_t len);

/*!
 * \brief Configure max TX power in dBm for BLE
 *
 * \param[in] max_tx_power Desired max TX power in dBm
 */
static void PLATFORM_SetBleMaxTxPower(int8_t max_tx_power);

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

int PLATFORM_InitBle(void)
{
    int status = 0;

    do
    {
        status = PLATFORM_InitTimerManager();
        CHECK_AND_RAISE_ERROR(status, -1);

        /* Init NBU domain and configure RFMC module
         * CM3 is still in reset and will be released by MCMGR_StartCore during RPMSG init */
        status = PLATFORM_InitNbu();
        CHECK_AND_RAISE_ERROR(status, -2);

        /* Init of the multicore*/
        status = PLATFORM_InitMulticore();
        CHECK_AND_RAISE_ERROR(status, -3);

        /* Init HCI link with NBU */
        status = PLATFORM_InitHciLink();
        CHECK_AND_RAISE_ERROR(status, -4);

        /* Initialize PLatform Service intercore channel
         *  Used to retrieve NBU version information but not retricted to this sole use.
         */
        status = PLATFORM_FwkSrvInit();
        CHECK_AND_RAISE_ERROR(status, -5);

#if defined(gMWS_Enabled_d) && (gMWS_Enabled_d == 1)
        MWS_Init();
#endif

        /* Send chip revision (A0 or A1) to NBU */
        status = PLATFORM_SendChipRevision();
        CHECK_AND_RAISE_ERROR(status, -6);

        /* Load HW parameters to RAM and set the 32Mhz Trim value */
        PLATFORM_LoadHwParams();

        PLATFORM_SetBleMaxTxPower(gAppMaxTxPowerDbm_c);

#if !defined(gBoardUseFro32k_d) || (gBoardUseFro32k_d == 0)
        /* Make sure OSC32k is ready and select it as clock source */
        status = PLATFORM_SwitchToOsc32k();
        CHECK_AND_RAISE_ERROR(status, -7);
#endif

        /* Initialize log handle for second core */
        BOARD_DBGCONFIGINITNBU(TRUE);
        // DBG_LOG_DUMP();

    } while (false);

    return status;
}

int PLATFORM_SetHciRxCallback(void (*callback)(uint8_t packetType, uint8_t *data, uint16_t len))
{
    int ret = 0;

    hci_rx_callback = callback;

    return ret;
}

void PLATFORM_SendHciMessage(uint8_t *msg, uint32_t len)
{
    /* Wake up controller before sending the message */
    PLATFORM_RemoteActiveReq();

    /* Send HCI Packet through RPMSG channel */
    (void)HAL_RpmsgSend(hciRpmsgHandle, msg, len);

    /* Release wake up request */
    PLATFORM_RemoteActiveRel();

#ifdef SERIAL_BTSNOOP
    sbtsnoop_write_hci_pkt(msg[0U], 0U, &msg[1], (len - 1U));
#endif
}

#if (gPlatformUseUniqueDeviceIdForBdAddr_d == 0)
static void PLATFORM_GenerateNewBDAddr(uint8_t *bleDeviceAddress)
{
    uint8_t ret;
    uint8_t macAddr[PLATFORM_BLE_BD_ADDR_RAND_PART_SIZE] = {0U};
    int16_t num;

    ret = RNG_Init();
    assert(ret == 0);
    (void)ret;

    /* Parameter pSeed ignored - please set to NULL */
    RNG_SetPseudoRandomNoSeed(NULL);

    num = RNG_GetPseudoRandomNo(macAddr, PLATFORM_BLE_BD_ADDR_RAND_PART_SIZE, NULL);
    assert(num == PLATFORM_BLE_BD_ADDR_RAND_PART_SIZE);
    (void)num;

    /* Set 3 LSB from mac address */
    FLib_MemCpy((void *)bleDeviceAddress, (const void *)macAddr, PLATFORM_BLE_BD_ADDR_RAND_PART_SIZE);
    /* Set 3 MSB from OUI */
    FLib_MemCpy((void *)&bleDeviceAddress[PLATFORM_BLE_BD_ADDR_RAND_PART_SIZE], (const void *)gBD_ADDR_OUI_c,
                PLATFORM_BLE_BD_ADDR_OUI_PART_SIZE);
}
#endif

void PLATFORM_GetBDAddr(uint8_t *bleDeviceAddress)
{
    hardwareParameters_t *pHWParams = NULL;
    uint32_t              status;

    status = NV_ReadHWParameters(&pHWParams);

    /* FLib_MemCmpToVal mandatory to make sure BLE mac address is valid
     * because return status of NV_ReadHWParameters is 1 only at 1st read attempt */
    if ((status == 0U) &&
        (FLib_MemCmpToVal((const void *)pHWParams->bluetooth_address, 0xFFU, PLATFORM_BLE_BD_ADDR_FULL_SIZE) == FALSE))
    {
        uint32_t regPrimask;

        regPrimask = DisableGlobalIRQ();
        FLib_MemCpy((void *)bleDeviceAddress, (const void *)pHWParams->bluetooth_address,
                    PLATFORM_BLE_BD_ADDR_FULL_SIZE);
        EnableGlobalIRQ(regPrimask);
    }
    else
    {
        uint32_t regPrimask;

        /* User can decide to use the device unique address or a random generated address */
#if gPlatformUseUniqueDeviceIdForBdAddr_d
        PLATFORM_GetMACAddr(bleDeviceAddress);
#else
        PLATFORM_GenerateNewBDAddr(bleDeviceAddress);
#endif

        regPrimask = DisableGlobalIRQ();
        FLib_MemCpy((void *)pHWParams->bluetooth_address, (void *)bleDeviceAddress, PLATFORM_BLE_BD_ADDR_FULL_SIZE);

        (void)NV_WriteHWParameters();
        EnableGlobalIRQ(regPrimask);
    }
}

int32_t PLATFORM_EnableBleSecureKeyManagement(void)
{
    int32_t ret = 0;

    ret = PLATFORM_FwkSrvSendPacket(gFwkSrvNbuSecureModeRequest_c, NULL, 0U);

    return ret;
}

bool PLATFORM_CheckNextBleConnectivityActivity(void)
{
    return true;
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static int PLATFORM_InitHciLink(void)
{
    int                status = 0;
    hal_rpmsg_config_t hci_rpmsg_config;
    hal_rpmsg_status_t hci_rpmsg_status;

    hci_rpmsg_config = hciRpmsgConfig;

    hci_rpmsg_status = HAL_RpmsgInit((hal_rpmsg_handle_t)hciRpmsgHandle, &hci_rpmsg_config);
    if (hci_rpmsg_status != kStatus_HAL_RpmsgSuccess)
    {
        assert(0);
        status = RAISE_ERROR(hci_rpmsg_status, 1);
    }
    /* Set RX Callback */
    else
    {
        hci_rpmsg_status =
            HAL_RpmsgInstallRxCallback((hal_rpmsg_handle_t)hciRpmsgHandle, PLATFORM_HciRpmsgRxCallback, NULL);
        if (hci_rpmsg_status != kStatus_HAL_RpmsgSuccess)
        {
            assert(0);
            status = RAISE_ERROR(hci_rpmsg_status, 2);
        }
    }
    return status;
}

static hal_rpmsg_return_status_t PLATFORM_HciRpmsgRxCallback(void *param, uint8_t *data, uint32_t len)
{
    PLATFORM_RemoteActiveReq();

    if (hci_rx_callback != NULL)
    {
        hci_rx_callback(data[0], &data[1], len - 1U);
    }

#ifdef SERIAL_BTSNOOP
    sbtsnoop_write_hci_pkt(data[0U], 1U, &data[1], len - 1U);
#endif

    PLATFORM_RemoteActiveRel();

    (void)param;

    return kStatus_HAL_RL_RELEASE;
}

static void PLATFORM_SetBleMaxTxPower(int8_t max_tx_power)
{
    uint8_t ldo_ana_trim;

    if (max_tx_power == 0)
    {
        ldo_ana_trim = 3U;
    }
    else if (max_tx_power == 7)
    {
        ldo_ana_trim = 9U;
    }
    else
    {
        if (max_tx_power != 10)
        {
            // set to 10dBm if setting is invalid
            assert(false);
            max_tx_power = 10;
        }
        ldo_ana_trim = 15U;
    }

    /* configure max tx power in controller */
    extern void Controller_SetMaxTxPower(int8_t power_dBm, uint8_t ldo_ant_trim);
    Controller_SetMaxTxPower(max_tx_power, ldo_ana_trim);
}

// /*!
//  * \brief Return HCI link status
//  *
//  * \return true Link is ready
//  * \return false Link is not ready yet
//  */
// static bool PLATFORM_IsHciLinkReady(void);

// static bool PLATFORM_IsHciLinkReady(void)
// {
//     return (HAL_ImuLinkIsUp(hci_rpmsg_config.imuLink) == kStatus_HAL_RpmsgSuccess);
// }

// int PLATFORM_StartHci(void)
// {
//     int ret = 0;

//     do
//     {
//         if (hciInitialized == true)
//         {
//             break;
//         }

//         while (PLATFORM_IsHciLinkReady() != true)
//         {
//         }

// #if !defined(gPlatformDisableVendorSpecificInit) || (gPlatformDisableVendorSpecificInit == 0)
//         /* This function call uses HCI vendor commands to configure the controller,
//          * this can cause troubles with some BLE Host. A host can send the HCI commands
//          * using its own API and then expect the right response from the controller, if the
//          * commands are sent under the hood using the framework, the host may receive unexpected
//          * responses which may lead to issues. In this case enable gPlatformDisableVendorSpecificInit.
//          */
//         PLATFORM_VendorSpecificInit();
// #endif
//         hciInitialized = true;
//     } while (false);

//     return ret;
// }

// int PLATFORM_TerminateBle(void)
// {
//     int ret = 0;

//     do
//     {
//         if (initialized == false)
//         {
//             break;
//         }

//         if (PLATFORM_TerminateHciLink() != 0)
//         {
//             ret = -1;
//             break;
//         }

//         if (PLATFORM_TerminateControllers((uint8_t)connBle_c) != 0) /* MISRA CID 26829044 */
//         {
//             ret = -2;
//             break;
//         }

//         // if (OSA_EventDestroy((osa_event_handle_t)wakeUpEventGroup) != KOSA_StatusSuccess)
//         // {
//         //     ret = -3;
//         //     break;
//         // }

//         // if (OSA_MutexDestroy((osa_mutex_handle_t)bleMutexHandle) != KOSA_StatusSuccess)
//         // {
//         //     ret = -4;
//         //     break;
//         // }

//         initialized = false;
//         /* after re-init cpu2, Reset hciInitialized to false. */
//         hciInitialized = false;
//     } while (false);

//     return ret;
// }

// static hal_rpmsg_return_status_t PLATFORM_HciRpmsgRxCallback(void *param, uint8_t *data, uint32_t len)
// {
//     bool    handled    = false;
//     uint8_t packetType = data[0];

//     (void)param;

//     (void)PLATFORM_HandleControllerPowerState();

//     /* If the macro BLE_VENDOR_EVENT_HANDLE is set to true, PLATFORM module will check if it can handle Vendor Specific
//      * Events without going through Ethermind's HCI tasks If the packet is not handled, then it is sent to upper layers
//      * This is likely used to handle Controller low power state, so this is
//      * completely transparent to the application. If the macro BLE_VENDOR_EVENT_HANDLE is set false, the Ethermind's HCI
//      * tasks will handle vendor event */
//     if (packetType == HCI_EVENT_PACKET)
//     {
//         uint8_t eventType = data[1];

//         if (eventType == HCI_VENDOR_SPECIFIC_DEBUG_EVENT)
//         {
//             /* Received packet is a Vendor Specific event, check if PLATFORM
//              * can process it, if not, it will be sent to Ethermind */
//             handled = PLATFORM_HandleHciVendorEvent(&data[3], data[2]);
//         }
//     }

//     if ((handled == false) && (hci_rx_callback != NULL))
//     {
//         hci_rx_callback(packetType, &data[1], (uint16_t)(len - 1U));
//     }

// #ifdef SERIAL_BTSNOOP
//     sbtsnoop_write_hci_pkt(data[0U], 1U, &data[1], (uint16_t)(len - 1U));
// #endif

//     return kStatus_HAL_RL_RELEASE;
// }
