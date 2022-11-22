/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
*   @file
*
*   @addtogroup i3c_ip
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "I3c_Ip.h"
#include "I3c_Ip_HwAccess.h"
#include "SchM_I3c.h"
#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
#include "Dma_Ip.h"
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define I3C_IP_VENDOR_ID_C                      43
#define I3C_IP_AR_RELEASE_MAJOR_VERSION_C       4
#define I3C_IP_AR_RELEASE_MINOR_VERSION_C       7
#define I3C_IP_AR_RELEASE_REVISION_VERSION_C    0
#define I3C_IP_SW_MAJOR_VERSION_C               0
#define I3C_IP_SW_MINOR_VERSION_C               9
#define I3C_IP_SW_PATCH_VERSION_C               0

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and I3c_Ip header file are of the same vendor */
#if (I3C_IP_VENDOR_ID_C != I3C_IP_VENDOR_ID)
    #error "I3c_Ip.c and I3c_Ip.h have different vendor ids"
#endif

/* Check if current file and I3c_Ip header file are of the same Autosar version */
#if ((I3C_IP_AR_RELEASE_MAJOR_VERSION_C     != I3C_IP_AR_RELEASE_MAJOR_VERSION) || \
     (I3C_IP_AR_RELEASE_MINOR_VERSION_C     != I3C_IP_AR_RELEASE_MINOR_VERSION) || \
     (I3C_IP_AR_RELEASE_REVISION_VERSION_C  != I3C_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of I3c_Ip.c and I3c_Ip.h are different"
#endif

/* Check if current file and I3c_Ip header file are of the same software version */
#if ((I3C_IP_SW_MAJOR_VERSION_C != I3C_IP_SW_MAJOR_VERSION) || \
     (I3C_IP_SW_MINOR_VERSION_C != I3C_IP_SW_MINOR_VERSION) || \
     (I3C_IP_SW_PATCH_VERSION_C != I3C_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of I3c_Ip.c and I3c_Ip.h are different"
#endif

/* Check if current file and I3c_Ip_HwAccess header file are of the same vendor */
#if (I3C_IP_VENDOR_ID_C != I3C_IP_VENDOR_ID_HWACCESS)
    #error "I3c_Ip.c and I3c_Ip_HwAccess.h have different vendor ids"
#endif

/* Check if current file and I3c_Ip_HwAccess header file are of the same Autosar version */
#if ((I3C_IP_AR_RELEASE_MAJOR_VERSION_C     != I3C_IP_AR_RELEASE_MAJOR_VERSION_HWACCESS) || \
     (I3C_IP_AR_RELEASE_MINOR_VERSION_C     != I3C_IP_AR_RELEASE_MINOR_VERSION_HWACCESS) || \
     (I3C_IP_AR_RELEASE_REVISION_VERSION_C  != I3C_IP_AR_RELEASE_REVISION_VERSION_HWACCESS))
    #error "AutoSar Version Numbers of I3c_Ip.c and I3c_Ip_HwAccess.h are different"
#endif

/* Check if current file and I3c_Ip_HwAccess header file are of the same software version */
#if ((I3C_IP_SW_MAJOR_VERSION_C != I3C_IP_SW_MAJOR_VERSION_HWACCESS) || \
     (I3C_IP_SW_MINOR_VERSION_C != I3C_IP_SW_MINOR_VERSION_HWACCESS) || \
     (I3C_IP_SW_PATCH_VERSION_C != I3C_IP_SW_PATCH_VERSION_HWACCESS))
    #error "Software Version Numbers of I3c_Ip.c and I3c_Ip_HwAccess.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and SchM_I3c header file are of the same Autosar version */
    #if ((I3C_IP_AR_RELEASE_MAJOR_VERSION_C != SCHM_I3C_AR_RELEASE_MAJOR_VERSION) || \
        (I3C_IP_AR_RELEASE_MINOR_VERSION_C != SCHM_I3C_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of I3c_Ip.c and SchM_I3c.h are different"
    #endif
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if current file and Dma_Ip header file are of the same Autosar version */
#if ((I3C_IP_AR_RELEASE_MAJOR_VERSION_C != DMA_IP_AR_RELEASE_MAJOR_VERSION) || \
     (I3C_IP_AR_RELEASE_MINOR_VERSION_C != DMA_IP_AR_RELEASE_MINOR_VERSION))
#error "AutoSar Version Numbers of I3c_Ip.c and Dma_Ip.h are different"
#endif
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/
/**
* @brief I3C baud counts max values.
*
* @details These are used to verify that the baud counts values set using I3c_Ip_MasterSetBaudRate()
*          are within hardware limits.
*/
#define I3C_IP_MAX_I2CBAUD          ((uint8)0x0F)
#define I3C_IP_MAX_PPBAUD           ((uint8)0x0F)

/**
* @brief SCL maximum frequency (in Hz) when in I2C Mode
*
* @details Then in I2C Mode, I3c Standard specifies a maximum SCL frequency of 1MHz when communicating
*          with devices that support Fast Mode+. If only Fast Mode is supported, maximum SCL frequency should be 400kHz.
*/
#define I3C_IP_MAX_I2C_BAUD_RATE   (1200000U)

/* Slave ID (Provisional ID + BCR + DCR) size in bytes. */
#define I3C_IP_SLAVE_ID_SIZE        (8U)

/* This mask is used to get the direction bit value (write or read) from an HDR-DDR Command Code */
#define I3C_IP_SLAVE_HDR_DDR_DIRECTION_MASK     ((uint8)0x80)

/* This is used to shift the HDR-DDR Command Code to get 1-bit value for HDR-DDR message direction */
#define I3C_IP_SLAVE_HDR_DDR_DIRECTION_SHIFT    ((uint8)7U)

#if (STD_ON == I3C_IP_SA10B_SUPPORT)
/* This is used to verify that if supported, Static Address 10-bit extension is within hardware limits. */
#define I3C_IP_MAX_SA10B    ((uint8)7U)
#endif /* (STD_ON == I3C_IP_SA10B_SUPPORT) */

#if (STD_ON == I3C_IP_SELF_RESET_SUPPORT)
/* This is the key used to reset the whole I3c peripheral (both master and slave IPs) */
#define I3C_IP_SELF_RESET_KEY       (0xB216E9U)
#endif /* (STD_ON == I3C_IP_SELF_RESET_SUPPORT) */

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
#define I3C_DMA_CHANNEL_CONFIG_LIST_SIZE  (12U)
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
/**
* @brief  This define indicate the maximum number for an In-Band interrupt data bytes, including the mandatory byte.
*         This is a standard I3C Mipi generic value.
*/
#define I3C_IP_IBI_DATA_MAX_SIZE        (8U)
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/
#define I3C_START_SEC_CONST_UNSPECIFIED
#include "I3c_MemMap.h"

/* Table of base addresses for I3C instances. */
static I3C_Type * const I3c_Ip_apxBase[I3C_INSTANCE_COUNT] = IP_I3C_BASE_PTRS;

#define I3C_STOP_SEC_CONST_UNSPECIFIED
#include "I3c_MemMap.h"

#define I3C_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "I3c_MemMap.h"

/* Pointers to runtime state structures */
static I3c_Ip_MasterStateType * I3c_Ip_apxMasterState[I3C_INSTANCE_COUNT]; /* implicit zero initialization: NULL_PTR */
static I3c_Ip_SlaveStateType * I3c_Ip_apxSlaveState[I3C_INSTANCE_COUNT]; /* implicit zero initialization: NULL_PTR */

#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
/* This will be used to store the current Hw Unit mode when the mastership transfers occurrs.
   If no mastership transfer occurs, it will store the initial Hw Unit mode, which remains unchanged. */
static I3c_Ip_MasterSlaveModeType I3c_Ip_aeChannelCurrentMode[I3C_INSTANCE_COUNT];
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES) */

#define I3C_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "I3c_MemMap.h"
/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define I3C_START_SEC_CODE
#include "I3c_MemMap.h"

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
static void I3c_Ip_MasterTxDmaConfig(I3C_Type * const Base,
                                     const I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterRxDmaConfig(I3C_Type * const Base,
                                     const I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_SlaveTxDmaConfig(I3C_Type * const Base,
                                    const I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveRxDmaConfig(I3C_Type * const Base,
                                    const I3c_Ip_SlaveStateType * const Slave);
void I3c_Ip_MasterDmaErrorHandler(const uint8 Instance);
void I3c_Ip_SlaveDmaErrorHandler(const uint8 Instance);
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */

static I3c_Ip_StatusType I3c_Ip_MasterWaitForStatus(const I3C_Type * const Base,
                                                    const uint32 Status);
static void I3c_Ip_MasterSendMessage(I3C_Type * const Base,
                                     I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_SlaveSendMessage(I3C_Type * const Base,
                                    I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_MasterReceiveMessage(const I3C_Type * const Base,
                                        I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_SlaveReceiveMessage(const I3C_Type * const Base,
                                       I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_MasterEndTransfer(I3C_Type * const Base,
                                     I3c_Ip_MasterStateType * const Master,
                                     boolean ResetFifos,
                                     boolean SendStop);
static void I3c_Ip_SlaveEndTransfer(I3C_Type * const Base,
                                    I3c_Ip_SlaveStateType * const Slave,
                                    boolean ResetFifos);
static void I3c_Ip_MasterTxNotFullEventHandler(I3C_Type * const Base,
                                               I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_SlaveTxSendEventHandler(I3C_Type * const Base,
                                           I3c_Ip_SlaveStateType * const Slave);
static I3c_Ip_FifoTriggerLevelType I3c_Ip_MasterConvertRxSizeToWatermark(const I3c_Ip_MasterStateType * const Master);
static I3c_Ip_FifoTriggerLevelType I3c_Ip_SlaveConvertRxSizeToWatermark(const I3C_Type * const Base,
                                                                        uint32 BufferSize);
static void I3c_Ip_MasterRxPendingEventHandler(I3C_Type * const Base,
                                               I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_SlaveRxPendingEventHandler(I3C_Type * const Base,
                                              I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveCCCHandledEventHandler(const I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveStartEventHandler(I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveStopEventHandler(I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveDAChangeEventHandler(const I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveDDRMatchedEventHandler(I3C_Type * const Base,
                                               I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveHeaderMatchedEventHandler(I3C_Type * const Base,
                                                  I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveEventsHandler(const I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveCCCEventHandler(const I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_MasterControlDoneEventHandler(I3C_Type * const Base,
                                                 I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterCompleteEventHandler(I3C_Type * const Base,
                                              I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterSlaveStartEventHandler(I3C_Type * const Base,
                                                const I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterArbitrationLostEventHandler(const uint8 Instance,
                                                     I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterNowMasterEventHandler(const uint8 Instance,
                                               const I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterErrorEventHandler(I3C_Type * const Base,
                                           I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_SlaveErrorEventHandler(I3C_Type * const Base,
                                          I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_MasterCheckFifoEvents(I3C_Type * const Base,
                                         I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_SlaveCheckFifoEvents(I3C_Type * const Base,
                                        I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveCheckCCCEvents(I3C_Type * const Base,
                                       const I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveCheckStartStopEvents(I3C_Type * const Base,
                                             I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveCheckDAChangeEvents(I3C_Type * const Base,
                                            const I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveCheckHeaderMatchedEvents(I3C_Type * const Base,
                                                 I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveCheckEvents(I3C_Type * const Base,
                                    const I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_MasterCheckCompletionEvents(I3C_Type * const Base,
                                               I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterCheckSlaveRequestEvents(const uint8 Instance,
                                                 I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterCheckErrorEvents(I3C_Type * const Base,
                                          I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_SlaveCheckErrorEvents(I3C_Type * const Base,
                                         I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_MasterIRQHandler(const uint8 Instance);
static void I3c_Ip_SlaveIRQHandler(const uint8 Instance);
static void I3c_Ip_MasterInitTransfer(I3C_Type * const Base,
                                      const I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterSendData(I3C_Type * const Base,
                                  I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterReceiveData(const I3C_Type * const Base,
                                     I3c_Ip_MasterStateType * const Master);
#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
static I3c_Ip_StatusType I3c_Ip_MasterSendDAARequest(const uint8 Instance,
                                                     const uint8 * const DynamicAddressList,
                                                     I3c_Ip_SlaveDeviceType * const SlaveDeviceList,
                                                     uint8 * SlaveIndex);
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES) */

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
static void I3c_Ip_SlaveWriteIbiExtraBytes(const uint8 Instance,
                                           const uint8 * ExtData);
static void I3c_Ip_SlaveRequestIbi(const uint8 Instance,
                                   const uint8 IbiData,
                                   const uint8 * ExtData);
static void I3c_Ip_MasterProcessIbi(I3C_Type * const Base,
                                    I3c_Ip_MasterStateType * const Master);
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */
static uint8 I3c_MasterComputeBestI2cBaudCount(const uint32 OpenDrainBaudRate,
                                               const uint32 TargetI2cBaudRate,
                                               const uint8 CurrentI2cBaudCount);
static uint8 I3c_Ip_ComputePpBaudRate(I3C_Type * const Base,
                                      const uint32 FunctionalClock,
                                      const uint32 TargetPpBaudRate,
                                      const I3c_Ip_BusType BusType);
static uint8 I3c_Ip_ComputeOdBaudRate(const I3C_Type * const Base,
                                      const uint8 PpBaudCount,
                                      const uint32 FunctionalClock,
                                      const uint32 TargetOdBaudRate);
static uint8 I3c_Ip_ComputeI2cBaudRate(const uint8 PpBaudCount,
                                       const uint8 OdBaudCount,
                                       const uint32 FunctionalClock,
                                       const uint32 TargetI2cBaudRate);
static void I3c_Ip_MasterTxConfig(const uint8 Instance);
static I3c_Ip_StatusType I3c_Ip_MasterSendMessageBlocking(I3C_Type * const Base,
                                                          I3c_Ip_MasterStateType * const Master);
static I3c_Ip_StatusType I3c_Ip_MasterReceiveMessageBlocking(I3C_Type * const Base,
                                                             I3c_Ip_MasterStateType * const Master);
#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
static void I3c_Ip_MasterProcessHotJoin(I3C_Type * const Base,
                                        const I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterProcessMasterRequest(I3C_Type * const Base,
                                              const I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterProcessCtrlDoneHJ(I3C_Type * const Base,
                                           I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterProcessCtrlDoneIBI(I3C_Type * const Base,
                                            I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterProcessCtrlDoneMR(I3C_Type * const Base,
                                           I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterEmitStop(I3C_Type * const Base,
                                  const boolean SendStop);
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

static void I3c_Ip_MasterCheckWriteHdrDdrCommandCode(I3C_Type * const Base,
                                                     const I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterWriteByteMessage(I3C_Type * const Base,
                                          I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterWriteHalfWordMessage(I3C_Type * const Base,
                                              I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_SlaveWriteByteMessage(I3C_Type * const Base,
                                         I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveWriteHalfWordMessage(I3C_Type * const Base,
                                             I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_MasterReceiveByteMessage(const I3C_Type * const Base,
                                            I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_MasterReceiveHalfWordMessage(const I3C_Type * const Base,
                                                I3c_Ip_MasterStateType * const Master);
static void I3c_Ip_SlaveReceiveByteMessage(const I3C_Type * const Base,
                                           I3c_Ip_SlaveStateType * const Slave);
static void I3c_Ip_SlaveReceiveHalfWordMessage(const I3C_Type * const Base,
                                               I3c_Ip_SlaveStateType * const Slave);
#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
static I3c_Ip_StatusType I3c_Ip_AssignDA(const uint8 Instance,
                                         const uint8 * const DynamicAddressList,
                                         I3c_Ip_SlaveDeviceType * const SlaveDeviceList,
                                         uint8 DynamicAddressCount);
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES) */
void I3c_Ip_IRQHandler(const uint8 Instance);

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterTxDmaConfig
* Description   : This function configures DMA Tx channel for master transfer.
*
* END**************************************************************************/
static void I3c_Ip_MasterTxDmaConfig
(
    I3C_Type * const Base,
    const I3c_Ip_MasterStateType * const Master
)
{
    Dma_Ip_LogicChannelTransferListType DmaTransferList[I3C_DMA_CHANNEL_CONFIG_LIST_SIZE];
    Dma_Ip_ReturnType eDmaReturnStatus;
    uint32 DmaCtrl = I3C_MDMACTRL_DMATB(2U);

    /* Source configuration parameters  */
    DmaTransferList[0U].Param = DMA_IP_CH_SET_SOURCE_ADDRESS;
    DmaTransferList[0U].Value = (uint32)(&(Master->TxDataBuffer[0U]));
    DmaTransferList[1U].Param = DMA_IP_CH_SET_SOURCE_SIGNED_OFFSET;
    DmaTransferList[2U].Param = DMA_IP_CH_SET_SOURCE_TRANSFER_SIZE;
    /* Destination configuration parameters */
    DmaTransferList[3U].Param = DMA_IP_CH_SET_DESTINATION_ADDRESS;
    DmaTransferList[4U].Param = DMA_IP_CH_SET_DESTINATION_SIGNED_OFFSET;
    DmaTransferList[4U].Value = 0U;
    DmaTransferList[5U].Param = DMA_IP_CH_SET_DESTINATION_TRANSFER_SIZE;
    /* Minor/Major Loop parameters */
    DmaTransferList[6U].Param = DMA_IP_CH_SET_MINORLOOP_SIZE;
    DmaTransferList[7U].Param = DMA_IP_CH_SET_MAJORLOOP_COUNT;
    /* Disable Hw auto request */
    DmaTransferList[8U].Param = DMA_IP_CH_SET_CONTROL_DIS_AUTO_REQUEST;
    DmaTransferList[8U].Value = 0U;
    /* Adjustment added to source address parameter */
    DmaTransferList[9U].Param = DMA_IP_CH_SET_SOURCE_SIGNED_LAST_ADDR_ADJ;
    DmaTransferList[9U].Value = 0U;
    DmaTransferList[10U].Param = DMA_IP_CH_SET_DESTINATION_SIGNED_LAST_ADDR_ADJ;
    DmaTransferList[10U].Value = 4U; /* This ensures that last data byte is written into END register */
    /* Enable major interrupt parammeter */
    DmaTransferList[11U].Param = DMA_IP_CH_SET_CONTROL_EN_MAJOR_INTERRUPT;
    DmaTransferList[11U].Value = 0U;

    if (I3C_IP_TRANSFER_HALF_WORDS == Master->TransferOption.TransferSize)
    {
        DmaCtrl |= I3C_MDMACTRL_DMAWIDTH(2U);
        DmaTransferList[1U].Value = 2U; /* SourceSignedOffset */
        DmaTransferList[2U].Value = DMA_IP_TRANSFER_SIZE_2_BYTE; /* SourceXferSize */
        DmaTransferList[3U].Value = (uint32)(&(Base->MWDATAH)); /* DestinationAddress */
        DmaTransferList[5U].Value = DMA_IP_TRANSFER_SIZE_2_BYTE; /* DestinationXferSize */
        DmaTransferList[6U].Value = 2U; /* MinorLoopSize */
        DmaTransferList[7U].Value = (Master->BufferSize >> 1U) - 1U; /* MajorLoopCount */
    }
    else
    {
        DmaCtrl |= I3C_MDMACTRL_DMAWIDTH(1U);
        DmaTransferList[1U].Value = 1U; /* SourceSignedOffset */
        DmaTransferList[2U].Value = DMA_IP_TRANSFER_SIZE_1_BYTE; /* SourceXferSize */
        DmaTransferList[3U].Value = (uint32)(&(Base->MWDATAB)); /* DestinationAddress */
        DmaTransferList[5U].Value = DMA_IP_TRANSFER_SIZE_1_BYTE; /* DestinationXferSize */
        DmaTransferList[6U].Value = 1U; /* MinorLoopSize */
        DmaTransferList[7U].Value = Master->BufferSize - 1U; /* MajorLoopCount */
    }

    /* Clear error status before setting up DMA configuration */
    eDmaReturnStatus = Dma_Ip_SetLogicChannelCommand(Master->DmaTxChannel, DMA_IP_CH_CLEAR_ERROR);
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(DMA_IP_STATUS_SUCCESS == eDmaReturnStatus);
#endif
    /* Configure DMA channel */
    eDmaReturnStatus = Dma_Ip_SetLogicChannelTransferList(Master->DmaTxChannel, DmaTransferList, I3C_DMA_CHANNEL_CONFIG_LIST_SIZE);
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(DMA_IP_STATUS_SUCCESS == eDmaReturnStatus);
#endif
    /* Start DMA channel */
    eDmaReturnStatus = Dma_Ip_SetLogicChannelCommand(Master->DmaTxChannel, DMA_IP_CH_SET_HARDWARE_REQUEST);
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(DMA_IP_STATUS_SUCCESS == eDmaReturnStatus);
#endif

    /* Set FIFO watermark */
    I3c_Ip_MasterSetTxFifoWatermark(Base, I3C_IP_FIFO_TRIGGER_ON_ALMOST_FULL);

    /* Enable DMA */
    Base->MDMACTRL = DmaCtrl;
    (void)eDmaReturnStatus;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterRxDmaConfig
* Description   : This function configures DMA Rx channel for master transfer.
*
* END**************************************************************************/
static void I3c_Ip_MasterRxDmaConfig
(
    I3C_Type * const Base,
    const I3c_Ip_MasterStateType * const Master
)
{
    Dma_Ip_LogicChannelTransferListType DmaTransferList[I3C_DMA_CHANNEL_CONFIG_LIST_SIZE];
    Dma_Ip_ReturnType eDmaReturnStatus;
    uint32 DmaCtrl = I3C_MDMACTRL_DMAFB(2U);

    /* Source configuration parameters  */
    DmaTransferList[0U].Param = DMA_IP_CH_SET_SOURCE_ADDRESS;
    DmaTransferList[1U].Param = DMA_IP_CH_SET_SOURCE_SIGNED_OFFSET;
    DmaTransferList[1U].Value = 0U; /* SourceSignedOffset */
    DmaTransferList[2U].Param = DMA_IP_CH_SET_SOURCE_TRANSFER_SIZE;
    /* Destination configuration parameters  */
    DmaTransferList[3U].Param = DMA_IP_CH_SET_DESTINATION_ADDRESS;
    DmaTransferList[3U].Value = (uint32)(&(Master->RxDataBuffer[0U]));
    DmaTransferList[4U].Param = DMA_IP_CH_SET_DESTINATION_SIGNED_OFFSET;
    DmaTransferList[5U].Param = DMA_IP_CH_SET_DESTINATION_TRANSFER_SIZE;
    /* Minor/Major Loop parameters */
    DmaTransferList[6U].Param = DMA_IP_CH_SET_MINORLOOP_SIZE;
    DmaTransferList[7U].Param = DMA_IP_CH_SET_MAJORLOOP_COUNT;
    /* Disable Hw auto request */
    DmaTransferList[8U].Param = DMA_IP_CH_SET_CONTROL_DIS_AUTO_REQUEST;
    DmaTransferList[8U].Value = 1U;
    /* Adjustment added to source address parameter */
    DmaTransferList[9U].Param = DMA_IP_CH_SET_SOURCE_SIGNED_LAST_ADDR_ADJ;
    DmaTransferList[9U].Value = 0U;
    DmaTransferList[10U].Param = DMA_IP_CH_SET_DESTINATION_SIGNED_LAST_ADDR_ADJ;
    DmaTransferList[10U].Value = 0U;
    /* Enable major interrupt parammeter */
    DmaTransferList[11U].Param = DMA_IP_CH_SET_CONTROL_EN_MAJOR_INTERRUPT;
    DmaTransferList[11U].Value = 0U;

    /* Source and Destination configuration values */
    if (I3C_IP_TRANSFER_HALF_WORDS == Master->TransferOption.TransferSize)
    {
        DmaCtrl |= I3C_MDMACTRL_DMAWIDTH(2U);
        DmaTransferList[0U].Value = (uint32)(&(Base->MRDATAH));  /* SourceAddress */
        DmaTransferList[2U].Value = DMA_IP_TRANSFER_SIZE_2_BYTE; /* SourceXferSize */
        DmaTransferList[4U].Value = 2U; /* DestinationSignedOffset */
        DmaTransferList[5U].Value = DMA_IP_TRANSFER_SIZE_2_BYTE; /* DestinationXferSize */
        DmaTransferList[6U].Value = 2U; /* MinorLoopSize */
        DmaTransferList[7U].Value = (Master->BufferSize >> 1U); /* MajorLoopCount */
    }
    else
    {
        DmaCtrl |= I3C_MDMACTRL_DMAWIDTH(1U);
        DmaTransferList[0U].Value = (uint32)(&(Base->MRDATAB)); /* SourceAddress */
        DmaTransferList[2U].Value = DMA_IP_TRANSFER_SIZE_1_BYTE; /* SourceXferSize */
        DmaTransferList[4U].Value = 1U; /* DestinationSignedOffset */
        DmaTransferList[5U].Value = DMA_IP_TRANSFER_SIZE_1_BYTE; /* DestinationXferSize */
        DmaTransferList[6U].Value = 1U; /* MinorLoopSize */
        DmaTransferList[7U].Value = Master->BufferSize; /* MajorLoopCount */
    }

    /* Clear error status before setting up DMA configuration */
    eDmaReturnStatus = Dma_Ip_SetLogicChannelCommand(Master->DmaRxChannel, DMA_IP_CH_CLEAR_ERROR);
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(DMA_IP_STATUS_SUCCESS == eDmaReturnStatus);
#endif
    /* Configure DMA channel */
    (void)Dma_Ip_SetLogicChannelTransferList(Master->DmaRxChannel, DmaTransferList, I3C_DMA_CHANNEL_CONFIG_LIST_SIZE);
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(DMA_IP_STATUS_SUCCESS == eDmaReturnStatus);
#endif
    /* Start DMA channel */
    (void)Dma_Ip_SetLogicChannelCommand(Master->DmaRxChannel, DMA_IP_CH_SET_HARDWARE_REQUEST);
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(DMA_IP_STATUS_SUCCESS == eDmaReturnStatus);
#endif

    /* Enter critical section */
    SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_01();

    /* Set DMA Trigger for any data in FIFO */
    I3c_Ip_MasterSetRxFifoWatermark(Base, I3C_IP_FIFO_TRIGGER_ON_EMPTY);

    /* Exit critical section */
    SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_01();

    /* Enable DMA */
    Base->MDMACTRL = DmaCtrl;
    (void)eDmaReturnStatus;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveTxDmaConfig
* Description   : This function configures DMA Tx channel for slave transfer.
*
* END**************************************************************************/
static void I3c_Ip_SlaveTxDmaConfig
(
    I3C_Type * const Base,
    const I3c_Ip_SlaveStateType * const Slave
)
{
    Dma_Ip_LogicChannelTransferListType DmaTransferList[I3C_DMA_CHANNEL_CONFIG_LIST_SIZE];
    Dma_Ip_ReturnType eDmaReturnStatus;
    uint32 DmaCtrl = I3C_SDMACTRL_DMATB(2U);

    /* Source configuration parameters  */
    DmaTransferList[0U].Param = DMA_IP_CH_SET_SOURCE_ADDRESS;
    DmaTransferList[0U].Value = (uint32)(&(Slave->DataBuffer[0U]));
    DmaTransferList[1U].Param = DMA_IP_CH_SET_SOURCE_SIGNED_OFFSET;
    DmaTransferList[2U].Param = DMA_IP_CH_SET_SOURCE_TRANSFER_SIZE;
    /* Destination configuration parameters */
    DmaTransferList[3U].Param = DMA_IP_CH_SET_DESTINATION_ADDRESS;
    DmaTransferList[4U].Param = DMA_IP_CH_SET_DESTINATION_SIGNED_OFFSET;
    DmaTransferList[4U].Value = 0U;
    DmaTransferList[5U].Param = DMA_IP_CH_SET_DESTINATION_TRANSFER_SIZE;
    /* Minor/Major Loop parameters */
    DmaTransferList[6U].Param = DMA_IP_CH_SET_MINORLOOP_SIZE;
    DmaTransferList[7U].Param = DMA_IP_CH_SET_MAJORLOOP_COUNT;
    /* Disable Hw auto request */
    DmaTransferList[8U].Param = DMA_IP_CH_SET_CONTROL_DIS_AUTO_REQUEST;
    DmaTransferList[8U].Value = 0U;
    /* Adjustment added to source and destination address parameter */
    DmaTransferList[9U].Param = DMA_IP_CH_SET_SOURCE_SIGNED_LAST_ADDR_ADJ;
    DmaTransferList[9U].Value = 0U;
    DmaTransferList[10U].Param = DMA_IP_CH_SET_DESTINATION_SIGNED_LAST_ADDR_ADJ;
    DmaTransferList[10U].Value = 4U; /* This ensures that last data byte is written into END register */
    /* Enable major interrupt parammeter */
    DmaTransferList[11U].Param = DMA_IP_CH_SET_CONTROL_EN_MAJOR_INTERRUPT;
    DmaTransferList[11U].Value = 0U;

    if (I3C_IP_TRANSFER_HALF_WORDS == Slave->TransferSize)
    {
        DmaCtrl |= I3C_SDMACTRL_DMAWIDTH(2U);
        DmaTransferList[1U].Value = 2U; /* SourceSignedOffset */
        DmaTransferList[2U].Value = DMA_IP_TRANSFER_SIZE_2_BYTE; /* SourceXferSize */
        DmaTransferList[3U].Value = (uint32)(&(Base->SWDATAH)); /* DestinationAddress */
        DmaTransferList[5U].Value = DMA_IP_TRANSFER_SIZE_2_BYTE;  /* DestinationXferSize */
        DmaTransferList[6U].Value = 2U; /* MinorLoopSize */
        DmaTransferList[7U].Value = (Slave->BufferSize >> 1U) - 1U; /* MajorLoopCount */
    }
    else
    {
        DmaCtrl |= I3C_SDMACTRL_DMAWIDTH(1U);
        DmaTransferList[1U].Value = 1U;  /* SourceSignedOffset */
        DmaTransferList[2U].Value = DMA_IP_TRANSFER_SIZE_1_BYTE; /* SourceXferSize */
        DmaTransferList[3U].Value = (uint32)(&(Base->SWDATAB)); /* DestinationAddress */
        DmaTransferList[5U].Value = DMA_IP_TRANSFER_SIZE_1_BYTE;  /* DestinationXferSize */
        DmaTransferList[6U].Value = 1U; /* MinorLoopSize */
        DmaTransferList[7U].Value = Slave->BufferSize - 1U; /* MajorLoopCount */
    }

    /* Clear error status before setting up DMA configuration */
    eDmaReturnStatus = Dma_Ip_SetLogicChannelCommand(Slave->DmaTxChannel, DMA_IP_CH_CLEAR_ERROR);
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(DMA_IP_STATUS_SUCCESS == eDmaReturnStatus);
#endif
    /* Configure DMA channel */
    eDmaReturnStatus = Dma_Ip_SetLogicChannelTransferList(Slave->DmaTxChannel, DmaTransferList, I3C_DMA_CHANNEL_CONFIG_LIST_SIZE);
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(DMA_IP_STATUS_SUCCESS == eDmaReturnStatus);
#endif
    /* Start DMA channel */
    eDmaReturnStatus = Dma_Ip_SetLogicChannelCommand(Slave->DmaTxChannel, DMA_IP_CH_SET_HARDWARE_REQUEST);
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(DMA_IP_STATUS_SUCCESS == eDmaReturnStatus);
#endif

    /* Set FIFO watermark */
    I3c_Ip_SlaveSetTxFifoWatermark(Base, I3C_IP_FIFO_TRIGGER_ON_ALMOST_FULL);

    /* Enable DMA */
    Base->SDMACTRL = DmaCtrl;
    (void)eDmaReturnStatus;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveRxDmaConfig
* Description   : This function configures DMA Rx channel for slave transfer.
*
* END**************************************************************************/
static void I3c_Ip_SlaveRxDmaConfig
(
    I3C_Type * const Base,
    const I3c_Ip_SlaveStateType * const Slave
)
{
    Dma_Ip_LogicChannelTransferListType DmaTransferList[I3C_DMA_CHANNEL_CONFIG_LIST_SIZE];
    Dma_Ip_ReturnType eDmaReturnStatus;
    uint32 DmaCtrl = I3C_SDMACTRL_DMAFB(2U);

    /* Source configuration parameters  */
    DmaTransferList[0U].Param = DMA_IP_CH_SET_SOURCE_ADDRESS;
    DmaTransferList[1U].Param = DMA_IP_CH_SET_SOURCE_SIGNED_OFFSET;
    DmaTransferList[1U].Value = 0U;
    DmaTransferList[2U].Param = DMA_IP_CH_SET_SOURCE_TRANSFER_SIZE;
    /* Destination configuration parameters  */
    DmaTransferList[3U].Param = DMA_IP_CH_SET_DESTINATION_ADDRESS;
    DmaTransferList[3U].Value = (uint32)(&(Slave->DataBuffer[0U]));
    DmaTransferList[4U].Param = DMA_IP_CH_SET_DESTINATION_SIGNED_OFFSET;
    DmaTransferList[5U].Param = DMA_IP_CH_SET_DESTINATION_TRANSFER_SIZE;
    /* Minor/Major Loop parameters */
    DmaTransferList[6U].Param = DMA_IP_CH_SET_MINORLOOP_SIZE;
    DmaTransferList[7U].Param = DMA_IP_CH_SET_MAJORLOOP_COUNT;
    /* Disable Hw auto request */
    DmaTransferList[8U].Param = DMA_IP_CH_SET_CONTROL_DIS_AUTO_REQUEST;
    DmaTransferList[8U].Value = 1U;
    /* Adjustment added to source and destination address parameter */
    DmaTransferList[9U].Param = DMA_IP_CH_SET_SOURCE_SIGNED_LAST_ADDR_ADJ;
    DmaTransferList[9U].Value = 0U;
    DmaTransferList[10U].Param = DMA_IP_CH_SET_DESTINATION_SIGNED_LAST_ADDR_ADJ;
    DmaTransferList[10U].Value = 0U;
    /* Enable major interrupt parammeter */
    DmaTransferList[11U].Param = DMA_IP_CH_SET_CONTROL_EN_MAJOR_INTERRUPT;
    DmaTransferList[11U].Value = 0U;

    /* Source and Destination configuration values */
    if (I3C_IP_TRANSFER_HALF_WORDS == Slave->TransferSize)
    {
        DmaCtrl |= I3C_SDMACTRL_DMAWIDTH(2U);
        DmaTransferList[0U].Value = (uint32)(&(Base->SRDATAH)); /* SourceAddress */
        DmaTransferList[2U].Value = DMA_IP_TRANSFER_SIZE_2_BYTE; /* SourceXferSize */
        DmaTransferList[4U].Value = 2U; /* DestinationSignedOffset */
        DmaTransferList[5U].Value = DMA_IP_TRANSFER_SIZE_2_BYTE;  /* DestinationXferSize */
        DmaTransferList[6U].Value = 2U; /* MinorLoopSize */
        DmaTransferList[7U].Value = (Slave->BufferSize >> 1U); /* MajorLoopCount */
    }
    else
    {
        DmaCtrl |= I3C_SDMACTRL_DMAWIDTH(1U);
        DmaTransferList[0U].Value = (uint32)(&(Base->SRDATAB)); /* SourceAddress */
        DmaTransferList[2U].Value = DMA_IP_TRANSFER_SIZE_1_BYTE; /* SourceXferSize */
        DmaTransferList[4U].Value = 1U; /* DestinationSignedOffset */
        DmaTransferList[5U].Value = DMA_IP_TRANSFER_SIZE_1_BYTE;  /* DestinationXferSize */
        DmaTransferList[6U].Value = 1U; /* MinorLoopSize */
        DmaTransferList[7U].Value = Slave->BufferSize; /* MajorLoopCount */
    }

    /* Clear error status before setting up DMA configuration */
    eDmaReturnStatus = Dma_Ip_SetLogicChannelCommand(Slave->DmaRxChannel, DMA_IP_CH_CLEAR_ERROR);
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(DMA_IP_STATUS_SUCCESS == eDmaReturnStatus);
#endif
    /* Configure DMA channel */
    eDmaReturnStatus = Dma_Ip_SetLogicChannelTransferList(Slave->DmaRxChannel, DmaTransferList, I3C_DMA_CHANNEL_CONFIG_LIST_SIZE);
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(DMA_IP_STATUS_SUCCESS == eDmaReturnStatus);
#endif
    /* Start DMA channel */
    eDmaReturnStatus = Dma_Ip_SetLogicChannelCommand(Slave->DmaRxChannel, DMA_IP_CH_SET_HARDWARE_REQUEST);
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(DMA_IP_STATUS_SUCCESS == eDmaReturnStatus);
#endif

    /* Set DMA Trigger for any data in FIFO */
    I3c_Ip_SlaveSetRxFifoWatermark(Base, I3C_IP_FIFO_TRIGGER_ON_EMPTY);

    /* Enable DMA */
    Base->SDMACTRL = DmaCtrl;
    (void)eDmaReturnStatus;
}
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterWaitForStatus
* Description   : This function waits until a status flag is set.
*
* END**************************************************************************/
static I3c_Ip_StatusType I3c_Ip_MasterWaitForStatus(const I3C_Type * const Base,
                                                    const uint32 Status)
{
    I3c_Ip_StatusType ReturnStatus = I3C_IP_STATUS_SUCCESS;
    uint32 TimeoutTicks = OsIf_MicrosToTicks(I3C_IP_TIMEOUT_VALUE, I3C_IP_TIMEOUT_TYPE);
    uint32 CurrentTicks = OsIf_GetCounter(I3C_IP_TIMEOUT_TYPE);
    uint32 ElapsedTicks = 0U;

    while ((I3c_Ip_MasterCheckStatus(Base, Status) == FALSE) && (ElapsedTicks < TimeoutTicks))
    {
        ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, I3C_IP_TIMEOUT_TYPE);
    }

    if (ElapsedTicks >= TimeoutTicks)
    {
        ReturnStatus = I3C_IP_STATUS_TIMEOUT;
    }

    return ReturnStatus;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterSendMessage
* Description   : Master writes data to be sent, either one byte or half word.
*
* END**************************************************************************/
static void I3c_Ip_MasterSendMessage(I3C_Type * const Base,
                                     I3c_Ip_MasterStateType * const Master)
{
    if (I3C_IP_TRANSFER_HALF_WORDS == Master->TransferOption.TransferSize)
    {
        I3c_Ip_MasterWriteHalfWordMessage(Base, Master);
    }
    else
    {
        I3c_Ip_MasterWriteByteMessage(Base, Master);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveSendMessage
* Description   : Slave writes data to be sent, either one byte or half word.
*
* END**************************************************************************/
static void I3c_Ip_SlaveSendMessage(I3C_Type * const Base,
                                    I3c_Ip_SlaveStateType * const Slave)
{
    if (I3C_IP_TRANSFER_HALF_WORDS == Slave->TransferSize)
    {
        I3c_Ip_SlaveWriteHalfWordMessage(Base, Slave);
    }
    else
    {
        I3c_Ip_SlaveWriteByteMessage(Base, Slave);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterReceiveByteMessage
* Description   : Master reads data from Rx FIFO, one byte for each FIFO access
*
* END**************************************************************************/
static void I3c_Ip_MasterReceiveByteMessage(const I3C_Type * const Base,
                                            I3c_Ip_MasterStateType * const Master)
{
    uint8 RxCount;

    RxCount = (uint8)((Base->MDATACTRL & I3C_MDATACTRL_RXCOUNT_MASK) >> I3C_MDATACTRL_RXCOUNT_SHIFT);

    while ((Master->BufferSize > 0U) && (RxCount > 0U))
    {
        Master->RxDataBuffer[0U] = I3c_Ip_ReadMRDATABRegister(Base);

        Master->RxDataBuffer = &(Master->RxDataBuffer[1U]);
        Master->BufferSize -= 1U;

        RxCount = (uint8)((Base->MDATACTRL & I3C_MDATACTRL_RXCOUNT_MASK) >> I3C_MDATACTRL_RXCOUNT_SHIFT);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterReceiveHalfWordMessage
* Description   : Master reads data from Rx FIFO, two bytes for each FIFO access.
*                 Reading half-words must be performed only if Rx FIFO contains at least 2 bytes of data.
*
* END**************************************************************************/
static void I3c_Ip_MasterReceiveHalfWordMessage(const I3C_Type * const Base,
                                                I3c_Ip_MasterStateType * const Master)
{
    uint8 RxCount;
    uint32 RegValue;

    RxCount = (uint8)((Base->MDATACTRL & I3C_MDATACTRL_RXCOUNT_MASK) >> I3C_MDATACTRL_RXCOUNT_SHIFT);

    while ((Master->BufferSize > 0U) && (RxCount >= 2U))
    {

        RegValue = Base->MRDATAH;
        Master->RxDataBuffer[0U] = I3c_Ip_GetMRDATAH_LSBValue(RegValue);
        Master->RxDataBuffer[1U] = I3c_Ip_GetMRDATAH_MSBValue(RegValue);

        Master->RxDataBuffer = &(Master->RxDataBuffer[2U]);
        Master->BufferSize -= 2U;

        RxCount = (uint8)((Base->MDATACTRL & I3C_MDATACTRL_RXCOUNT_MASK) >> I3C_MDATACTRL_RXCOUNT_SHIFT);
    }

    /* Read the last byte in the case when reading half-words, but the Rx Buffer size is an odd number of bytes */
    if ((1U == Master->BufferSize) && (RxCount > 0U))
    {
        Master->RxDataBuffer[0U] = I3c_Ip_ReadMRDATABRegister(Base);

        Master->RxDataBuffer = &(Master->RxDataBuffer[1U]);
        Master->BufferSize -= 1U;
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterReceiveMessage
* Description   : Master reads data from Rx FIFO, either one byte or half word
*
* END**************************************************************************/
static void I3c_Ip_MasterReceiveMessage(const I3C_Type * const Base,
                                        I3c_Ip_MasterStateType * const Master)
{
    if (I3C_IP_TRANSFER_BYTES == Master->TransferOption.TransferSize)
    {
       I3c_Ip_MasterReceiveByteMessage(Base, Master);
    }
    else
    {
        I3c_Ip_MasterReceiveHalfWordMessage(Base, Master);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveReceiveByteMessage
* Description   : Slave reads data from Rx FIFO, one byte for each FIFO access
*
* END**************************************************************************/
static void I3c_Ip_SlaveReceiveByteMessage(const I3C_Type * const Base,
                                           I3c_Ip_SlaveStateType * const Slave)
{
    uint8 RxCount;

    RxCount = (uint8)((Base->SDATACTRL & I3C_SDATACTRL_RXCOUNT_MASK) >> I3C_SDATACTRL_RXCOUNT_SHIFT);

    while ((Slave->BufferSize > 0U) && (RxCount > 0U))
    {
        Slave->DataBuffer[0U] = I3c_Ip_ReadSRDATABRegister(Base);

        Slave->DataBuffer = &(Slave->DataBuffer[1U]);
        Slave->BufferSize -= 1U;

        RxCount = (uint8)((Base->SDATACTRL & I3C_SDATACTRL_RXCOUNT_MASK) >> I3C_SDATACTRL_RXCOUNT_SHIFT);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveReceiveHalfWordMessage
* Description   : Slave reads data from Rx FIFO, two bytes for each FIFO access.
*                 Reading half-words must be performed only if Rx FIFO contains at least 2 bytes of data.
*
* END**************************************************************************/
static void I3c_Ip_SlaveReceiveHalfWordMessage(const I3C_Type * const Base,
                                               I3c_Ip_SlaveStateType * const Slave)
{
    uint8 RxCount;
    uint32 RegValue;

    RxCount = (uint8)((Base->SDATACTRL & I3C_SDATACTRL_RXCOUNT_MASK) >> I3C_SDATACTRL_RXCOUNT_SHIFT);

    while ((Slave->BufferSize > 0U) && (RxCount >= 2U))
    {
        RegValue = Base->SRDATAH;
        Slave->DataBuffer[0U] = I3c_Ip_GetSRDATAH_LSBValue(RegValue);
        Slave->DataBuffer[1U] = I3c_Ip_GetSRDATAH_MSBValue(RegValue);

        Slave->DataBuffer = &(Slave->DataBuffer[2U]);
        Slave->BufferSize -= 2U;

        RxCount = (uint8)((Base->SDATACTRL & I3C_SDATACTRL_RXCOUNT_MASK) >> I3C_SDATACTRL_RXCOUNT_SHIFT);
    }

    /* Read the last byte in the case when reading half-words, but the Rx Buffer size is an odd number of bytes */
    if ((1U == Slave->BufferSize) && (RxCount > 0U))
    {
        Slave->DataBuffer[0U] = I3c_Ip_ReadSRDATABRegister(Base);

        Slave->DataBuffer = &(Slave->DataBuffer[1U]);
        Slave->BufferSize -= 1U;
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveReceiveMessage
* Description   : Slave reads data from Rx FIFO, either one byte or half word.
*                 Reading half-words must be performed only if Rx FIFO contains at least 2 bytes of data.
*
* END**************************************************************************/
static void I3c_Ip_SlaveReceiveMessage(const I3C_Type * const Base,
                                       I3c_Ip_SlaveStateType * const Slave)
{
    if (I3C_IP_TRANSFER_BYTES == Slave->TransferSize)
    {
        I3c_Ip_SlaveReceiveByteMessage(Base, Slave);
    }
    else
    {
        I3c_Ip_SlaveReceiveHalfWordMessage(Base, Slave);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_EmitEndSignal
* Description   : Emit either STOP or Force Exit, depending on the current transfer mode.
*
* END**************************************************************************/
static void I3c_Ip_EmitEndSignal(I3C_Type * const Base)
{
    /* Emit a STOP on the bus if SDR and Force Exit if DDR */
    if (TRUE == I3c_Ip_MasterGetState(Base, (uint32)I3C_IP_MASTER_MODE_MSG_DDR))
    {
        /* Enter critical section */
        SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_02();

        /* ODSTOP must be 0 when sending HDR exit pattern */
        Base->MCONFIG = (Base->MCONFIG & (~I3C_MCONFIG_ODSTOP_MASK)) | I3C_MCONFIG_ODSTOP(0U);

        I3c_Ip_MasterEmitRequest(Base, I3C_IP_REQUEST_FORCE_EXIT);

        /* Exit critical section */
        SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_02();
    }
    else
    {
        /* Enter critical section */
        SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_02();

        I3c_Ip_MasterEmitRequest(Base, I3C_IP_REQUEST_EMIT_STOP);

        /* Exit critical section */
        SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_02();
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterEndTransfer
* Description   : This function ends the current transfer.
*
* END**************************************************************************/
static void I3c_Ip_MasterEndTransfer(I3C_Type * const Base,
                                     I3c_Ip_MasterStateType * const Master,
                                     boolean ResetFifos,
                                     boolean SendStop)
{
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Master != NULL_PTR);
    DevAssert(Base != NULL_PTR);
#endif

    /* Disable transfer related events */
    I3c_Ip_MasterDisableInterrupts(Base, ((uint32)I3C_MINTCLR_MCTRLDONE_MASK  |
                                                  I3C_MINTCLR_COMPLETE_MASK   |
                                                  I3C_MINTCLR_RXPEND_MASK));

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
    if (I3C_IP_USING_DMA == Master->TransferType)
    {
        /* Stop DMA Tx/Rx channel */
        if (Master->TransferOption.Direction == I3C_IP_WRITE)
        {
            (void)Dma_Ip_SetLogicChannelCommand(Master->DmaTxChannel, DMA_IP_CH_CLEAR_HARDWARE_REQUEST);
        }
        else
        {
            (void)Dma_Ip_SetLogicChannelCommand(Master->DmaRxChannel, DMA_IP_CH_CLEAR_HARDWARE_REQUEST);
        }
    }
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */

    if (TRUE == ResetFifos)
    {
        /* Enter critical section */
        SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_16();

        /* Flush master Tx and Rx FIFOs */
        I3c_Ip_MasterResetTxFIFO(Base);
        I3c_Ip_MasterResetRxFIFO(Base);

        /* Exit critical section */
        SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_16();
    }

    if (TRUE == SendStop)
    {
        I3c_Ip_EmitEndSignal(Base);

        Master->BufferSize = 0U;
        Master->TxDataBuffer = NULL_PTR;
        Master->RxDataBuffer = NULL_PTR;
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveEndTransfer
* Description   : This function ends the current transfer.
*
* END**************************************************************************/
static void I3c_Ip_SlaveEndTransfer(I3C_Type * const Base,
                                    I3c_Ip_SlaveStateType * const Slave,
                                    boolean ResetFifos)
{
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Slave != NULL_PTR);
    DevAssert(Base != NULL_PTR);
#endif

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
    if (I3C_IP_USING_DMA == Slave->TransferType)
    {
        /* Stop DMA Tx/Rx channel */
        if (Slave->Direction == I3C_IP_WRITE)
        {
            (void)Dma_Ip_SetLogicChannelCommand(Slave->DmaTxChannel, DMA_IP_CH_CLEAR_HARDWARE_REQUEST);
        }
        else
        {
            (void)Dma_Ip_SetLogicChannelCommand(Slave->DmaRxChannel, DMA_IP_CH_CLEAR_HARDWARE_REQUEST);
        }
    }
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */

    /* Disable all slave events */
    I3c_Ip_SlaveDisableInterrupts(Base, ((uint32)I3C_SINTCLR_START_MASK       |
                                                 I3C_SINTCLR_MATCHED_MASK     |
                                                 I3C_SINTCLR_STOP_MASK        |
                                                 I3C_SINTCLR_RXPEND_MASK      |
                                                 I3C_SINTCLR_TXSEND_MASK      |
                                                 I3C_SINTCLR_DACHG_MASK       |
                                                 I3C_SINTCLR_CCC_MASK         |
                                                 I3C_SINTCLR_ERRWARN_MASK     |
                                                 I3C_SINTCLR_DDRMATCHED_MASK  |
                                                 I3C_SINTCLR_CHANDLED_MASK    |
                                                 I3C_SINTCLR_EVENT_MASK));

    if (TRUE == ResetFifos)
    {
        /* Flush slave Tx and Rx FIFOs */
        I3c_Ip_SlaveResetTxFIFO(Base);
        I3c_Ip_SlaveResetRxFIFO(Base);
    }

    Slave->BufferSize = 0U;
    Slave->DataBuffer = NULL_PTR;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterTxNotFullEventHandler
* Description   : Handle Tx not full event.
*
* END**************************************************************************/
static void I3c_Ip_MasterTxNotFullEventHandler(I3C_Type * const Base,
                                               I3c_Ip_MasterStateType * const Master)
{
    /* Send data bytes */
    I3c_Ip_MasterSendMessage(Base, Master);

    if (0U == Master->BufferSize)
    {
        I3c_Ip_MasterDisableInterrupts(Base, (uint32)I3C_MINTCLR_TXNOTFULL_MASK);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveTxSendEventHandler
* Description   : Handle Slave Tx send event.
*
* END**************************************************************************/
static void I3c_Ip_SlaveTxSendEventHandler(I3C_Type * const Base,
                                           I3c_Ip_SlaveStateType * const Slave)
{
    if (0U == Slave->BufferSize)
    {
        /* Tx buffer is empty, call callback to allow user to provide a new buffer */
        if (Slave->SlaveCallback != NULL_PTR)
        {
            Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_TX_EMPTY);
        }
    }

    /* Send data */
    I3c_Ip_SlaveSendMessage(Base, Slave);

    if (0U == Slave->BufferSize)
    {
        /* This is the case when the module is in HDR-DDR mode and will remain in this mode
           for the upcoming transfers. Thus, no Repeated START will be detected. This indicates
           the end of the transfer. */
        if (TRUE == I3c_Ip_SlaveCheckStatus(Base, (uint32)I3C_SSTATUS_STHDR_MASK))
        {
            if (I3C_IP_STATUS_BUSY == Slave->Status)
            {
                Slave->Status = I3C_IP_STATUS_SUCCESS;
            }
        }

        /* Disable Tx send event */
        I3c_Ip_SlaveDisableInterrupts(Base, (uint32)I3C_SINTCLR_TXSEND_MASK);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterConvertRxSizeToWatermark
* Description   : This function returns the currently configured value for the Rx FIFO watermark.
*
* END**************************************************************************/
static I3c_Ip_FifoTriggerLevelType I3c_Ip_MasterConvertRxSizeToWatermark(const I3c_Ip_MasterStateType * const Master)
{
    I3c_Ip_FifoTriggerLevelType ReturnValue;

    /* I3C_IP_FIFO_TRIGGER_ON_ALMOST_FULL should not be used because for higher SCL frequencies might lead to Master Rx Overrun.*/
    if (I3C_IP_BUS_TYPE_I3C_DDR == Master->TransferOption.BusType)
    {
        if (Master->BufferSize >= (I3C_IP_RX_FIFO_SIZE / 4U))
        {
            ReturnValue = I3C_IP_FIFO_TRIGGER_ON_ONE_QUARTER;
        }
        else
        {
            ReturnValue = I3C_IP_FIFO_TRIGGER_ON_EMPTY;
        }
    }
    else
    {
        if (Master->BufferSize >= (I3C_IP_RX_FIFO_SIZE / 2U))
        {
            ReturnValue = I3C_IP_FIFO_TRIGGER_ON_ONE_HALF;
        }
        else if (Master->BufferSize >= (I3C_IP_RX_FIFO_SIZE / 4U))
        {
            ReturnValue = I3C_IP_FIFO_TRIGGER_ON_ONE_QUARTER;
        }
        else
        {
            ReturnValue = I3C_IP_FIFO_TRIGGER_ON_EMPTY;
        }
    }

    return ReturnValue;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveConvertRxSizeToWatermark
* Description   : This function returns the value for the Rx FIFO watermark that should be set.
*
* END**************************************************************************/
static I3c_Ip_FifoTriggerLevelType I3c_Ip_SlaveConvertRxSizeToWatermark(const I3C_Type * const Base,
                                                                        uint32 BufferSize)
{
    I3c_Ip_FifoTriggerLevelType ReturnValue;

    /* I3C_IP_FIFO_TRIGGER_ON_ALMOST_FULL should not be used because for higher SCL frequencies might lead to Slave Rx Overrun.*/
    if (TRUE == I3c_Ip_SlaveCheckStatus(Base, (uint32)I3C_SSTATUS_STHDR_MASK))
    {
        if (BufferSize >= (I3C_IP_RX_FIFO_SIZE / 4U))
        {
            ReturnValue = I3C_IP_FIFO_TRIGGER_ON_ONE_QUARTER;
        }
        else
        {
            ReturnValue = I3C_IP_FIFO_TRIGGER_ON_EMPTY;
        }
    }
    else
    {
        if (BufferSize >= (I3C_IP_RX_FIFO_SIZE / 2U))
        {
            ReturnValue = I3C_IP_FIFO_TRIGGER_ON_ONE_HALF;
        }
        else if (BufferSize >= (I3C_IP_RX_FIFO_SIZE / 4U))
        {
            ReturnValue = I3C_IP_FIFO_TRIGGER_ON_ONE_QUARTER;
        }
        else
        {
            ReturnValue = I3C_IP_FIFO_TRIGGER_ON_EMPTY;
        }
    }

    return ReturnValue;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterRxPendingEventHandler
* Description   : Handle Rx pending event.
*
* END**************************************************************************/
static void I3c_Ip_MasterRxPendingEventHandler(I3C_Type * const Base,
                                               I3c_Ip_MasterStateType * const Master)
{
    I3c_Ip_FifoTriggerLevelType RxFifoWatermark;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Master->RxDataBuffer != NULL_PTR);
#endif

    /* Get the receiving data into the Rx DataBuffer */
    I3c_Ip_MasterReceiveMessage(Base, Master);

    /* Update Rx trigger level */
    RxFifoWatermark = I3c_Ip_MasterConvertRxSizeToWatermark(Master);
    I3c_Ip_MasterSetRxFifoWatermark(Base, RxFifoWatermark);
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveRxPendingEventHandler
* Description   : Handle Rx pending event.
*
* END**************************************************************************/
static void I3c_Ip_SlaveRxPendingEventHandler(I3C_Type * const Base,
                                              I3c_Ip_SlaveStateType * const Slave)
{
    I3c_Ip_FifoTriggerLevelType RxFifoWatermark;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Slave->DataBuffer != NULL_PTR);
#endif

    /* Get the receiving data into the Rx DataBuffer */
    I3c_Ip_SlaveReceiveMessage(Base, Slave);

    if (0U == Slave->BufferSize)
    {
        /* This is the case when the module is in HDR-DDR mode and will remain in this mode
           for the upcoming transfers. Thus, no Repeated START will be detected. This indicates
           the end of the transfer. */
        if (TRUE == I3c_Ip_SlaveCheckStatus(Base, (uint32)I3C_SSTATUS_STHDR_MASK))
        {
            if (I3C_IP_STATUS_BUSY == Slave->Status)
            {
                Slave->Status = I3C_IP_STATUS_SUCCESS;
            }
        }

        /* Disable Rx pending event */
        I3c_Ip_SlaveDisableInterrupts(Base, (uint32)I3C_SINTCLR_RXPEND_MASK);
    }
    else
    {
        /* Set Rx watermark */
        RxFifoWatermark = I3c_Ip_SlaveConvertRxSizeToWatermark(Base, Slave->BufferSize);
        I3c_Ip_SlaveSetRxFifoWatermark(Base, RxFifoWatermark);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveCCCHandledEventHandler
* Description   : Handle Common Command Code handled event.
*
* END**************************************************************************/
static void I3c_Ip_SlaveCCCHandledEventHandler(const I3c_Ip_SlaveStateType * const Slave)
{
    if (Slave->SlaveCallback != NULL_PTR)
    {
        Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_CCC_HANDLED);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveStartEventHandler
* Description   : Handle START event. This is not usually needed, but can be used for wake events.
*
* END**************************************************************************/
static void I3c_Ip_SlaveStartEventHandler(I3c_Ip_SlaveStateType * const Slave)
{
    if (Slave->SlaveCallback != NULL_PTR)
    {
        Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_START);
    }

    /* This is the case with Repeated Start between the transfers.
       Usually, when STOP is detected, Slave->Status is updated to I3C_IP_STATUS_SUCCESS if no error occurred.
       If Repeated Start is used between the transfers, Slave->Status shall be also updated. */
    if (Slave->RepeatedStarts > 0U)
    {
        Slave->Status = I3C_IP_STATUS_SUCCESS;
    }

    Slave->RepeatedStarts++;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveStopEventHandler
* Description   : Handle STOP event.
*
* END**************************************************************************/
static void I3c_Ip_SlaveStopEventHandler(I3c_Ip_SlaveStateType * const Slave)
{
    /* If no error occurred during the transfer, when STOP signal is detected
       Slave->Status should be updated to I3C_IP_STATUS_SUCCESS. */
    if (I3C_IP_STATUS_BUSY == Slave->Status)
    {
        Slave->Status = I3C_IP_STATUS_SUCCESS;
    }

    Slave->RepeatedStarts = 0U;

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
    if (I3C_IP_USING_DMA == Slave->TransferType)
    {
        /* Stop DMA Tx/Rx channel */
        if (Slave->Direction == I3C_IP_WRITE)
        {
            (void)Dma_Ip_SetLogicChannelCommand(Slave->DmaTxChannel, DMA_IP_CH_CLEAR_HARDWARE_REQUEST);
        }
        else
        {
            (void)Dma_Ip_SetLogicChannelCommand(Slave->DmaRxChannel, DMA_IP_CH_CLEAR_HARDWARE_REQUEST);
        }
    }
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */

    if (Slave->SlaveCallback != NULL_PTR)
    {
        Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_STOP);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveDAChangeEventHandler
* Description   : Handle Dynamic Address change event.
*
* END**************************************************************************/
static void I3c_Ip_SlaveDAChangeEventHandler(const I3c_Ip_SlaveStateType * const Slave)
{
    if (Slave->SlaveCallback != NULL_PTR)
    {
        Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_DA_CHANGED);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveDDRMatchedEventHandler
* Description   : Handle HDR-DDR command matched event.
*
* END**************************************************************************/
static void I3c_Ip_SlaveDDRMatchedEventHandler(I3C_Type * const Base,
                                               I3c_Ip_SlaveStateType * const Slave)
{
    uint8 HdrDdrCmdCode;

    if (Slave->SlaveCallback != NULL_PTR)
    {
        Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_HDR_MATCHED);
    }

    /* Current implementation implies the HDR-DDR command code is written into MWDATAB Tx FIFO,
       both for write and read operations, regardless of transfer type: in bytes or in half-words. */
    HdrDdrCmdCode = I3c_Ip_ReadSRDATABRegister(Base);

    /* If this value is 0, HDR-DDR message is a write to this slave. If the value is 1, the message is a read from the slave. */
    if (0U == ((HdrDdrCmdCode & I3C_IP_SLAVE_HDR_DDR_DIRECTION_MASK) >> I3C_IP_SLAVE_HDR_DDR_DIRECTION_SHIFT))
    {
        /* Slave reads the incoming message */
        Slave->Direction = I3C_IP_READ;
        Slave->Status = I3C_IP_STATUS_BUSY;

        /* This callback allows the user to set the buffer */
        if (Slave->SlaveCallback != NULL_PTR)
        {
            Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_RX_REQUEST);
        }

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
        if (I3C_IP_USING_DMA == Slave->TransferType)
        {
            /* Start slave DMA transfer */
            I3c_Ip_SlaveRxDmaConfig(Base, Slave);
        }
        else
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */
        {
            /* Enable Rx pending event*/
            I3c_Ip_SlaveEnableInterrupts(Base, (uint32)I3C_SINTSET_RXPEND_MASK);
        }
    }
    else
    {
        if (Slave->SlaveCallback != NULL_PTR)
        {
            Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_TX_REQUEST);
        }
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveHeaderMatchedEventHandler
* Description   : Handle header matched event.
*
* END**************************************************************************/
static void I3c_Ip_SlaveHeaderMatchedEventHandler(I3C_Type * const Base,
                                                  I3c_Ip_SlaveStateType * const Slave)
{
    uint32 CurrentTicks = OsIf_GetCounter(I3C_IP_TIMEOUT_TYPE);
    uint32 ElapsedTicks = 0U;
    uint32 TimeoutTicks = OsIf_MicrosToTicks(I3C_IP_TIMEOUT_VALUE, I3C_IP_TIMEOUT_TYPE);

    /* Callback for header matched */
    if (Slave->SlaveCallback != NULL_PTR)
    {
        Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_HEADER_MATCHED);
    }

    /* Wait for the direction status flag to be set accordingly. */
    while (((Base->SSTATUS & (uint32)I3C_SSTATUS_STREQWR_MASK) == 0U) &&
           ((Base->SSTATUS & (uint32)I3C_SSTATUS_STREQRD_MASK) == 0U) &&
           (ElapsedTicks < TimeoutTicks))
    {
        ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, I3C_IP_TIMEOUT_TYPE);
    }

    /* Check if the matched header is requesting a write to this slave */
    if (TRUE == I3c_Ip_SlaveCheckStatus(Base, (uint32)I3C_SSTATUS_STREQWR_MASK))
    {
        Slave->Direction = I3C_IP_READ;
        Slave->Status = I3C_IP_STATUS_BUSY;

        /* This callback allows the user to set the buffer */
        if (Slave->SlaveCallback != NULL_PTR)
        {
            Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_RX_REQUEST);
        }

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
        if (I3C_IP_USING_DMA == Slave->TransferType)
        {
            /* Start slave DMA transfer */
            I3c_Ip_SlaveRxDmaConfig(Base, Slave);
        }
        else
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */
        {
            /* Enable Rx pending event*/
            I3c_Ip_SlaveEnableInterrupts(Base, (uint32)I3C_SINTSET_RXPEND_MASK);
        }
    }
    else if (TRUE == I3c_Ip_SlaveCheckStatus(Base, (uint32)I3C_SSTATUS_STREQRD_MASK))
    {
        if (Slave->SlaveCallback != NULL_PTR)
        {
            Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_TX_REQUEST);
        }
    }
    else
    {
        /* Do nothing */
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveEventsHandler
* Description   : Handle slave events (IBI/HJ/MR).
*
* END**************************************************************************/
static void I3c_Ip_SlaveEventsHandler(const I3c_Ip_SlaveStateType * const Slave)
{
    if (Slave->SlaveCallback != NULL_PTR)
    {
        Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_REQUEST);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveCCCEventHandler
* Description   : Handle Common Command Code not handled event.
*
* END**************************************************************************/
static void I3c_Ip_SlaveCCCEventHandler(const I3c_Ip_SlaveStateType * const Slave)
{
    if (Slave->SlaveCallback != NULL_PTR)
    {
        Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_CCC_RECEIVED);
    }
}

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterProcessCtrlDoneHJ
* Description   : This function is used to emit STOP after IbiAckNack decision
*                 was done for Hot-Join request.
*
* END**************************************************************************/
static void I3c_Ip_MasterProcessCtrlDoneHJ(I3C_Type * const Base,
                                           I3c_Ip_MasterStateType * const Master)
{
    /* After Hot-Join, STOP shall be emitted for any manual decision because
       the next action could be a Dynamic Address Asignment, which shall be done
       only when the I3c bus is idle. */
    I3c_Ip_MasterEmitStop(Base, Master->IbiOptions.IbiSendStop);

    Master->Status = I3C_IP_STATUS_SUCCESS;
    I3c_Ip_MasterDisableInterrupts(Base, (uint32)I3C_MINTCLR_MCTRLDONE_MASK);
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterProcessCtrlDoneIBI
* Description   : This function is used to emit STOP after IbiAckNack decision
*                 was done for IBI request.
*
* END**************************************************************************/
static void I3c_Ip_MasterProcessCtrlDoneIBI(I3C_Type * const Base,
                                            I3c_Ip_MasterStateType * const Master)
{
    uint8 IbiResp;
    IbiResp = (uint8)((Base->MCTRL & I3C_MCTRL_IBIRESP_MASK) >> I3C_MCTRL_IBIRESP_SHIFT);

    if ((uint8)I3C_IP_IBI_RESP_NACK == IbiResp)
    {
        I3c_Ip_MasterEmitStop(Base, Master->IbiOptions.IbiSendStop);
        Master->Status = I3C_IP_STATUS_SUCCESS;
    }
    else
    {
        I3c_Ip_MasterEnableInterrupts(Base, I3C_MINTSET_COMPLETE_MASK);
    }

    I3c_Ip_MasterDisableInterrupts(Base, (uint32)I3C_MINTCLR_MCTRLDONE_MASK);
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterProcessCtrlDoneMR
* Description   : This function is used to emit STOP after IbiAckNack decision
*                 was done for IBI request.
*
* END**************************************************************************/
static void I3c_Ip_MasterProcessCtrlDoneMR(I3C_Type * const Base,
                                           I3c_Ip_MasterStateType * const Master)
{
    I3c_Ip_MasterEmitStop(Base, Master->IbiOptions.IbiSendStop);

    Master->Status = I3C_IP_STATUS_SUCCESS;
    I3c_Ip_MasterDisableInterrupts(Base, (uint32)I3C_MINTCLR_MCTRLDONE_MASK);
}
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterControlDoneEventHandler
* Description   : Handle control done event.
*
* END**************************************************************************/
static void I3c_Ip_MasterControlDoneEventHandler(I3C_Type * const Base,
                                                 I3c_Ip_MasterStateType * const Master)
{
#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
    uint8 Event;
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

    if (Master->MasterCallback != NULL_PTR)
    {
        Master->MasterCallback(I3C_IP_MASTER_EVENT_CONTROL_DONE);
    }

    if (I3C_IP_STATUS_CONTROL_DONE == Master->Status)
    {
        if (I3C_IP_BUS_TYPE_I3C_DDR == Master->TransferOption.BusType)
        {
            Base->MCONFIG = (Base->MCONFIG & (~I3C_MCONFIG_ODSTOP_MASK)) | I3C_MCONFIG_ODSTOP((Master->OpenDrainStop) ? 1U : 0U);
        }
        Master->Status = I3C_IP_STATUS_SUCCESS;
        I3c_Ip_MasterDisableInterrupts(Base, (uint32)I3C_MINTCLR_MCTRLDONE_MASK);
    }

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
    Event = (uint8)((Base->MSTATUS & (uint32)I3C_MSTATUS_IBITYPE_MASK) >> I3C_MSTATUS_IBITYPE_SHIFT);

    switch (Event)
    {
        case (uint8)(I3C_IP_SLAVE_IBI_REQUEST):
        {
            I3c_Ip_MasterProcessCtrlDoneIBI(Base, Master);
            break;
        }
        case (uint8)(I3C_IP_SLAVE_HOT_JOIN_REQUEST):
        {
            I3c_Ip_MasterProcessCtrlDoneHJ(Base, Master);
            break;
        }
        case (uint8)(I3C_IP_SLAVE_MASTER_REQUEST):
        {
            I3c_Ip_MasterProcessCtrlDoneMR(Base, Master);
            break;
        }
        default:
        {
            /* Do nothing */
            break;
        }
    }
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

    (void)Base;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterCompleteEventHandler
* Description   : Handle complete event.
*
* END**************************************************************************/
static void I3c_Ip_MasterCompleteEventHandler(I3C_Type * const Base,
                                              I3c_Ip_MasterStateType * const Master)
{
#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
    uint8 RxCount;
    uint8 TempCount;
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

    boolean SendStop;

    if (Master->MasterCallback != NULL_PTR)
    {
        Master->MasterCallback(I3C_IP_MASTER_EVENT_COMPLETE);
    }

    SendStop = Master->TransferOption.SendStop;

    /* For read operation, complete event fires after MCTRL[RDTERM] number of bytes.
       However, this could happen before the last byte was received in the Master Rx FIFO.
       Thus, if any remaining receiving data, read the last byte. */
    if (I3C_IP_READ == Master->TransferOption.Direction)
    {
        if (Master->BufferSize > 0U)
        {
            /* Read data from Rx FIFO */
            I3c_Ip_MasterReceiveMessage(Base, Master);
        }
    }

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
    if (I3C_IP_STATUS_IBI_WON == Master->Status)
    {
        TempCount = 0U;

        /* Get the number of data bytes sent by the slave with the IBI */
        RxCount = (uint8)((Base->MDATACTRL & I3C_MDATACTRL_RXCOUNT_MASK) >> I3C_MDATACTRL_RXCOUNT_SHIFT);

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
        if (RxCount > 0U)
        {
            DevAssert(Master->IbiOptions.IbiDataBuffer != NULL_PTR);
        }
#endif /* (STD_ON == I3C_IP_DEV_ERROR_DETECT) */

        /* There are maximum I3C_IP_IBI_DATA_MAX_SIZE bytes sent with the IBI, if any data at all */
        while ((TempCount < I3C_IP_IBI_DATA_MAX_SIZE) && (RxCount > 0U))
        {
            Master->IbiOptions.IbiDataBuffer[TempCount] = I3c_Ip_ReadMRDATABRegister(Base);
            TempCount++;
            RxCount = (uint8)((Base->MDATACTRL & I3C_MDATACTRL_RXCOUNT_MASK) >> I3C_MDATACTRL_RXCOUNT_SHIFT);
        }

        SendStop = Master->IbiOptions.IbiSendStop;
    }
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

    I3c_Ip_MasterEndTransfer(Base, Master, FALSE, SendStop);

    if (TRUE == SendStop)
    {
        I3c_Ip_MasterEnableInterrupts(Base, I3C_MINTSET_MCTRLDONE_MASK);
        Master->Status = I3C_IP_STATUS_CONTROL_DONE;
    }
    else
    {
        Master->Status = I3C_IP_STATUS_SUCCESS;
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterSlaveStartEventHandler
* Description   : Handle slave start request.
*
* END**************************************************************************/
static void I3c_Ip_MasterSlaveStartEventHandler(I3C_Type * const Base,
                                                const I3c_Ip_MasterStateType * const Master)
{
    if (TRUE == I3c_Ip_MasterGetState(Base, (uint32)I3C_IP_MASTER_MODE_SLAVE_REQUEST))
    {
        if (Master->MasterCallback != NULL_PTR)
        {
            Master->MasterCallback(I3C_IP_MASTER_EVENT_SLAVE_START);
        }

        /* Clear transfer related flags and disable corresponding events */
        I3c_Ip_MasterClearStatus(Base, (uint32)I3C_MSTATUS_COMPLETE_MASK);
        I3c_Ip_MasterDisableInterrupts(Base, ((uint32)I3C_MINTCLR_MCTRLDONE_MASK  |
                                                      I3C_MINTCLR_COMPLETE_MASK   |
                                                      I3C_MINTCLR_RXPEND_MASK     |
                                                      I3C_MINTCLR_TXNOTFULL_MASK));

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
        if (TRUE == Master->IbiOptions.EmitAutoIbi)
        {
            /* For HJ and MR, Master->IbiResp is MANUAL, regardless of its current value. */
            Base->MCTRL = (Base->MCTRL & (~(I3C_MCTRL_REQUEST_MASK              |
                                            I3C_MCTRL_IBIRESP_MASK)))           |
                                            I3C_MCTRL_IBIRESP(Master->IbiResp)  |
                                            I3C_MCTRL_REQUEST(I3C_IP_REQUEST_AUTO_IBI);
        }
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */
    }
}

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterProcessHotJoin
* Description   : Process Hot-Join Request.
*
* END**************************************************************************/
static void I3c_Ip_MasterProcessHotJoin(I3C_Type * const Base,
                                        const I3c_Ip_MasterStateType * const Master)
{
    /* In case of a Hot-Join, In-Band Interrupt Response is Manual, regardless of current value in MCTRL[IBIRESP].
       Thus, a manual decision should be taken using I3c_Ip_MasterManualIbiResponse() function in I3C_IP_MASTER_EVENT_HOT_JOIN callback. */
    if (Master->MasterCallback != NULL_PTR)
    {
        Master->MasterCallback(I3C_IP_MASTER_EVENT_HOT_JOIN);
    }

    /* Enable control done interrupt to know when the IbiAckNack request was sent using the I3C_IP_MASTER_EVENT_HOT_JOIN callback. */
    I3c_Ip_MasterEnableInterrupts(Base, I3C_MINTSET_MCTRLDONE_MASK);
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterProcessIbi
* Description   : Process an In-Band Interrupt Request.
*
* END**************************************************************************/
static void I3c_Ip_MasterProcessIbi(I3C_Type * const Base,
                                    I3c_Ip_MasterStateType * const Master)
{
    uint8 IbiAddress;
    IbiAddress = (uint8)((Base->MSTATUS & I3C_MSTATUS_IBIADDR_MASK) >> I3C_MSTATUS_IBIADDR_SHIFT);

    /* This callback is used by the user to get the address of the slave
       which requested an In-Band interrupt. This callback is used only if the
       In-Band interrupt won the header arbitration. If the In-Band interrupt requires
       manual intervention, based on the slave's address, the user can use MasterCallback for I3C_IP_MASTER_EVENT_IBI event
       and the I3c_Ip_MasterManualIbiResponse() function to make the ACK/NACK decision. If no manual intervention is required,
       this callback can be used so that the user is aware of the address of the slave which requested the IBI. */
    if (Master->MasterGetIbiAddressCallback != NULL_PTR)
    {
        Master->MasterGetIbiAddressCallback(I3C_IP_MASTER_EVENT_IBI, IbiAddress);
    }

    /* If master NACKs an In-Band interrupt, no processing action is needed.
       However, if In-Band interrupt response is set to Manual in MCTRL[IBIRESP],
       a decision should be be taken using I3c_Ip_MasterManualIbiResponse() function in I3C_IP_MASTER_EVENT_IBI callback.
       If In-Band interrupt response is set to ACK or ACK with MDB, the callback is not used.
       Finally, IBI data bytes are read (if any) when complete event fires. */
    if (I3C_IP_IBI_RESP_NACK == Master->IbiResp)
    {
        I3c_Ip_MasterEmitStop(Base, Master->IbiOptions.IbiSendStop);
        Master->Status = I3C_IP_STATUS_SUCCESS;
    }
    else
    {
        if (I3C_IP_IBI_RESP_MANUAL == Master->IbiResp)
        {
            /* When IBI response is I3C_IP_IBI_RESP_MANUAL, a decision must be taken using I3c_Ip_MasterManualIbiResponse() function in
               I3C_IP_MASTER_EVENT_IBI callback. */
            if (Master->MasterCallback != NULL_PTR)
            {
                Master->MasterCallback(I3C_IP_MASTER_EVENT_IBI);
            }

            /* Enable control done interrupt to know when the IbiAckNack request was sent using the I3C_IP_MASTER_EVENT_IBI callback. */
            I3c_Ip_MasterEnableInterrupts(Base, I3C_MINTSET_MCTRLDONE_MASK);
        }
        else
        {
            /* Enable complete event to fire when all IBI data bytes have been sent, if any.
               All IBI data bytes will be read when complete interrupt fires and Master->Status is I3C_IP_STATUS_IBI_WON.
               For manual intervention, complete event is enabled using control done interrupt handler,
               after IbiAckNack request completed. */
            I3c_Ip_MasterEnableInterrupts(Base, I3C_MINTSET_COMPLETE_MASK);
        }
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterProcessMasterRequest
* Description   : Process a Master-Request.
*
* END**************************************************************************/
static void I3c_Ip_MasterProcessMasterRequest(I3C_Type * const Base,
                                              const I3c_Ip_MasterStateType * const Master)
{
    uint8 IbiAddress;
    IbiAddress = (uint8)((Base->MSTATUS & I3C_MSTATUS_IBIADDR_MASK) >> I3C_MSTATUS_IBIADDR_SHIFT);

    /* This callback is used by the user to get the address of the slave which requested the mastership.
       This callback is used only if the In-Band interrupt won the header arbitration.
       Based on the slave's address, the user can use MasterCallback for I3C_IP_MASTER_EVENT_MASTER_REQUEST event
       and the I3c_Ip_MasterManualIbiResponse() function to make the ACK/NACK decision. */
    if (Master->MasterGetIbiAddressCallback != NULL_PTR)
    {
        Master->MasterGetIbiAddressCallback(I3C_IP_MASTER_EVENT_MASTER_REQUEST, IbiAddress);
    }

    /* In case of a Master-Request, In-Band Interrupt Response is Manual, regardless of current value in MCTRL[IBIRESP].
       Thus, a manual decision should be taken using I3c_Ip_MasterManualIbiResponse() function in I3C_IP_MASTER_EVENT_MASTER_REQUEST callback. */
    if (Master->MasterCallback != NULL_PTR)
    {
        Master->MasterCallback(I3C_IP_MASTER_EVENT_MASTER_REQUEST);
    }

    /* Enable control done interrupt to know when the IbiAckNack request was sent using the I3C_MINTSET_MCTRLDONE_MASK callback. */
    I3c_Ip_MasterEnableInterrupts(Base, I3C_MINTSET_MCTRLDONE_MASK);
}
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterArbitrationLostEventHandler
* Description   : Handle slave won arbitration event.
*
* END**************************************************************************/
static void I3c_Ip_MasterArbitrationLostEventHandler(const uint8 Instance,
                                                     I3c_Ip_MasterStateType * const Master)
{
#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
    uint8 Event;
    I3C_Type * Base = I3c_Ip_apxBase[Instance];
#else
    const I3C_Type * Base = I3c_Ip_apxBase[Instance];
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

    if (Master->MasterCallback != NULL_PTR)
    {
        Master->MasterCallback(I3C_IP_MASTER_EVENT_ARBITRATION_LOST);
    }

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)

    Event = (uint8)((Base->MSTATUS & (uint32)I3C_MSTATUS_IBITYPE_MASK) >> I3C_MSTATUS_IBITYPE_SHIFT);

    switch (Event)
    {
        case (uint8)(I3C_IP_SLAVE_IBI_REQUEST):
        {
            Master->Status = I3C_IP_STATUS_IBI_WON;
            I3c_Ip_MasterProcessIbi(Base, Master);
            break;
        }
        case (uint8)(I3C_IP_SLAVE_HOT_JOIN_REQUEST):
        {
            I3c_Ip_MasterProcessHotJoin(Base, Master);
            break;
        }
        case (uint8)(I3C_IP_SLAVE_MASTER_REQUEST):
        {
            I3c_Ip_MasterProcessMasterRequest(Base, Master);
            break;
        }
        default:
        {
            /* Do nothing */
            break;
        }
    }
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */
    (void)Base;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterNowMasterEventHandler
* Description   : Handle now master event.
*
* END**************************************************************************/
static void I3c_Ip_MasterNowMasterEventHandler(const uint8 Instance,
                                               const I3c_Ip_MasterStateType * const Master)
{
    I3C_Type * Base;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif

    Base = I3c_Ip_apxBase[Instance];

#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
    /* This interrupt fires multiple times. This check is done to apply the changes only once. */
    if (I3C_IP_MASTER_CAPABLE_MODE == I3c_Ip_aeChannelCurrentMode[Instance])
    {
        I3c_Ip_aeChannelCurrentMode[Instance] = I3C_IP_MASTER_MODE;

        /* Enable the module as master and disable its slave functionality */
        I3c_Ip_MasterEnable(Base, I3C_IP_MASTER_ON);
        I3c_Ip_SlaveEnable(Base, FALSE);
    }
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES) */

    if (Master->MasterCallback != NULL_PTR)
    {
        Master->MasterCallback(I3C_IP_MASTER_EVENT_NOW_MASTER);
    }

    (void)Base;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterErrorEventHandler
* Description   : Handle error event.
*
* END**************************************************************************/
static void I3c_Ip_MasterErrorEventHandler(I3C_Type * const Base,
                                           I3c_Ip_MasterStateType * const Master)
{
    /* Disable slave request events */
    I3c_Ip_MasterDisableInterrupts(Base, ((uint32)I3C_MINTCLR_SLVSTART_MASK   |
                                                  I3C_MINTCLR_IBIWON_MASK     |
                                                  I3C_MINTCLR_ERRWARN_MASK    |
                                                  I3C_MINTCLR_NOWMASTER_MASK));

    I3c_Ip_MasterEndTransfer(Base, Master, TRUE, TRUE);
    Master->Status = I3C_IP_STATUS_ERROR;

    if (Master->MasterCallback != NULL_PTR)
    {
        Master->MasterCallback(I3C_IP_MASTER_EVENT_ERROR);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveErrorEventHandler
* Description   : Handle error event.
*
* END**************************************************************************/
static void I3c_Ip_SlaveErrorEventHandler(I3C_Type * const Base,
                                          I3c_Ip_SlaveStateType * const Slave)
{
    /* End the transfer */
    I3c_Ip_SlaveEndTransfer(Base, Slave, TRUE);
    Slave->Status = I3C_IP_STATUS_ERROR;

    if (Slave->SlaveCallback != NULL_PTR)
    {
        Slave->SlaveCallback(I3C_IP_SLAVE_EVENT_ERROR);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterCheckFifoEvents
* Description   : This function checks if a FIFO event occurred, and calls its handler if so.
*                 FIFO event means either Tx not full or Rx pending.
*
* END**************************************************************************/
static void I3c_Ip_MasterCheckFifoEvents(I3C_Type * const Base,
                                         I3c_Ip_MasterStateType * const Master)
{
    /* Check if the interrupt was spurious and call its handler */
    if (FALSE == I3c_Ip_MasterCheckSpuriousInt(Base, (uint32)I3C_MINTMASKED_TXNOTFULL_MASK))
    {
        /* Handle Tx not full event */
        I3c_Ip_MasterTxNotFullEventHandler(Base, Master);
    }

     /* Check if the interrupt was spurious and call its handler */
    if (FALSE == I3c_Ip_MasterCheckSpuriousInt(Base, (uint32)I3C_MINTMASKED_RXPEND_MASK))
    {
        /* Handle Rx ready event */
        I3c_Ip_MasterRxPendingEventHandler(Base, Master);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveCheckFifoEvents
* Description   : This function checks if a FIFO event occurred, and calls its handler if so.
*                 FIFO event means either Tx send or Rx pending.
*
* END**************************************************************************/
static void I3c_Ip_SlaveCheckFifoEvents(I3C_Type * const Base,
                                        I3c_Ip_SlaveStateType * const Slave)
{
    /* Check if the interrupt was spurious and call its handler */
    if (FALSE == I3c_Ip_SlaveCheckSpuriousInt(Base, (uint32)I3C_SINTMASKED_TXSEND_MASK))
    {
        /* Handle Tx send event */
        I3c_Ip_SlaveTxSendEventHandler(Base, Slave);
    }

     /* Check if the interrupt was spurious and call its handler */
    if (FALSE == I3c_Ip_SlaveCheckSpuriousInt(Base, (uint32)I3C_SINTMASKED_RXPEND_MASK))
    {
        /* Handle Rx pending event */
        I3c_Ip_SlaveRxPendingEventHandler(Base, Slave);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveCheckCCCEvents
* Description   : This function checks if a Common Command Code event occurred, and calls its handler if so.
*
* END**************************************************************************/
static void I3c_Ip_SlaveCheckCCCEvents(I3C_Type * const Base,
                                       const I3c_Ip_SlaveStateType * const Slave)
{
    /* Check if the interrupt was spurious and call its handler */
    if (FALSE == I3c_Ip_SlaveCheckSpuriousInt(Base, (uint32)I3C_SINTMASKED_CHANDLED_MASK))
    {
        /* Handle CCC not handled event */
        I3c_Ip_SlaveCCCHandledEventHandler(Slave);

        /* Clear interrupt flag */
        I3c_Ip_SlaveClearStatus(Base, (uint32)I3C_SSTATUS_CHANDLED_MASK);
    }

     /* Check if the interrupt was spurious and call its handler */
    if (FALSE == I3c_Ip_SlaveCheckSpuriousInt(Base, (uint32)I3C_SINTMASKED_CCC_MASK))
    {
        /* Handle CCC received event */
        I3c_Ip_SlaveCCCEventHandler(Slave);

        /* Clear interrupt flag */
        I3c_Ip_SlaveClearStatus(Base, (uint32)I3C_SSTATUS_CCC_MASK);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveCheckStartStopEvents
* Description   : This function checks if a START or STOP event occurred, and calls the corresponding handler if so.
*
* END**************************************************************************/
static void I3c_Ip_SlaveCheckStartStopEvents(I3C_Type * const Base,
                                             I3c_Ip_SlaveStateType * const Slave)
{
    /* Check if the interrupt was spurious and call its handler */
    if (FALSE == I3c_Ip_SlaveCheckSpuriousInt(Base, (uint32)I3C_SINTMASKED_START_MASK))
    {
        /* Handle Start event */
        I3c_Ip_SlaveStartEventHandler(Slave);

        /* Clear interrupt flag */
        I3c_Ip_SlaveClearStatus(Base, (uint32)I3C_SSTATUS_START_MASK);
    }

     /* Check if the interrupt was spurious and call its handler */
    if (FALSE == I3c_Ip_SlaveCheckSpuriousInt(Base, (uint32)I3C_SINTMASKED_STOP_MASK))
    {
        /* Handle STOP event */
        I3c_Ip_SlaveStopEventHandler(Slave);

        /* Clear interrupt flag */
        I3c_Ip_SlaveClearStatus(Base, (uint32)I3C_SSTATUS_STOP_MASK);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveCheckDAChangeEvents
* Description   : This function checks if a Dynamic Address change event occurred, and calls the corresponding handler if so.
*
* END**************************************************************************/
static void I3c_Ip_SlaveCheckDAChangeEvents(I3C_Type * const Base,
                                            const I3c_Ip_SlaveStateType * const Slave)
{
    /* Check if the interrupt was spurious and call its handler */
    if (FALSE == I3c_Ip_SlaveCheckSpuriousInt(Base, (uint32)I3C_SINTMASKED_DACHG_MASK))
    {
        /* Handle DA change event */
        I3c_Ip_SlaveDAChangeEventHandler(Slave);

        /* Clear interrupt flag */
        I3c_Ip_SlaveClearStatus(Base, (uint32)I3C_SSTATUS_DACHG_MASK);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveCheckHeaderMatchedEvents
* Description   : This function checks if either a header or a HDR-DDR command matched event occurred, and calls the corresponding handler if so.
*
* END**************************************************************************/
static void I3c_Ip_SlaveCheckHeaderMatchedEvents(I3C_Type * const Base,
                                                 I3c_Ip_SlaveStateType * const Slave)
{
    /* Check if the interrupt was spurious and call its handler */
    if (FALSE == I3c_Ip_SlaveCheckSpuriousInt(Base, (uint32)I3C_SINTMASKED_DDRMATCHED_MASK))
    {
        /* Handle HDR-DDR command matched event */
        I3c_Ip_SlaveDDRMatchedEventHandler(Base, Slave);

        /* Clear interrupt flag */
        I3c_Ip_SlaveClearStatus(Base, (uint32)I3C_SSTATUS_HDRMATCH_MASK);
    }

    /* Check if the interrupt was spurious and call its handler */
    if (FALSE == I3c_Ip_SlaveCheckSpuriousInt(Base, (uint32)I3C_SINTMASKED_MATCHED_MASK))
    {
        /* Handle header matched event */
        I3c_Ip_SlaveHeaderMatchedEventHandler(Base, Slave);

        /* Clear interrupt flag */
        I3c_Ip_SlaveClearStatus(Base, (uint32)I3C_SSTATUS_MATCHED_MASK);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveCheckEvents
* Description   : This function checks if a slave event (IBI, HJ, MR) occurred, and calls the corresponding handler if so.
*
* END**************************************************************************/
static void I3c_Ip_SlaveCheckEvents(I3C_Type * const Base,
                                    const I3c_Ip_SlaveStateType * const Slave)
{
    /* Check if the interrupt was spurious and call its handler */
    if (FALSE == I3c_Ip_SlaveCheckSpuriousInt(Base, (uint32)I3C_SINTMASKED_EVENT_MASK))
    {
        /* Handle IBI/HJ/MR event */
        I3c_Ip_SlaveEventsHandler(Slave);

        /* Clear interrupt flag */
        I3c_Ip_SlaveClearStatus(Base, (uint32)I3C_SSTATUS_EVENT_MASK);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterCheckCompletionEvents
* Description   : This function checks if a completion event occurred, and calls its handler if so.
*                 Completion events: master control done, complete
*
* END**************************************************************************/
static void I3c_Ip_MasterCheckCompletionEvents(I3C_Type * const Base,
                                               I3c_Ip_MasterStateType * const Master)
{
    /* Check if the interrupt was spurious, clear the interrupt, and call its handler */
    if (FALSE == I3c_Ip_MasterCheckSpuriousInt(Base, (uint32)I3C_MINTMASKED_MCTRLDONE_MASK))
    {
        /* Handle Control done event */
        I3c_Ip_MasterControlDoneEventHandler(Base, Master);

        /* Clear interrupt flag */
        I3c_Ip_MasterClearStatus(Base, (uint32)I3C_MSTATUS_MCTRLDONE_MASK);
    }
    if (FALSE == I3c_Ip_MasterCheckSpuriousInt(Base, (uint32)I3C_MINTMASKED_COMPLETE_MASK))
    {
        /* Handle Complete event */
        I3c_Ip_MasterCompleteEventHandler(Base, Master);

        /* Clear interrupt flag */
        I3c_Ip_MasterClearStatus(Base, (uint32)I3C_MSTATUS_COMPLETE_MASK);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterCheckSlaveRequestEvents
* Description   : This function checks if a slave request event occurred, and calls its handler if so.
*                 Slave requests are: In-Band Interrupt, Hot-Join, Master Request
*
* END**************************************************************************/
static void I3c_Ip_MasterCheckSlaveRequestEvents(const uint8 Instance,
                                                 I3c_Ip_MasterStateType * const Master)
{
    I3C_Type * Base = I3c_Ip_apxBase[Instance];

    /* Check if the interrupt was spurious, clear the interrupt, and call its handler */
    if (FALSE == I3c_Ip_MasterCheckSpuriousInt(Base, (uint32)I3C_MINTMASKED_SLVSTART_MASK))
    {
        /* Handle Slave start event */
        I3c_Ip_MasterSlaveStartEventHandler(Base, Master);

        /* Clear interrupt flag */
        I3c_Ip_MasterClearStatus(Base, (uint32)I3C_MSTATUS_SLVSTART_MASK);
    }

    if (FALSE == I3c_Ip_MasterCheckSpuriousInt(Base, (uint32)I3C_MINTMASKED_IBIWON_MASK))
    {
        /* Handle Master Arbitration lost event */
        I3c_Ip_MasterArbitrationLostEventHandler(Instance, Master);

        /* Clear interrupt flag */
        I3c_Ip_MasterClearStatus(Base, (uint32)I3C_MSTATUS_IBIWON_MASK);
    }

    if (FALSE == I3c_Ip_MasterCheckSpuriousInt(Base, (uint32)I3C_MINTMASKED_NOWMASTER_MASK))
    {
        /* Handle Now Master event */
        I3c_Ip_MasterNowMasterEventHandler(Instance, Master);

        /* Clear interrupt flag */
        I3c_Ip_MasterClearStatus(Base, (uint32)I3C_MSTATUS_NOWMASTER_MASK);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterCheckErrorEvents
* Description   : This function checks for error events for the master module.
*
* END**************************************************************************/
static void I3c_Ip_MasterCheckErrorEvents(I3C_Type * const Base,
                                          I3c_Ip_MasterStateType * const Master)
{
    if (FALSE == I3c_Ip_MasterCheckSpuriousInt(Base, (uint32)I3C_MINTMASKED_ERRWARN_MASK))
    {
        /* Handle error event */
        I3c_Ip_MasterErrorEventHandler(Base, Master);

        /* Clear the error interrupt status flags */
        I3c_Ip_MasterClearErrorStatusFlags(Base, (I3C_IP_MASTER_ERROR_NACK            |
                                                  I3C_IP_MASTER_ERROR_WRITE_ABORT     |
                                                  I3C_IP_MASTER_ERROR_TERM            |
                                                  I3C_IP_MASTER_ERROR_PARITY          |
                                                  I3C_IP_MASTER_ERROR_CRC             |
                                                  I3C_IP_MASTER_ERROR_OREAD           |
                                                  I3C_IP_MASTER_ERROR_OWRITE          |
                                                  I3C_IP_MASTER_ERROR_MSG             |
                                                  I3C_IP_MASTER_ERROR_INVALID_REQUEST |
                                                  I3C_IP_MASTER_ERROR_TIMEOUT));
#if (STD_ON == I3C_IP_HDR_BT_SUPPORT)
        I3c_Ip_MasterClearErrorStatusFlags(Base, (I3C_IP_MASTER_ERROR_WRONG_SIZE      |
                                                  I3C_IP_MASTER_ERROR_BT_TX_UNDERRUN  |
                                                  I3C_IP_MASTER_ERROR_BT_NO_VERIFY));
#endif /* (STD_ON == I3C_IP_HDR_BT_SUPPORT) */
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveCheckErrorEvents
* Description   : This function checks for error events for the slave module.
*
* END**************************************************************************/
static void I3c_Ip_SlaveCheckErrorEvents(I3C_Type * const Base,
                                         I3c_Ip_SlaveStateType * const Slave)
{
    /* Check if the interrupt was spurious */
    if (FALSE == I3c_Ip_SlaveCheckSpuriousInt(Base, (uint32)I3C_SINTMASKED_ERRWARN_MASK))
    {
        /* Handle error event */
        I3c_Ip_SlaveErrorEventHandler(Base, Slave);

        /* Clear the error interrupt status flags */
        I3c_Ip_SlaveClearErrorStatusFlags(Base, (I3C_IP_SLAVE_ERROR_RX_OVERRUN       |
                                                 I3C_IP_SLAVE_ERROR_TX_UNDERRUN      |
                                                 I3C_IP_SLAVE_ERROR_TX_UNDERRUN_NACK |
                                                 I3C_IP_SLAVE_ERROR_TERM             |
                                                 I3C_IP_SLAVE_ERROR_INVALID_START    |
                                                 I3C_IP_SLAVE_ERROR_SDR_PARITY       |
                                                 I3C_IP_SLAVE_ERROR_HDR_PARITY_ERROR |
                                                 I3C_IP_SLAVE_ERROR_HDR_CRC          |
                                                 I3C_IP_SLAVE_ERROR_S0S1             |
                                                 I3C_IP_SLAVE_ERROR_OREAD            |
                                                 I3C_IP_SLAVE_ERROR_OWRITE));

#if (STD_ON == I3C_IP_HDR_BT_SUPPORT)
        I3c_Ip_SlaveClearErrorStatusFlags(Base, (I3C_IP_SLAVE_ERROR_WRONG_SIZE  |
                                                 I3C_IP_SLAVE_ERROR_BT_INV_REQ  |
                                                 I3C_IP_SLAVE_ERROR_BT_NO_VERIFY));
#endif /* (STD_ON == I3C_IP_HDR_BT_SUPPORT) */
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterIRQHandler
* Description   : This function handles I3C master interrupts.
*
* END**************************************************************************/
static void I3c_Ip_MasterIRQHandler(const uint8 Instance)
{
    I3C_Type * Base;
    I3c_Ip_MasterStateType * Master;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];
    Master = I3c_Ip_apxMasterState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Master != NULL_PTR);
#endif

    /* Check which interrupt was triggered and call its handler */
    I3c_Ip_MasterCheckFifoEvents(Base, Master);

    I3c_Ip_MasterCheckSlaveRequestEvents(Instance, Master);

    I3c_Ip_MasterCheckCompletionEvents(Base, Master);

    I3c_Ip_MasterCheckErrorEvents(Base, Master);
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveIRQHandler
* Description   : This function handles I3C slave interrupts.
*
* END**************************************************************************/
static void I3c_Ip_SlaveIRQHandler(const uint8 Instance)
{
    I3C_Type * Base;
    I3c_Ip_SlaveStateType * Slave;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];
    Slave = I3c_Ip_apxSlaveState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Slave != NULL_PTR);
#endif

    /* Check which interrupt was triggered and call its handler */
    I3c_Ip_SlaveCheckHeaderMatchedEvents(Base, Slave);

    I3c_Ip_SlaveCheckFifoEvents(Base, Slave);

    I3c_Ip_SlaveCheckStartStopEvents(Base, Slave);

    I3c_Ip_SlaveCheckCCCEvents(Base, Slave);

    I3c_Ip_SlaveCheckDAChangeEvents(Base, Slave);

    I3c_Ip_SlaveCheckEvents(Base, Slave);

    I3c_Ip_SlaveCheckErrorEvents(Base, Slave);
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterInitTransfer
* Description   : This function initializes the transfer, either send or receive.
*                 It returns I3C_IP_STATUS_SUCCESS if no timeout occurred in master requests.
*
* END**************************************************************************/
static void I3c_Ip_MasterInitTransfer(I3C_Type * const Base,
                                      const I3c_Ip_MasterStateType * const Master)
{
    uint8 GetIbiResp = 0x00U;
    uint8 ReadTerminate = 0x00U;

    /* In address header, the first START + I3C_IP_BROADCAST_ADDRESS + W should meet an open drain timing.
       For this, MCONFIG[ODHPP] = 0 and ODBAUD are used to ensure at least 200 ns for the Open-Drain half-period.
       Following a Repeated START or a STOP, after the first I3C_IP_BROADCAST_ADDRESS, all subsequent messages should use MCONFIG[ODHPP] = 1.
       This is valid only for an I3c Bus, since for an I2c bus type MCONFIG[ODHPP] is ignored and only I2c Baud Rate is relevant. */

    /* Enter critical section */
    SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_03();

    if (I3C_IP_BROADCAST_ADDRESS == Master->TransferOption.SlaveAddress)
    {
        Base->MCONFIG = (Base->MCONFIG & (~I3C_MCONFIG_ODHPP_MASK)) | I3C_MCONFIG_ODHPP(0U);
    }
    else
    {
        Base->MCONFIG = (Base->MCONFIG & (~I3C_MCONFIG_ODHPP_MASK)) | I3C_MCONFIG_ODHPP(1U);
    }

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
    GetIbiResp = (uint8)Master->IbiResp;
#endif

    if (I3C_IP_READ == Master->TransferOption.Direction)
    {
        ReadTerminate = (uint8)Master->BufferSize;
    }

    /* Emit START + slave address */
    Base->MCTRL = (Base->MCTRL & (~(I3C_MCTRL_TYPE_MASK   |
                                    I3C_MCTRL_REQUEST_MASK |
                                    I3C_MCTRL_DIR_MASK     |
                                    I3C_MCTRL_RDTERM_MASK  |
                                    I3C_MCTRL_IBIRESP_MASK |
                                    I3C_MCTRL_ADDR_MASK))) |
                                    I3C_MCTRL_TYPE(Master->TransferOption.BusType)         |
                                    I3C_MCTRL_RDTERM(ReadTerminate) |
                                    I3C_MCTRL_IBIRESP(GetIbiResp)                      |
                                    I3C_MCTRL_REQUEST(I3C_IP_REQUEST_EMIT_START_ADDR)       |
                                    I3C_MCTRL_DIR(Master->TransferOption.Direction)        |
                                    I3C_MCTRL_ADDR(Master->TransferOption.SlaveAddress);
    /* Exit critical section */
    SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_03();
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterSendData
* Description   : Master send handler.
*
* END**************************************************************************/
static void I3c_Ip_MasterSendData(I3C_Type * const Base,
                                  I3c_Ip_MasterStateType * const Master)
{
    /* Check if there is more data to transfer, according to the Tx FIFO watermark */
    if (TRUE == I3c_Ip_MasterCheckStatus(Base, (uint32)I3C_MSTATUS_TXNOTFULL_MASK))
    {
        /* Write data in the Tx FIFO */
        I3c_Ip_MasterSendMessage(Base, Master);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterReceiveData
* Description   : This function initializes master receive operation.
*
* END**************************************************************************/
static void I3c_Ip_MasterReceiveData(const I3C_Type * const Base,
                                     I3c_Ip_MasterStateType * const Master)
{
    /* Check if Rx FIFO contains any data to be received, according to the Rx FIFO watermark */
    if (TRUE == I3c_Ip_MasterCheckStatus(Base, (uint32)I3C_MSTATUS_RXPEND_MASK))
    {
        /* Read data from Rx FIFO */
        I3c_Ip_MasterReceiveMessage(Base, Master);
    }
}

#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterSendDAARequest
* Description   : Send the Dynamic Address to the slave which won the arbitration.
*
* END**************************************************************************/
static I3c_Ip_StatusType I3c_Ip_MasterSendDAARequest(const uint8 Instance,
                                                     const uint8 * const DynamicAddressList,
                                                     I3c_Ip_SlaveDeviceType * const SlaveDeviceList,
                                                     uint8 * SlaveIndex)
{
    I3C_Type * Base = I3c_Ip_apxBase[Instance];
    I3c_Ip_StatusType ReturnStatus;
    uint8 RxCount;
    uint8 TempCount;

    /* This buffer is used by the master to receive the Provisional IDs of the slaves which participate in DAA process.*/
    uint8 TempRxBuffer[I3C_IP_SLAVE_ID_SIZE] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

    /* Wait for Control Done flag */
    ReturnStatus = I3c_Ip_MasterWaitForStatus(Base, (uint32)I3C_MSTATUS_MCTRLDONE_MASK);

    if ((I3C_IP_STATUS_SUCCESS == ReturnStatus) && (TRUE == I3c_Ip_MasterCheckStatus(Base, (uint32)I3C_MSTATUS_RXPEND_MASK)))
    {
        TempCount = 0U;
        RxCount = (uint8)((Base->MDATACTRL & I3C_MDATACTRL_RXCOUNT_MASK) >> I3C_MDATACTRL_RXCOUNT_SHIFT);

        while ((TempCount < I3C_IP_SLAVE_ID_SIZE) && (RxCount > 0U))
        {
            TempRxBuffer[TempCount] = I3c_Ip_ReadMRDATABRegister(Base);
            TempCount++;
        }

        if (SlaveDeviceList != NULL_PTR)
        {
            SlaveDeviceList[*SlaveIndex].DynamicAddress = DynamicAddressList[*SlaveIndex];
            SlaveDeviceList[*SlaveIndex].VendorId       = ((((uint16)TempRxBuffer[0U] << 8U) | (uint16)TempRxBuffer[1U]) & 0xFFFEU) >> 1U;
            SlaveDeviceList[*SlaveIndex].PartNumber     = ((uint32)TempRxBuffer[2U] << 24U) |
                                                          ((uint32)TempRxBuffer[3U] << 16U) |
                                                          ((uint32)TempRxBuffer[4U] << 8U)  |
                                                           (uint32)TempRxBuffer[5U];
            SlaveDeviceList[*SlaveIndex].BCR            = TempRxBuffer[6U];
            SlaveDeviceList[*SlaveIndex].DCR            = TempRxBuffer[7U];
        }

        if ((TRUE == I3c_Ip_MasterGetState(Base, (uint32)I3C_IP_MASTER_MODE_DAA)) &&
            ((Base->MSTATUS & (uint32)I3C_MSTATUS_BETWEEN_MASK) != 0U))
        {
            /* Write the DA for the slave whose ID was read */
            Base->MWDATAB = (uint32)(DynamicAddressList[*SlaveIndex]);

            (*SlaveIndex)++;

            /* Not needed to add exclusive area to protect RMW in function I3c_Ip_MasterEmitRequest
             * because I3c_Ip_MasterSendDAARequest only called from I3c_Ip_MasterProcessDAA withing EXCLUSIVE_AREA_04 */
            /* Init DAA again to send the DA to the slave */
            I3c_Ip_MasterEmitRequest(Base, I3C_IP_REQUEST_PROCESS_DAA);
        }
    }

    (void)SlaveIndex;

    return ReturnStatus;
}
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES) */

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveWriteIbiExtraBytes
* Description   : Write the extra bytes if the In-Band interrupt has more bytes passed the MDB.
*
* END**************************************************************************/
static void I3c_Ip_SlaveWriteIbiExtraBytes(const uint8 Instance,
                                           const uint8 * ExtData)
{

    I3C_Type * Base;
    const I3c_Ip_SlaveStateType * Slave;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Slave = I3c_Ip_apxSlaveState[Instance];
    Base = I3c_Ip_apxBase[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Slave != NULL_PTR);
#endif

    switch (Slave->CountExtraBytes)
    {
        case 1U:
        {
            Base->IBIEXT1 = (Base->IBIEXT1 & (~I3C_IBIEXT1_EXT1_MASK)) | I3C_IBIEXT1_EXT1(ExtData[0U]);
            break;
        }
        case 2U:
        {
            Base->IBIEXT1 = (Base->IBIEXT1 & (~(I3C_IBIEXT1_EXT1_MASK              |
                                                I3C_IBIEXT1_EXT2_MASK)))           |
                                                I3C_IBIEXT1_EXT1(ExtData[0U])      |
                                                I3C_IBIEXT1_EXT2(ExtData[1U]);
            break;
        }
        case 3U:
        {
            Base->IBIEXT1 = (Base->IBIEXT1 & (~(I3C_IBIEXT1_EXT1_MASK              |
                                                I3C_IBIEXT1_EXT2_MASK              |
                                                I3C_IBIEXT1_EXT3_MASK)))           |
                                                I3C_IBIEXT1_EXT1(ExtData[0U])      |
                                                I3C_IBIEXT1_EXT2(ExtData[1U])      |
                                                I3C_IBIEXT1_EXT3(ExtData[2U]);
            break;
        }
        case 4U:
        {
            Base->IBIEXT1 = (Base->IBIEXT1 & (~(I3C_IBIEXT1_EXT1_MASK           |
                                                I3C_IBIEXT1_EXT2_MASK           |
                                                I3C_IBIEXT1_EXT3_MASK)))        |
                                                I3C_IBIEXT1_EXT1(ExtData[0U])   |
                                                I3C_IBIEXT1_EXT2(ExtData[1U])   |
                                                I3C_IBIEXT1_EXT3(ExtData[2U]);
            Base->IBIEXT2 = (Base->IBIEXT2 & (~I3C_IBIEXT2_EXT4_MASK)) | I3C_IBIEXT2_EXT4(ExtData[3U]);
            break;
        }
#if (STD_ON == I3C_IP_OVER_IBI_EXTRA_DATA_BYTES_THR)
        case 5U:
        {
            Base->IBIEXT1 = (Base->IBIEXT1 & (~(I3C_IBIEXT1_EXT1_MASK           |
                                                I3C_IBIEXT1_EXT2_MASK           |
                                                I3C_IBIEXT1_EXT3_MASK)))        |
                                                I3C_IBIEXT1_EXT1(ExtData[0U])   |
                                                I3C_IBIEXT1_EXT2(ExtData[1U])   |
                                                I3C_IBIEXT1_EXT3(ExtData[2U]);
            Base->IBIEXT2 = (Base->IBIEXT2 & (~(I3C_IBIEXT2_EXT4_MASK           |
                                                I3C_IBIEXT2_EXT5_MASK)))        |
                                                I3C_IBIEXT2_EXT4(ExtData[3U])   |
                                                I3C_IBIEXT2_EXT5(ExtData[4U]);
            break;
        }
        case 6U:
        {
            Base->IBIEXT1 = (Base->IBIEXT1 & (~(I3C_IBIEXT1_EXT1_MASK           |
                                                I3C_IBIEXT1_EXT2_MASK           |
                                                I3C_IBIEXT1_EXT3_MASK)))        |
                                                I3C_IBIEXT1_EXT1(ExtData[0U])   |
                                                I3C_IBIEXT1_EXT2(ExtData[1U])   |
                                                I3C_IBIEXT1_EXT3(ExtData[2U]);
            Base->IBIEXT2 = (Base->IBIEXT2 & (~(I3C_IBIEXT2_EXT4_MASK           |
                                                I3C_IBIEXT2_EXT5_MASK           |
                                                I3C_IBIEXT2_EXT6_MASK)))        |
                                                I3C_IBIEXT2_EXT4(ExtData[3U])   |
                                                I3C_IBIEXT2_EXT5(ExtData[4U])   |
                                                I3C_IBIEXT2_EXT6(ExtData[5U]);
            break;
        }
        case 7U:
        {
            Base->IBIEXT1 = (Base->IBIEXT1 & (~(I3C_IBIEXT1_EXT1_MASK           |
                                                I3C_IBIEXT1_EXT2_MASK           |
                                                I3C_IBIEXT1_EXT3_MASK)))        |
                                                I3C_IBIEXT1_EXT1(ExtData[0U])   |
                                                I3C_IBIEXT1_EXT2(ExtData[1U])   |
                                                I3C_IBIEXT1_EXT3(ExtData[2U]);
            Base->IBIEXT2 = 0U;
            Base->IBIEXT2 = I3C_IBIEXT2_EXT4(ExtData[3U])   |
                            I3C_IBIEXT2_EXT5(ExtData[4U])   |
                            I3C_IBIEXT2_EXT6(ExtData[5U])   |
                            I3C_IBIEXT2_EXT7(ExtData[6U]);
            break;
        }
#endif /* (STD_ON == I3C_IP_OVER_IBI_EXTRA_DATA_BYTES_THR) */
        default:
        {
            /* Do nothing */
            break;
        }
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveRequestIbi
* Description   : Slave request In-Band interrupt.
*
* END**************************************************************************/
static void I3c_Ip_SlaveRequestIbi(const uint8 Instance,
                                   const uint8 IbiData,
                                   const uint8 * ExtData)
{
    I3C_Type * Base;
    const I3c_Ip_SlaveStateType * Slave;
    uint8 MaxNumOfIbiExtraBytes;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Slave = I3c_Ip_apxSlaveState[Instance];
    Base = I3c_Ip_apxBase[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Slave != NULL_PTR);
#endif

    /* Enter critical section */
    SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_04();

    /* Write the mandatory byte to be sent with the In-Band interrupt */
    Base->SCTRL = (Base->SCTRL & (~(I3C_SCTRL_IBIDATA_MASK | I3C_SCTRL_EXTDATA_MASK))) | I3C_SCTRL_IBIDATA(IbiData) | I3C_SCTRL_EXTDATA(Slave->ExtendedIbiData ? 1U : 0U);

    /* Exit critical section */
    SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_04();
    if (TRUE == Slave->ExtendedIbiData)
    {
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
        DevAssert(ExtData != NULL_PTR);
#endif
        MaxNumOfIbiExtraBytes = (uint8)((Base->IBIEXT1 & (uint32)I3C_IBIEXT1_MAX_MASK) >> I3C_IBIEXT1_MAX_SHIFT);
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
        DevAssert(Slave->CountExtraBytes <= MaxNumOfIbiExtraBytes);
#endif
        Base->IBIEXT1 = (Base->IBIEXT1 & (~I3C_IBIEXT1_CNT_MASK)) | I3C_IBIEXT1_CNT(Slave->CountExtraBytes);
        /* Write the extra bytes */
        I3c_Ip_SlaveWriteIbiExtraBytes(Instance, ExtData);
    }
}
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_MasterComputeBestI2cBaudCount
* Description   : Used to check and determine if there is an I2CBAUD Count which sets an open-drain
*                 baud rate closer to the target open-drain baud rate.
*
* END**************************************************************************/
static uint8 I3c_MasterComputeBestI2cBaudCount(const uint32 OpenDrainBaudRate,
                                               const uint32 TargetI2cBaudRate,
                                               const uint8 CurrentI2cBaudCount)
{
    uint8 I2cBaudCount = 0U;
    uint32 I2cBaudRateCalc;
    uint32 MinDiff;
    uint32 Temp;
    uint8 count;

    /*
    Considering Div = OpenDrainBaudRate / TargetI2cBaudRate, then the following scenarios have been obtained
    by measuring the SCL frequency:

    Measured OpenDrainBaudRate --> 2.5 MHz

    I2cBaudCount = 0 --> SCL = 2.5 MHz    --> Div = 1
    I2cBaudCount = 1 --> SCL = 1.66 MHz   --> Div = 1.5

    I2cBaudCount = 2 --> SCL = 1.25 MHz   --> Div = 2
    I2cBaudCount = 3 --> SCL = 1 MHz      --> Div = 2.5

    I2cBaudCount = 4 --> SCL = 833.33 kHz --> Div = 3
    I2cBaudCount = 5 --> SCL = 714 kHz    --> Div = 3.5

    I2cBaudCount = 6 --> SCL = 625 kHz    --> Div = 4
    I2cBaudCount = 7 --> SCL = 555.55 kHz --> Div = 4.5

    I2cBaudCount = 8 --> SCL = 500 kHz    --> Div = 5
    I2cBaudCount = 9 --> SCL = 454.5 kHz  --> Div = 5.5

    Using these cases, the following relations were determined:
            - I2cBaudCount = 2 * (Div - 1) when I2cBaudCount is even
            - I2cBaudCount = 2 * (Div - 1) + 1 when I2cBaudCount is odd

    However, the following formula to set the I2c Baud Count was used:

            - I2cBaudCount = 2 * (Div - 1)

    By using this equation, an I2c Baud Rate different than the target one is obtained. For example, if target
    I2c Baud Rate is 626 kHz results in I2cBaudCount = 4, which gives calculated I2c Baud Rate of 833.33 kHZ.
    As remarked from the above scenarios, I2cBaudCount changes when Div changes with a step of 0.5. Thus, searching
    for an I2cBaudCount which gives an I2c Baud Rate closer to the target one takes place among Div, Div + 0.5 and Div + 1.

    Calculated I2c Baud Rate is determined using the following relation:
            - Calculated I2c Baud Rate = (2 * OpenDrainBaudRate) / (I2cBaudCount + 2)

    After Calculated I2c Baud Rate is determined for Div, Div + 0.5 and Div + 1, the I2cBaudCount is chosen as the one which gives
    the smallest difference between target I2c Baud Rate and the calculated ones.
    */

    count = 0U;
    MinDiff = TargetI2cBaudRate;
    do
    {
        I2cBaudRateCalc = (OpenDrainBaudRate << 1U) / (CurrentI2cBaudCount + count + 2U);
        if (I2cBaudRateCalc > TargetI2cBaudRate)
        {
            Temp = I2cBaudRateCalc - TargetI2cBaudRate;
            if (Temp <= MinDiff)
            {
                MinDiff = Temp;
                I2cBaudCount = CurrentI2cBaudCount + count;
            }
        }
        else
        {
            Temp = TargetI2cBaudRate - I2cBaudRateCalc;
            if (Temp <= MinDiff)
            {
                MinDiff = Temp;
                I2cBaudCount = CurrentI2cBaudCount + count;
            }
        }

        count++;

    } while (count < 3U);

    if (I2cBaudCount > I3C_IP_MAX_I2CBAUD)
    {
        I2cBaudCount = I3C_IP_MAX_I2CBAUD;
    }

    return I2cBaudCount;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_ComputePpBaudRate
* Description   : Used to compute PPBAUD and PPLOW.
*
* END**************************************************************************/
static uint8 I3c_Ip_ComputePpBaudRate(I3C_Type * const Base,
                                      const uint32 FunctionalClock,
                                      const uint32 TargetPpBaudRate,
                                      const I3c_Ip_BusType BusType)
{
    uint8 PpDiv;
    uint32 MaxPpBaudRate;
    uint8 PpBaudCount;
    uint8 PushPullLow = 0U;

    /* I3c Standard specifies a maximum Push-Pull frequency of 12.9 MHz in SDR mode.
       Also, it specifies that in HDR-DDR mode SDA changes on both SCL edges (when High and when Low), effectively doubling the data rate.
       Thus, one could assume a maximum Push-Pull frequency of 25.8 MHz in HDR-DDR mode. */
    MaxPpBaudRate = (BusType == I3C_IP_BUS_TYPE_I3C_DDR) ? 25800000U : 12900000U;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(TargetPpBaudRate <= MaxPpBaudRate);
#endif

    /* Push-Pull frequency = FCLK / ((2 * (PPBAUD + 1)) + PPLOW), 0 <= PPBAUD <= 15
       Compute the divider to generate target PP frequency. */
    PpDiv = (uint8)(FunctionalClock / TargetPpBaudRate);

    while ((FunctionalClock / PpDiv) > MaxPpBaudRate)
    {
        PpDiv++;
    }

    if ((PpDiv & 0x1U) != 0U)
    {
        PpBaudCount = PpDiv >> 1U;

        /* PPLOW is used to add more FCLKs to the low Push-Pull period. Thus, duty cycle is changed and
           Push-Pull Frequency is decreased. This is usually used when a lower frequency is desired but
           Push-Pull high period is kept lower than 50ns to ensure that I2C devices do not see the high period. */
        PushPullLow = 1U;
    }
    else
    {
        PpBaudCount = (PpDiv >> 1U) - 1U;
    }

    if (PpBaudCount > I3C_IP_MAX_PPBAUD)
    {
        PpBaudCount = I3C_IP_MAX_PPBAUD;
    }

    /* Enter critical section */
    SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_05();

    Base->MCONFIG = (Base->MCONFIG & (~I3C_MCONFIG_PPLOW_MASK)) | I3C_MCONFIG_PPLOW(PushPullLow);

    /* Exit critical section */
    SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_05();

    return PpBaudCount;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_ComputeOdBaudRate
* Description   : Used to compute ODBAUD.
*
* END**************************************************************************/
static uint8 I3c_Ip_ComputeOdBaudRate(const I3C_Type * const Base,
                                      const uint8 PpBaudCount,
                                      const uint32 FunctionalClock,
                                      const uint32 TargetOdBaudRate)
{
    uint32 Freq;
    uint32 MaxOdBaudRate;
    uint8 OdBaudCount;
    uint8 OdDiv;

    /* I3c Standard specifies a minimum low Open-Drain period of 200ns, which means a maximum frequency of 2.5 MHz
       if the high Open-Drain period is the same as the low period. If high push-pull period is used for the high open-drain period (ODHPP = 1),
       then the maximum open-drain frequency becomes aprox. 5 MHz (using high push-pull period for Push-Pull frequency of 12.9 MHz).
       Adding 10% margin when ODHPP = 0 to ensure maximum value from Standard could be reached.*/
    MaxOdBaudRate = ((Base->MCONFIG & (uint32)I3C_MCONFIG_ODHPP_MASK) != 0U) ? 5000000U : 2700000U;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(TargetOdBaudRate <= MaxOdBaudRate);
#endif

    /* Open-Drain frequency = PPfreq / (ODBAUD + 1), 1 <= ODBAUD <= 255
       Compute the divider to generate target OD frequency.
       Calculated push-pull frequency when high push-pull period is the same as the low push-pull period. */
    Freq = FunctionalClock / ((PpBaudCount + 1UL) << 1UL);

    OdDiv = (uint8)(Freq / TargetOdBaudRate);

    while ((Freq / OdDiv) > MaxOdBaudRate)
    {
        OdDiv++;
    }

    OdBaudCount = OdDiv - 1U;

    return OdBaudCount;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_ComputeI2cBaudRate
* Description   : Used to compute I2CBAUD.
*
* END**************************************************************************/
static uint8 I3c_Ip_ComputeI2cBaudRate(const uint8 PpBaudCount,
                                       const uint8 OdBaudCount,
                                       const uint32 FunctionalClock,
                                       const uint32 TargetI2cBaudRate)
{
    uint8 TempI2cBaudCount;
    uint8 I2cBaudCount;
    uint32 Freq;
    uint32 I2cDiv;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(TargetI2cBaudRate <= I3C_IP_MAX_I2C_BAUD_RATE);
#endif

    /* I2c frequency = ODfreq / (I2CBAUD + 1), 0 <= I2CBAUD <= 15
       Compute the divider to generate target I2C frequency.
       Calculated open-drain frequency when high open-drain period is the same as the low open-drain period.*/
    Freq = FunctionalClock / (((PpBaudCount + 1UL) << 1UL) * (OdBaudCount + 1UL));

    I2cDiv = Freq / TargetI2cBaudRate;

    while ((Freq / I2cDiv) > I3C_IP_MAX_I2C_BAUD_RATE)
    {
        I2cDiv++;
    }

    /* Compute I2CBAUD count for determined div, yet not the final I2CBAUD count */
    TempI2cBaudCount = (uint8)((I2cDiv - 1U) << 1U);

    I2cBaudCount = I3c_MasterComputeBestI2cBaudCount(Freq, TargetI2cBaudRate, TempI2cBaudCount);

    return I2cBaudCount;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterTxConfig
* Description   : Used to pre-fill FIFO if target address is not the broadcast address.
*
* END**************************************************************************/
static void I3c_Ip_MasterTxConfig(const uint8 Instance)
{
    I3C_Type * Base;
    I3c_Ip_MasterStateType * Master;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];
    Master = I3c_Ip_apxMasterState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Master != NULL_PTR);
#endif

    /* If HDR-DDR mode, write the command code */
    I3c_Ip_MasterCheckWriteHdrDdrCommandCode(Base, Master);

    /* Write Master Tx FIFO with data. */
    I3c_Ip_MasterSendMessage(Base, Master);

    if (Master->BufferSize > 0U)
    {
        /* Enter critical section */
        SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_06();

        /* Set Tx FIFO watermark */
        I3c_Ip_MasterSetTxFifoWatermark(Base, I3C_IP_FIFO_TRIGGER_ON_ALMOST_FULL);

        /* Exit critical section */
        SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_06();
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterSendMessageBlocking
* Description   : Master writes message in Blocking manner.
*
* END**************************************************************************/
static I3c_Ip_StatusType I3c_Ip_MasterSendMessageBlocking(I3C_Type * const Base,
                                                          I3c_Ip_MasterStateType * const Master)
{
    I3c_Ip_StatusType ReturnStatus;

    uint32 CurrentTicks = OsIf_GetCounter(I3C_IP_TIMEOUT_TYPE);
    uint32 ElapsedTicks = 0U;
    uint32 TimeoutTicks = OsIf_MicrosToTicks(I3C_IP_TIMEOUT_VALUE, I3C_IP_TIMEOUT_TYPE);

    while ((ElapsedTicks < TimeoutTicks) && (I3c_Ip_MasterCheckStatus(Base, (uint32)I3C_MSTATUS_COMPLETE_MASK) == FALSE))
    {
#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
        if (Master->TransferType != I3C_IP_USING_DMA)
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */
        {
            /* Master send data */
            I3c_Ip_MasterSendData(Base, Master);
        }
        ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, I3C_IP_TIMEOUT_TYPE);
    }

    if (ElapsedTicks >= TimeoutTicks)
    {
        ReturnStatus = I3C_IP_STATUS_TIMEOUT;
    }
    else
    {
        if (Master->MasterCallback != NULL_PTR)
        {
            Master->MasterCallback(I3C_IP_MASTER_EVENT_END_TRANSFER);
        }

        I3c_Ip_MasterEndTransfer(Base, Master, FALSE, Master->TransferOption.SendStop);

        if (TRUE == Master->TransferOption.SendStop)
        {
            /* Wait for control done flag */
            ReturnStatus = I3c_Ip_MasterWaitForStatus(Base, (uint32)I3C_MSTATUS_MCTRLDONE_MASK);
        }
        else
        {
            ReturnStatus = I3C_IP_STATUS_SUCCESS;
        }
    }

    return ReturnStatus;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterReceiveMessageBlocking
* Description   : Master reads message in Blocking manner.
*
* END**************************************************************************/
static I3c_Ip_StatusType I3c_Ip_MasterReceiveMessageBlocking(I3C_Type * const Base,
                                                             I3c_Ip_MasterStateType * const Master)
{
    I3c_Ip_StatusType ReturnStatus;

    uint32 CurrentTicks = OsIf_GetCounter(I3C_IP_TIMEOUT_TYPE);
    uint32 ElapsedTicks = 0U;
    uint32 TimeoutTicks = OsIf_MicrosToTicks(I3C_IP_TIMEOUT_VALUE, I3C_IP_TIMEOUT_TYPE);

    /* Receiving data is read until Complete status is set, thus, when the last byte is received in the Master Rx FIFO.
       This means that I3c_Ip_MasterReceiveData() will not be called inside this loop to read the last byte from the FIFO.
       It shall be called again to ensure all data bytes are read. */
    while ((ElapsedTicks < TimeoutTicks) && (I3c_Ip_MasterCheckStatus(Base, (uint32)I3C_MSTATUS_COMPLETE_MASK) == FALSE))
    {
#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
        if (Master->TransferType != I3C_IP_USING_DMA)
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */
        {
            /* Master read data */
            I3c_Ip_MasterReceiveData(Base, Master);
        }
        ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, I3C_IP_TIMEOUT_TYPE);
    }

    if (ElapsedTicks >= TimeoutTicks)
    {
        ReturnStatus = I3C_IP_STATUS_TIMEOUT;
    }
    else
    {
        /* I3c_Ip_MasterReceiveData() is called again here to read the last byte which is in the Rx FIFO. */
        I3c_Ip_MasterReceiveData(Base, Master);

        if (Master->MasterCallback != NULL_PTR)
        {
            Master->MasterCallback(I3C_IP_MASTER_EVENT_END_TRANSFER);
        }

        I3c_Ip_MasterEndTransfer(Base, Master, FALSE, Master->TransferOption.SendStop);

        if (TRUE == Master->TransferOption.SendStop)
        {
            /* Wait for control done flag */
            ReturnStatus = I3c_Ip_MasterWaitForStatus(Base, (uint32)I3C_MSTATUS_MCTRLDONE_MASK);
        }
        else
        {
            ReturnStatus = I3C_IP_STATUS_SUCCESS;
        }
    }

    return ReturnStatus;
}

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterEmitStop
* Description   : This function is used to emit STOP on the bus for I2C and SDR messages.
*
* END**************************************************************************/
static void I3c_Ip_MasterEmitStop(I3C_Type * const Base,
                                  const boolean SendStop)
{
    if (TRUE == SendStop)
    {
        I3c_Ip_MasterEmitRequest(Base, I3C_IP_REQUEST_EMIT_STOP);
    }
}
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterCheckWriteHdrDdrCommandCode
* Description   : If the I3c bus is in the HDR-DDR mode, write the HDR-DDR command code into MWDATAB register.
*                 I3c specification mentions more possible values for the HDR-DDR command code for a write to the slave: 0x00 - 0x7F
*                 and for a read from the slave: 0x80 - 0xFF.
*                 However, I3c specification does not mention how these values shall be chosen. Thus, current driver uses 0x00 as HDR-DDR
*                 command code for write to the slave, and 0x80 for read from the slave.
*
* END**************************************************************************/
static void I3c_Ip_MasterCheckWriteHdrDdrCommandCode(I3C_Type * const Base,
                                                     const I3c_Ip_MasterStateType * const Master)
{
    if (I3C_IP_BUS_TYPE_I3C_DDR == Master->TransferOption.BusType)
    {
        if (I3C_IP_WRITE == Master->TransferOption.Direction)
        {
            Base->MWDATAB = (uint32)0x00U;
        }
        else
        {
            Base->MWDATAB = (uint32)0x80U;
        }
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterWriteByteMessage
* Description   : Master writes data to be sent, in bytes.
*
* END**************************************************************************/
static void I3c_Ip_MasterWriteByteMessage(I3C_Type * const Base,
                                          I3c_Ip_MasterStateType * const Master)
{
    uint8 TxCount;

    TxCount = (uint8)((Base->MDATACTRL & I3C_MDATACTRL_TXCOUNT_MASK) >> I3C_MDATACTRL_TXCOUNT_SHIFT);

    /* Note that when in HDR-DDR, if an odd number of bytes is used, this leads to an extra 0x00 byte to be also sent
       (regardless of the transfer size in bytes, or in half-words) because HDR-DDR mode works with byte-pairs. */
    while ((Master->BufferSize > 0U) && (TxCount < I3C_IP_TX_FIFO_SIZE))
    {
        if (Master->BufferSize > 1U)
        {
            Base->MWDATAB = (uint32)Master->TxDataBuffer[0U];

        }
        else
        {
            Base->MWDATABE = (uint32)Master->TxDataBuffer[0U];
        }

        Master->TxDataBuffer = &(Master->TxDataBuffer[1U]);
        Master->BufferSize -= 1U;

        TxCount = (uint8)((Base->MDATACTRL & I3C_MDATACTRL_TXCOUNT_MASK) >> I3C_MDATACTRL_TXCOUNT_SHIFT);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterWriteHalfWordMessage
* Description   : Master writes data to be sent, in half-words.
*
* END**************************************************************************/
static void I3c_Ip_MasterWriteHalfWordMessage(I3C_Type * const Base,
                                              I3c_Ip_MasterStateType * const Master)
{
    uint8 TxCount;

    /* MaxNumOfBytes ensures that the Tx FIFO is written only if there is room for both bytes. */
    const uint8 MaxNumOfBytes = I3C_IP_TX_FIFO_SIZE - 1U;

    TxCount = (uint8)((Base->MDATACTRL & I3C_MDATACTRL_TXCOUNT_MASK) >> I3C_MDATACTRL_TXCOUNT_SHIFT);

    while ((Master->BufferSize >= 2U) && (TxCount < MaxNumOfBytes))
    {
        if (Master->BufferSize > 2U)
        {
            Base->MWDATAH = ((uint32)Master->TxDataBuffer[1U] << 8UL) | (uint32)(Master->TxDataBuffer[0U]);
        }
        else
        {
            Base->MWDATAHE = ((uint32)Master->TxDataBuffer[1U] << 8UL) | (uint32)(Master->TxDataBuffer[0U]);
        }

        Master->TxDataBuffer = &(Master->TxDataBuffer[2U]);
        Master->BufferSize -= 2U;

        TxCount = (uint8)((Base->MDATACTRL & I3C_MDATACTRL_TXCOUNT_MASK) >> I3C_MDATACTRL_TXCOUNT_SHIFT);
    }

    if (TxCount < I3C_IP_TX_FIFO_SIZE)
    {
        /* This ensures the following cases: when writing half-words, Tx Buffer size is an odd number of bytes,
           and this is the last byte to be sent, then writing into MWDATAHE will send an extra 0x00 byte. Instead,
           use MWDATABE to send the last byte. Usually, when half-word transfers are used, an even number of bytes shall be sent.
           Note that when in HDR-DDR, if an odd number of bytes is used, this leads to an extra 0x00 byte to be also sent
           (regardless of the transfer size in bytes, or in half-words) because HDR-DDR mode works with byte-pairs.
           Also, this treats the case when there are at least 2 more bytes to be sent, but there is not enough space in Tx FIFO for both.
           Then, write 1 byte to fill the Tx FIFO, and the rest is sent when Tx not full event fires. */
        if (Master->BufferSize > 1U)
        {
            Base->MWDATAB = (uint32)Master->TxDataBuffer[0U];
            Master->TxDataBuffer = &(Master->TxDataBuffer[1U]);
            Master->BufferSize -= 1U;
        }
        else if (1U == Master->BufferSize)
        {
            Base->MWDATABE = (uint32)Master->TxDataBuffer[0U];
            Master->BufferSize -= 1U;
        }
        else
        {
            /* Do nothing */
        }
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveWriteByteMessage
* Description   : Slave writes data to be sent, in bytes.
*
* END**************************************************************************/
static void I3c_Ip_SlaveWriteByteMessage(I3C_Type * const Base,
                                         I3c_Ip_SlaveStateType * const Slave)
{
    uint8 TxCount;

    TxCount = (uint8)((Base->SDATACTRL & I3C_SDATACTRL_TXCOUNT_MASK) >> I3C_SDATACTRL_TXCOUNT_SHIFT);

    /* Note that when in HDR-DDR, if an odd number of bytes is used, this leads to an extra 0x00 byte to be also sent
       (regardless of the transfer size in bytes, or in half-words) because HDR-DDR mode works with byte-pairs. */
    while ((Slave->BufferSize > 0U) && (TxCount < I3C_IP_TX_FIFO_SIZE))
    {
        if (Slave->BufferSize > 1U)
        {
            Base->SWDATAB = (uint32)Slave->DataBuffer[0U];

        }
        else
        {
            Base->SWDATABE = (uint32)Slave->DataBuffer[0U];
        }

        Slave->DataBuffer = &(Slave->DataBuffer[1U]);
        Slave->BufferSize -= 1U;

        TxCount = (uint8)((Base->SDATACTRL & I3C_SDATACTRL_TXCOUNT_MASK) >> I3C_SDATACTRL_TXCOUNT_SHIFT);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveWriteHalfWordMessage
* Description   : Slave writes data to be sent, in half-words.
*
* END**************************************************************************/
static void I3c_Ip_SlaveWriteHalfWordMessage(I3C_Type * const Base,
                                              I3c_Ip_SlaveStateType * const Slave)
{
    uint8 TxCount;

    /* MaxNumOfBytes ensures that the Tx FIFO is written only if there is room for both bytes. */
    const uint8 MaxNumOfBytes = I3C_IP_TX_FIFO_SIZE - 1U;

    TxCount = (uint8)((Base->SDATACTRL & I3C_SDATACTRL_TXCOUNT_MASK) >> I3C_SDATACTRL_TXCOUNT_SHIFT);

    while ((Slave->BufferSize >= 2U) && (TxCount < MaxNumOfBytes))
    {
        if (Slave->BufferSize > 2U)
        {
            Base->SWDATAH = ((uint32)Slave->DataBuffer[1U] << 8UL) | (uint32)(Slave->DataBuffer[0U]);

        }
        else
        {
            Base->SWDATAHE = ((uint32)Slave->DataBuffer[1U] << 8UL) | (uint32)(Slave->DataBuffer[0U]);
        }

        Slave->DataBuffer = &(Slave->DataBuffer[2U]);
        Slave->BufferSize -= 2U;

        TxCount = (uint8)((Base->SDATACTRL & I3C_SDATACTRL_TXCOUNT_MASK) >> I3C_SDATACTRL_TXCOUNT_SHIFT);
    }

    if (TxCount < I3C_IP_TX_FIFO_SIZE)
    {
        /* This ensures the following cases: when writing half-words, Tx Buffer size is an odd number of bytes,
        and this is the last byte to be sent, then writing into SWDATAHE will send an extra 0x00 byte. Instead,
        use SWDATABE to send the last byte. Usually, when half-word transfers are used, an even number of bytes shall be sent.
        Note that when in HDR-DDR, if an odd number of bytes is used, this leads to an extra 0x00 byte to be also sent
        (regardless of the transfer size in bytes, or in half-words) because HDR-DDR mode works with byte-pairs.
        Also, this treats the case when there are at least 2 more bytes to be sent, but there is not enough space in Tx FIFO for both.
        Then, write 1 byte to fill the Tx FIFO, and the rest is sent when Tx not full event fires. */
        if (Slave->BufferSize > 1U)
        {
            Base->SWDATAB = (uint32)Slave->DataBuffer[0U];
            Slave->DataBuffer = &(Slave->DataBuffer[1U]);
            Slave->BufferSize -= 1U;
        }
        else if (1U == Slave->BufferSize)
        {
            Base->SWDATABE = (uint32)Slave->DataBuffer[0U];
            Slave->BufferSize -= 1U;
        }
        else
        {
            /* Do nothing */
        }
    }
}

#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_AssignDA
* Description   : Assign Dynamic Addresses to all Slave devices on the bus.
*
* END**************************************************************************/
static I3c_Ip_StatusType I3c_Ip_AssignDA(const uint8 Instance,
                                         const uint8 * const DynamicAddressList,
                                         I3c_Ip_SlaveDeviceType * const SlaveDeviceList,
                                         uint8 DynamicAddressCount)
{
    I3c_Ip_StatusType ReturnStatus;
    I3C_Type * Base;
    uint8 SlaveIndex = 0U;

    uint32 CurrentTicks = 0U;
    uint32 ElapsedTicks = 0U;
    uint32 TimeoutTicks = OsIf_MicrosToTicks(I3C_IP_TIMEOUT_VALUE, I3C_IP_TIMEOUT_TYPE);

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];

    CurrentTicks = OsIf_GetCounter(I3C_IP_TIMEOUT_TYPE);
    do
    {
        /* Send the DA to the slave whose PID has just been read */
        ReturnStatus = I3c_Ip_MasterSendDAARequest(Instance, DynamicAddressList, SlaveDeviceList, &SlaveIndex);

        ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, I3C_IP_TIMEOUT_TYPE);

    } while (((Base->MSTATUS & (uint32)I3C_MSTATUS_COMPLETE_MASK) == 0U) &&
             ((Base->MSTATUS & (uint32)I3C_MSTATUS_STATE_MASK) != 0U)    &&
             (ElapsedTicks < TimeoutTicks)                               &&
             (SlaveIndex <= DynamicAddressCount));

    /* Clear complete status*/
    I3c_Ip_MasterClearStatus(Base, (uint32)I3C_MSTATUS_COMPLETE_MASK);

    if (ElapsedTicks >= TimeoutTicks)
    {
        ReturnStatus = I3C_IP_STATUS_TIMEOUT;
    }
    else if (SlaveIndex > DynamicAddressCount)
    {
        ReturnStatus = I3C_IP_STATUS_MAX_SLAVES_EXCEED;
    }
    else
    {
        /* Do nothing */
    }

    return ReturnStatus;
}
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES) */

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterInit
* Description   : This function initializes the I3C master module.
*
* @implements     I3c_Ip_MasterInit_Activity
* END**************************************************************************/
void I3c_Ip_MasterInit(const uint8 Instance,
                       const I3c_Ip_MasterConfigType * const Config)
{
    I3C_Type * Base;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Config != NULL_PTR);
    DevAssert(Instance < I3C_INSTANCE_COUNT);
    DevAssert(I3c_Ip_apxMasterState[Instance] == NULL_PTR);
#endif

    Config->MasterState->BufferSize = 0U;
    Config->MasterState->TxDataBuffer = NULL_PTR;
    Config->MasterState->RxDataBuffer = NULL_PTR;
    Config->MasterState->Status = I3C_IP_STATUS_SUCCESS;
    Config->MasterState->TransferOption.SlaveAddress = 0x00U;
    Config->MasterState->TransferOption.SendStop = TRUE;
    Config->MasterState->TransferOption.Direction = I3C_IP_WRITE;
    Config->MasterState->TransferOption.TransferSize = I3C_IP_TRANSFER_BYTES;
    Config->MasterState->TransferOption.BusType = I3C_IP_BUS_TYPE_I2C;

    Base = I3c_Ip_apxBase[Instance];
    I3c_Ip_apxMasterState[Instance] = Config->MasterState;

#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
    /* I3c_Ip_MasterInit() is also called for a master-capable device
       in order to configure the master-related parameters. Store initial Hw Unit mode
       only for the main master. */
    if (I3C_IP_MASTER_ON == Config->MasterEnable)
    {
        I3c_Ip_aeChannelCurrentMode[Instance] = I3C_IP_MASTER_MODE;
    }
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES) */

    /* Reset all registers to default values */
    I3c_Ip_MasterReset(Base);

    /* Set the initial configuration of the I3C master */
    I3c_Ip_MasterConfigInit(Base, Config);

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
    /* Configure In-Band interrupts rules */
    I3c_Ip_MasterConfigIbiRules(Base, Config);
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

    /* Set the initial baud rates, if the master is idle */
    if (0U == (Base->MSTATUS & (uint32)I3C_MSTATUS_STATE_MASK))
    {
        I3c_Ip_MasterInitBaudRate(Base, Config->I2cBaud, Config->OpenDrainBaud, Config->PushPullBaud);
    }

    /* Enable Slave Requests and Error events */
    I3c_Ip_MasterEnableInterrupts(Base, I3C_MINTSET_IBIWON_MASK    |
                                        I3C_MINTSET_SLVSTART_MASK  |
                                        I3C_MINTSET_NOWMASTER_MASK |
                                        I3C_MINTSET_ERRWARN_MASK);

    /* Enable the master module in the configured mode: MASTER_ON / MASTER_OFF */
    I3c_Ip_MasterEnable(Base, Config->MasterEnable);

#if (STD_ON == I3C_IP_ENABLE_DAA_INIT_TIME)
    (void)I3c_Ip_MasterProcessDAA(Instance, Config->MasterState->DynamicAddressList, Config->MasterState->DynamicAddressCount, NULL_PTR);
#endif /* (STD_OFF == I3C_IP_ENABLE_DAA_INIT_TIME) */
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterDeInit
* Description   : This function deinitializes the master module. The driver can't be used until initialized again.
*
* @implements     I3c_Ip_MasterDeInit_Activity
* END**************************************************************************/
void I3c_Ip_MasterDeInit(const uint8 Instance)
{
    I3C_Type * Base;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];
    const I3c_Ip_MasterStateType * Master = I3c_Ip_apxMasterState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Master != NULL_PTR);
#endif

    /* Reset all registers to default values */
    I3c_Ip_MasterReset(Base);

#if (STD_ON == I3C_IP_SELF_RESET_SUPPORT)
    Base->SELFRESET = (Base->SELFRESET & (~(I3C_SELFRESET_KEY_MASK | I3C_SELFRESET_RST_MASK))) | I3C_SELFRESET_KEY(I3C_IP_SELF_RESET_KEY) | I3C_SELFRESET_RST(1U);
#endif

    I3c_Ip_apxMasterState[Instance] = NULL_PTR;

    /* Disable the master module */
    I3c_Ip_MasterEnable(Base, I3C_IP_MASTER_OFF);

    (void)Master;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveInit
* Description   : This function initializes the I3C slave module.
*
* @implements     I3c_Ip_SlaveInit_Activity
* END**************************************************************************/
void I3c_Ip_SlaveInit(const uint8 Instance,
                      const I3c_Ip_SlaveConfigType * const Config)
{
    I3C_Type * Base;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Config != NULL_PTR);
    DevAssert(Instance < I3C_INSTANCE_COUNT);
    DevAssert(I3c_Ip_apxSlaveState[Instance] == NULL_PTR);
#endif

    Config->SlaveState->BufferSize = 0U;
    Config->SlaveState->DataBuffer = NULL_PTR;
    Config->SlaveState->Status = I3C_IP_STATUS_SUCCESS;
    Config->SlaveState->RepeatedStarts = 0U;

    Base = I3c_Ip_apxBase[Instance];
    I3c_Ip_apxSlaveState[Instance] = Config -> SlaveState;

#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
    /* Store initial Hw Unit mode. This should be done only for the secondary master
       in order to not overwrite the current mode for the main master that might share the mastership.
       Note that I3c_Ip_SlaveInit() is also called for the main master that might share the mastership,
       but in that case, SlaveEnable = FALSE. */
    if ((TRUE == Config->MasterCapableDevice) && (TRUE == Config->SlaveEnable))
    {
        I3c_Ip_aeChannelCurrentMode[Instance] = I3C_IP_MASTER_CAPABLE_MODE;
    }
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES) */

    /* Reset all registers to default values */
    I3c_Ip_SlaveReset(Base);

    /* Set the initial configuration of the I3C slave */
    I3c_Ip_SlaveConfigInit(Base, Config);

    /* If current platform has Self Reset support, configure Slave reset rules */
#if (STD_ON == I3C_IP_SLAVE_RESET_SUPPORT)
    Base->SRSTACTTIME &= ~(I3C_SRSTACTTIME_PERRSTTIM_MASK   |
                           I3C_SRSTACTTIME_SYSRSTTIM_MASK   |
                           I3C_SRSTACTTIME_CUSRSTTIM_MASK);
    Base->SRSTACTTIME |= I3C_SRSTACTTIME_PERRSTTIM(Config->I3cResetTimeRecover)   |
                         I3C_SRSTACTTIME_SYSRSTTIM(Config->ChipResetTimeRecover)  |
                         I3C_SRSTACTTIME_CUSRSTTIM(Config->CustomTimeRecover);
#endif /* (STD_ON == I3C_IP_SLAVE_RESET_SUPPORT) */

    /* Set the Provisional Id of the I3C slave */
    I3c_Ip_SlaveConfigProvisionalId(Base, Config);

    /* Set status to be returned by GETSTATUS CCC */
    I3c_Ip_SlaveConfigStatusActivities(Base, Config);

    /* Configure Index of Dynamic Address that IBI is for */
    Base->SCTRL = (Base->SCTRL & (~I3C_SCTRL_MAPIDX_MASK)) | I3C_SCTRL_MAPIDX(Config->MapIndex);

    /* Set the slave's 7-bit static addrress */
    I3c_Ip_SlaveSetStaticAddr(Base, Config->StaticAddress);

    /* Set up the interrupts */
    I3c_Ip_SlaveEnableInterrupts(Base, ((uint32)I3C_SINTSET_START_MASK      |
                                                I3C_SINTSET_MATCHED_MASK    |
                                                I3C_SINTSET_STOP_MASK       |
                                                I3C_SINTSET_DACHG_MASK      |
                                                I3C_SINTSET_CCC_MASK        |
                                                I3C_SINTSET_ERRWARN_MASK    |
                                                I3C_SINTSET_DDRMATCHED_MASK |
                                                I3C_SINTSET_CHANDLED_MASK   |
                                                I3C_SINTSET_EVENT_MASK));

    /* After the Slave device has been initialized as a Hot-Join device,
       I3C_IP_SLAVE_EVENT_REQUEST callback can be used to check when the request was sent.
       In addition, SSTATUS[EVDET] can be checked to see Master's decision regarding the request
       (ACK or NACK). SSTATUS[EVDET] can be also checked to see if the master hasn't issued a START yet,
       or if the Slave device is still waiting for Bus-Idle condition. */
#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
    if (TRUE == Config->HotJoinDevice)
    {
        /* Hot-Join request */
        Base->SCTRL = (Base->SCTRL & (~I3C_SCTRL_EVENT_MASK)) | I3C_SCTRL_EVENT(I3C_IP_SLAVE_HOT_JOIN_REQUEST);
    }
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

    I3c_Ip_SlaveEnable(Base, Config->SlaveEnable);
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveDeInit
* Description   : This function deinitializes the slave module. The driver can't be used until initialized again.
*
* @implements     I3c_Ip_SlaveDeInit_Activity
* END**************************************************************************/
void I3c_Ip_SlaveDeInit(const uint8 Instance)
{
    I3C_Type * Base;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];
    const I3c_Ip_SlaveStateType * Slave = I3c_Ip_apxSlaveState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Slave != NULL_PTR);
#endif

    /* Reset all registers to default values */
    I3c_Ip_SlaveReset(Base);

#if (STD_ON == I3C_IP_SELF_RESET_SUPPORT)
    Base->SELFRESET = (Base->SELFRESET & (~(I3C_SELFRESET_KEY_MASK | I3C_SELFRESET_RST_MASK))) | I3C_SELFRESET_KEY(I3C_IP_SELF_RESET_KEY) | I3C_SELFRESET_RST(1U);
#endif

    I3c_Ip_apxSlaveState[Instance] = NULL_PTR;

    /* Disable slave */
    I3c_Ip_SlaveEnable(Base, FALSE);

    (void)Slave;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterSendBlocking
* Description   : This function is used to send a block of data to the configured slave's address.
*                 It only returns when the transfer is complete.
*
* @implements     I3c_Ip_MasterSendBlocking_Activity
* END**************************************************************************/
I3c_Ip_StatusType I3c_Ip_MasterSendBlocking(const uint8 Instance,
                                            const uint8 * TxBuffer,
                                            const uint32 TxBufferSize,
                                            const I3c_Ip_TransferConfigType * const TransferOption)
{
    I3C_Type * Base;
    I3c_Ip_MasterStateType * Master;
    I3c_Ip_StatusType ReturnStatus = I3C_IP_STATUS_BUSY;
    uint8 MasterState;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
    DevAssert(TxBufferSize > 0U);
    DevAssert(TxBuffer != NULL_PTR);
#endif
    Base = I3c_Ip_apxBase[Instance];
    Master = I3c_Ip_apxMasterState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Master != NULL_PTR);
#endif

    /* Clear master status flags */
    I3c_Ip_MasterClearAllStatusFlags(Base);

    /* Disable transfer related events */
    I3c_Ip_MasterDisableInterrupts(Base, ((uint32)I3C_MINTCLR_MCTRLDONE_MASK  |
                                                  I3C_MINTCLR_COMPLETE_MASK   |
                                                  I3C_MINTCLR_RXPEND_MASK     |
                                                  I3C_MINTCLR_TXNOTFULL_MASK));

    /* Get the current state of the master */
    MasterState = (uint8)((Base->MSTATUS & (uint32)I3C_MSTATUS_STATE_MASK) >> I3C_MSTATUS_STATE_SHIFT);

    /* A transfer shall be initiated using this function only in one of the following cases:
       the bus is idle, or a transfer is ongoing (meaning that this transfer comes after a Repeated Start),
       or this transfer comes after a Slave Request (the header emitted here will be subject to arbitration
       against the Slave Request address). */
    if ((MasterState == (uint8)I3C_IP_MASTER_MODE_IDLE)              ||
        (MasterState == (uint8)I3C_IP_MASTER_MODE_NORMAL_ACTIVE_SDR) ||
        (MasterState == (uint8)I3C_IP_MASTER_MODE_MSG_DDR)           ||
        (MasterState == (uint8)I3C_IP_MASTER_MODE_SLAVE_REQUEST))
    {
        Master->BufferSize = TxBufferSize;
        Master->TxDataBuffer = TxBuffer;
        Master->TransferOption.SlaveAddress = TransferOption->SlaveAddress;
        Master->TransferOption.SendStop = TransferOption->SendStop;
        Master->TransferOption.Direction = TransferOption->Direction;
        Master->TransferOption.TransferSize = TransferOption->TransferSize;
        Master->TransferOption.BusType = TransferOption->BusType;
        Master->Status = I3C_IP_STATUS_BUSY;

        I3c_Ip_MasterResetTxFIFO(Base);

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
        if (Master->TransferType == I3C_IP_USING_DMA)
        {
            I3c_Ip_MasterTxDmaConfig(Base, Master);
        }
        else
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */
        {
            /* If Slave Address is not the broadcast address, pre-fill FIFO. The broadcast address is used as Slave's address
               only for a broadcast command or when using Option 2 for Sending/Receiving I3c messages.
               When so, RM specifies that Tx FIFO must not be pre-written.
               For more details, please refer to RM 69.3.2.2, Step 2: Write the MCTRL register */
            if (Master->TransferOption.SlaveAddress != I3C_IP_BROADCAST_ADDRESS)
            {
                I3c_Ip_MasterTxConfig(Instance);
            }
        }

        /* Initialize send transfer: Configure IBI response, BusType, and Emit START + Address + Write */
        I3c_Ip_MasterInitTransfer(Base, Master);

        ReturnStatus = I3c_Ip_MasterSendMessageBlocking(Base, Master);

        /* During a blocking transfer, Master->Status can be changed only by an error on the I3c bus, when Master->Status
           will be I3C_IP_STATUS_ERROR, or by a Slave Request that won the header arbitration, which will lead to the current
           blocking transfer to timeout. If Master->Status is changed in I3c_Ip_MasterSendBlocking(),
           ReturnStatus should reflect that status. Otherwise, Master->Status should be updated to reflect the transfer status. */
        if (I3C_IP_STATUS_ERROR == Master->Status)
        {
            ReturnStatus = Master->Status;
        }
        else
        {
            Master->Status = ReturnStatus;
        }
    }

    return ReturnStatus;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterSend
* Description   : This function is used to send a block of data to the configured slave's address.
*                 It returns immediately. The rest of the transfer is handled by the interrupt service routine.
*                 Use I3c_Ip_MasterGetTransferStatus() to check the progress of the transfer.
*
* @implements     I3c_Ip_MasterSend_Activity
* END**************************************************************************/
I3c_Ip_StatusType I3c_Ip_MasterSend(const uint8 Instance,
                                    const uint8 * TxBuffer,
                                    const uint32 TxBufferSize,
                                    const I3c_Ip_TransferConfigType * const TransferOption)
{
    I3C_Type * Base;
    I3c_Ip_MasterStateType * Master;
    uint8 MasterState;
    I3c_Ip_StatusType ReturnStatus;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
    DevAssert(TxBufferSize > 0U);
    DevAssert(TxBuffer != NULL_PTR);
#endif
    Base = I3c_Ip_apxBase[Instance];
    Master = I3c_Ip_apxMasterState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Master != NULL_PTR);
#endif

    /* Clear master status flags */
    I3c_Ip_MasterClearAllStatusFlags(Base);

    /* Get the current state of the master */
    MasterState = (uint8)((Base->MSTATUS & (uint32)I3C_MSTATUS_STATE_MASK) >> I3C_MSTATUS_STATE_SHIFT);

    /* A transfer shall be initiated using this function only in one of the following cases:
       the bus is idle, or a transfer is ongoing (meaning that this transfer comes after a Repeated Start),
       or this transfer comes after a Slave Request (the header emitted here will be subject to arbitration
       against the Slave Request address). */
    if ((MasterState == (uint8)I3C_IP_MASTER_MODE_IDLE)              ||
        (MasterState == (uint8)I3C_IP_MASTER_MODE_NORMAL_ACTIVE_SDR) ||
        (MasterState == (uint8)I3C_IP_MASTER_MODE_MSG_DDR)           ||
        (MasterState == (uint8)I3C_IP_MASTER_MODE_SLAVE_REQUEST))
    {
        Master->BufferSize = TxBufferSize;
        Master->TxDataBuffer = TxBuffer;
        Master->TransferOption.SlaveAddress = TransferOption->SlaveAddress;
        Master->TransferOption.SendStop = TransferOption->SendStop;
        Master->TransferOption.Direction = TransferOption->Direction;
        Master->TransferOption.TransferSize = TransferOption->TransferSize;
        Master->TransferOption.BusType = TransferOption->BusType;
        Master->Status = I3C_IP_STATUS_BUSY;

        /* Exit critical section */
        SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_07();

        I3c_Ip_MasterResetTxFIFO(Base);

        /* Exit critical section */
        SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_07();

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
        if (I3C_IP_USING_DMA == Master->TransferType)
        {
            I3c_Ip_MasterTxDmaConfig(Base, Master);
        }
        else
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */
        {
            /* If Slave Address is not the broadcast address, pre-fill FIFO. The broadcast address is used as Slave's address
               only for a broadcast command or when using Option 2 for Sending/Receiving I3c messages.
               When so, RM specifies that Tx FIFO must not be pre-written.
               For more details, please refer to RM 69.3.2.2, Step 2: Write the MCTRL register */
            if (Master->TransferOption.SlaveAddress != I3C_IP_BROADCAST_ADDRESS)
            {
                I3c_Ip_MasterTxConfig(Instance);

                if (Master->BufferSize > 0U)
                {
                    /* If more data to transfer, Tx interrupt is used */
                    I3c_Ip_MasterEnableInterrupts(Base, I3C_MINTSET_TXNOTFULL_MASK);
                }
            }
        }

        /* Initialize send transfer: Configure IBI response, BusType, and Emit START + Address + Write. */
        I3c_Ip_MasterInitTransfer(Base, Master);

        /* Enable complete event to signal when the transfer has completed */
        I3c_Ip_MasterEnableInterrupts(Base, I3C_MINTSET_COMPLETE_MASK);

        if (I3C_IP_BROADCAST_ADDRESS == Master->TransferOption.SlaveAddress)
        {
            /* Enter critical section */
            SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_07();

            /* Set Tx FIFO watermark */
            I3c_Ip_MasterSetTxFifoWatermark(Base, I3C_IP_FIFO_TRIGGER_ON_ALMOST_FULL);

            /* Exit critical section */
            SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_07();

            /* Enable Tx not full event */
            I3c_Ip_MasterEnableInterrupts(Base, I3C_MINTSET_TXNOTFULL_MASK);
        }

        ReturnStatus = I3C_IP_STATUS_SUCCESS;
    }
    else
    {
        ReturnStatus = I3C_IP_STATUS_ERROR;
    }

    return ReturnStatus;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterReceiveBlocking
* Description   : This function is used to receive a block of data from the configured slave's address.
*                 It only returns when the transfer is complete.
*
* @implements     I3c_Ip_MasterReceiveBlocking_Activity
* END**************************************************************************/
I3c_Ip_StatusType I3c_Ip_MasterReceiveBlocking(const uint8 Instance,
                                               uint8 * RxBuffer,
                                               const uint32 RxBufferSize,
                                               const I3c_Ip_TransferConfigType * const TransferOption)
{
    I3C_Type * Base;
    I3c_Ip_MasterStateType * Master;
    I3c_Ip_StatusType ReturnStatus = I3C_IP_STATUS_BUSY;
    uint8 MasterState;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
    DevAssert(RxBufferSize > 0U);
    DevAssert(RxBuffer != NULL_PTR);
#endif
    Base = I3c_Ip_apxBase[Instance];
    Master = I3c_Ip_apxMasterState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Master != NULL_PTR);
#endif

    /* Clear master status flags */
    I3c_Ip_MasterClearAllStatusFlags(Base);

    /* Disable transfer related events */
    I3c_Ip_MasterDisableInterrupts(Base, ((uint32)I3C_MINTCLR_MCTRLDONE_MASK  |
                                                  I3C_MINTCLR_COMPLETE_MASK   |
                                                  I3C_MINTCLR_RXPEND_MASK     |
                                                  I3C_MINTCLR_TXNOTFULL_MASK));

    /* Get the current state of the master */
    MasterState = (uint8)((Base->MSTATUS & (uint32)I3C_MSTATUS_STATE_MASK) >> I3C_MSTATUS_STATE_SHIFT);

    /* A transfer shall be initiated using this function only in one of the following cases:
       the bus is idle, or a transfer is ongoing (meaning that this transfer comes after a Repeated Start),
       or this transfer comes after a Slave Request (the header emitted here will be subject to arbitration
       against the Slave Request address). */
    if ((MasterState == (uint8)I3C_IP_MASTER_MODE_IDLE)              ||
        (MasterState == (uint8)I3C_IP_MASTER_MODE_NORMAL_ACTIVE_SDR) ||
        (MasterState == (uint8)I3C_IP_MASTER_MODE_MSG_DDR)           ||
        (MasterState == (uint8)I3C_IP_MASTER_MODE_SLAVE_REQUEST))
    {
        Master->BufferSize = RxBufferSize;
        Master->RxDataBuffer = RxBuffer;
        Master->TransferOption.SlaveAddress = TransferOption->SlaveAddress;
        Master->TransferOption.SendStop = TransferOption->SendStop;
        Master->TransferOption.Direction = TransferOption->Direction;
        Master->TransferOption.TransferSize = TransferOption->TransferSize;
        Master->TransferOption.BusType = TransferOption->BusType;
        Master->Status = I3C_IP_STATUS_BUSY;

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
        if (Master->TransferType == I3C_IP_USING_DMA)
        {
            I3c_Ip_MasterRxDmaConfig(Base, Master);
        }
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */

        /* If HDR-DDR mode, write the command code */
        I3c_Ip_MasterCheckWriteHdrDdrCommandCode(Base, Master);

        /* Enter critical section */
        SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_08();

        /* Set Rx FIFO watermark */
        I3c_Ip_MasterSetRxFifoWatermark(Base, I3C_IP_FIFO_TRIGGER_ON_EMPTY);

        /* Exit critical section */
        SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_08();

        /* Initialize receive transfer: Configure IBI response, BusType, and Emit START + Address + Read */
        I3c_Ip_MasterInitTransfer(Base, Master);

        ReturnStatus = I3c_Ip_MasterReceiveMessageBlocking(Base, Master);

        /* During a blocking transfer, Master->Status can be changed only by an error on the I3c bus, when Master->Status
           will be I3C_IP_STATUS_ERROR, or by a Slave Request that won the header arbitration, which will lead to the current
           blocking transfer to timeout. If Master->Status is changed in I3c_Ip_MasterReceiveBlocking(),
           ReturnStatus should reflect that status. Otherwise, Master->Status should be updated to reflect the transfer status. */
        if (I3C_IP_STATUS_ERROR == Master->Status)
        {
            ReturnStatus = Master->Status;
        }
        else
        {
            Master->Status = ReturnStatus;
        }
    }

    return ReturnStatus;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterReceive
* Description   : This function is used to receive a block of data from the configured slave's address.
*                 It returns immediately. The rest of the transfer is handled by the interrupt service routine.
*                 Use I3c_Ip_MasterGetTransferStatus() to check the progress of the transfer.
*
* @implements     I3c_Ip_MasterReceive_Activity
* END**************************************************************************/
I3c_Ip_StatusType I3c_Ip_MasterReceive(const uint8 Instance,
                                       uint8 * RxBuffer,
                                       const uint32 RxBufferSize,
                                       const I3c_Ip_TransferConfigType * const TransferOption)
{
    I3C_Type * Base;
    I3c_Ip_MasterStateType * Master;
    I3c_Ip_FifoTriggerLevelType RxFifoWatermark;
    uint8 MasterState;
    I3c_Ip_StatusType ReturnStatus;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
    DevAssert(RxBufferSize > 0U);
    DevAssert(RxBuffer != NULL_PTR);
#endif
    Base = I3c_Ip_apxBase[Instance];
    Master = I3c_Ip_apxMasterState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Master != NULL_PTR);
#endif

    /* Clear master status flags */
    I3c_Ip_MasterClearAllStatusFlags(Base);

    /* Get the current state of the master */
    MasterState = (uint8)((Base->MSTATUS & (uint32)I3C_MSTATUS_STATE_MASK) >> I3C_MSTATUS_STATE_SHIFT);

    /* A transfer shall be initiated using this function only in one of the following cases:
       the bus is idle, or a transfer is ongoing (meaning that this transfer comes after a Repeated Start),
       or this transfer comes after a Slave Request (the header emitted here will be subject to arbitration
       against the Slave Request address). */
    if ((MasterState == (uint8)I3C_IP_MASTER_MODE_IDLE)              ||
        (MasterState == (uint8)I3C_IP_MASTER_MODE_NORMAL_ACTIVE_SDR) ||
        (MasterState == (uint8)I3C_IP_MASTER_MODE_MSG_DDR)           ||
        (MasterState == (uint8)I3C_IP_MASTER_MODE_SLAVE_REQUEST))
    {
        Master->BufferSize = RxBufferSize;
        Master->RxDataBuffer = RxBuffer;
        Master->TransferOption.SlaveAddress = TransferOption->SlaveAddress;
        Master->TransferOption.SendStop = TransferOption->SendStop;
        Master->TransferOption.Direction = TransferOption->Direction;
        Master->TransferOption.TransferSize = TransferOption->TransferSize;
        Master->TransferOption.BusType = TransferOption->BusType;
        Master->Status = I3C_IP_STATUS_BUSY;

        /* Set Rx FIFO watermark */
        RxFifoWatermark = I3c_Ip_MasterConvertRxSizeToWatermark(Master);

        /* Enter critical section */
        SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_09();

        I3c_Ip_MasterSetRxFifoWatermark(Base, RxFifoWatermark);

        /* Exit critical section */
        SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_09();

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
        if (I3C_IP_USING_DMA == Master->TransferType)
        {
            I3c_Ip_MasterRxDmaConfig(Base, Master);
        }
        else
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */
        {
            I3c_Ip_MasterEnableInterrupts(Base, I3C_MINTSET_RXPEND_MASK);
        }

        /* If HDR-DDR mode, write the command code */
        I3c_Ip_MasterCheckWriteHdrDdrCommandCode(Base, Master);

        /* Initialize receive transfer: Configure IBI response, BusType, and Emit START + Address + Read */
        I3c_Ip_MasterInitTransfer(Base, Master);

        /* Enable complete event to signal when the transfer has completed */
        I3c_Ip_MasterEnableInterrupts(Base, I3C_MINTSET_COMPLETE_MASK);

        ReturnStatus = I3C_IP_STATUS_SUCCESS;
    }
    else
    {
        ReturnStatus = I3C_IP_STATUS_ERROR;

    }

    return ReturnStatus;
}

#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterProcessDAA
* Description   : This function is used to emit ENTDAA command and assign dynamic addresses to the slaves which need one.
*                 It shall be used either at bus initialization, or after one or more devices have been added to the I3C bus (hot-join devices).
*                 Only the slaves with no dynamic address will be assigned one.
*
* @implements     I3c_Ip_MasterProcessDAA_Activity
* END**************************************************************************/
I3c_Ip_StatusType I3c_Ip_MasterProcessDAA(const uint8 Instance,
                                          const uint8 * const AddressList,
                                          const uint8 AddressCount,
                                          I3c_Ip_SlaveDeviceType * const SlaveDeviceList)
{
    I3C_Type * Base;
    I3c_Ip_StatusType ReturnStatus = I3C_IP_STATUS_BUSY;
    I3c_Ip_MasterStateType * Master;
    const uint8 * DynamicAddressList;
    uint8 DynamicAddressCount;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];
    Master = I3c_Ip_apxMasterState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Master != NULL_PTR);
#endif

    /* If AddressList is NULL_PTR, then address list from configurator is used. */
    if (AddressList != NULL_PTR)
    {
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
        DevAssert(AddressCount < I3C_IP_MAX_DEVICES);
#endif
        DynamicAddressList = AddressList;
        DynamicAddressCount = AddressCount;
    }
    else
    {
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
        DevAssert(Master->DynamicAddressList != NULL_PTR);
#endif
        DynamicAddressList = Master->DynamicAddressList;
        DynamicAddressCount = Master->DynamicAddressCount;
    }

    /* Perform DAA only if the master is not busy */
    if (0U == (Base->MSTATUS & (uint32)I3C_MSTATUS_STATE_MASK))
    {
        Master->Status = I3C_IP_STATUS_BUSY;

        /* Clear master status flags */
        I3c_Ip_MasterClearAllStatusFlags(Base);

        /* Enter critical section */
        SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_00();

        /* Set the Rx watermark. Rx pending interrupt is used to read the 48-bit ID, BCR, and DCR concateneted, namely 8 bytes per slave. */
        I3c_Ip_MasterSetRxFifoWatermark(Base, I3C_IP_FIFO_TRIGGER_ON_ONE_HALF);

        /* In address header, the first I3C_IP_BROADCAST_ADDRESS should meet an open drain timing.
           For this, MCONFIG[ODHPP] = 0 and ODBAUD are used to ensure at least 200 ns for the Open-Drain half-period.
           Following a Repeated START or a STOP, after the first I3C_IP_BROADCAST_ADDRESS, all subsequent messages should use MCONFIG[ODHPP] = 1. */
        Base->MCONFIG &= ~I3C_MCONFIG_ODHPP_MASK;
        Base->MCONFIG |= I3C_MCONFIG_ODHPP(0U);


        /* Initiate Dynamic Address Assignment process */
        I3c_Ip_MasterEmitRequest(Base, I3C_IP_REQUEST_PROCESS_DAA);

        /* Change to MCONFIG[ODHPP] = 1 for all subsequent messages following START + I3C_IP_BROADCAST_ADDRESS + W */
        Base->MCONFIG &= ~I3C_MCONFIG_ODHPP_MASK;
        Base->MCONFIG |= I3C_MCONFIG_ODHPP(1U);

        /* Exit critical section */
        SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_00();

        ReturnStatus = I3c_Ip_AssignDA(Instance, DynamicAddressList, SlaveDeviceList, DynamicAddressCount);

        Master->Status = ReturnStatus;
    }

    return ReturnStatus;
}
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterGetTransferStatus
* Description   : Return the current status of the I3C master transfer.
*                 This function can be called during a non-blocking transmission to check the status of the transfer.
*                 In addition, if the transfer is still in progress, this functions returns the remaining bytes to be transferred,
*                 only if BytesRemaining pointer is different than NULL_PTR.
*
* @implements     I3c_Ip_MasterGetTransferStatus_Activity
* END**************************************************************************/
I3c_Ip_StatusType I3c_Ip_MasterGetTransferStatus(const uint8 Instance,
                                                 uint32 * const BytesRemaining)
{
    const I3c_Ip_MasterStateType * Master;
#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
    uint32 I3cDmaChn;
    uint32 I3cLastData;
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Master = I3c_Ip_apxMasterState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Master != NULL_PTR);
#endif

    if (BytesRemaining != NULL_PTR)
    {
#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
        if ((I3C_IP_USING_DMA == Master->TransferType) && (Master->BufferSize != 0U))
        {
            if (I3C_IP_WRITE == Master->TransferOption.Direction)
            {
                I3cDmaChn = Master->DmaTxChannel;
                I3cLastData = 1U;
            }
            else
            {
                I3cDmaChn = Master->DmaRxChannel;
                I3cLastData = 0U;
            }
            (void)Dma_Ip_GetLogicChannelParam(I3cDmaChn, DMA_IP_CH_GET_CURRENT_ITER_COUNT, BytesRemaining);
            if (I3C_IP_TRANSFER_HALF_WORDS == Master->TransferOption.TransferSize)
            {
                *BytesRemaining = ((*BytesRemaining + I3cLastData) << 1U);
            }
            else
            {
                *BytesRemaining += I3cLastData;
            }
        }
        else
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */
        {
            *BytesRemaining = Master->BufferSize;
        }
    }

    return Master->Status;
}

#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterWriteOwnDynamicAddress
* Description   : This function is used to set a Dynamic Address for the main master before handing off the mastership.
*                 This address should be written before switching to Slave mode and should not be changed once in Slave mode.
*
* @implements     I3c_Ip_MasterWriteOwnDynamicAddress_Activity
* END**************************************************************************/
void I3c_Ip_MasterWriteOwnDynamicAddress(const uint8 Instance,
                                         const uint8 Address,
                                         const boolean Valid)
{
    I3C_Type * Base;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];
    /* Enter critical section */
    SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_10();

    I3c_Ip_MasterWriteOwnDAFields(Base, Address, Valid);

    /* Exit critical section */
    SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_10();
}
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveGetTransferStatus
* Description   : Return the current status of the I3C slave transfer.
*                 This function can be called during a non-blocking transmission to check the status of the transfer.
*                 In addition, if the transfer is still in progress, this functions returns the remaining bytes to be transferred,
*                 only if BytesRemaining pointer is different than NULL_PTR.
*
* @implements     I3c_Ip_SlaveGetTransferStatus_Activity
* END**************************************************************************/
I3c_Ip_StatusType I3c_Ip_SlaveGetTransferStatus(const uint8 Instance,
                                                uint32 * const BytesRemaining)
{
    const I3c_Ip_SlaveStateType * Slave;
#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
    uint32 I3cDmaChn;
    uint8 I3cLastData;
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Slave = I3c_Ip_apxSlaveState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Slave != NULL_PTR);
#endif

    if (BytesRemaining != NULL_PTR)
    {
#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
        if ((I3C_IP_USING_DMA == Slave->TransferType) && (Slave->BufferSize != 0U))
        {
            if (I3C_IP_WRITE == Slave->Direction)
            {
                I3cDmaChn = Slave->DmaTxChannel;
                I3cLastData = 1U;
            }
            else
            {
                I3cDmaChn = Slave->DmaRxChannel;
                I3cLastData = 0U;
            }
            (void)Dma_Ip_GetLogicChannelParam(I3cDmaChn, DMA_IP_CH_GET_CURRENT_ITER_COUNT, BytesRemaining);
            if (I3C_IP_TRANSFER_HALF_WORDS == Slave->TransferSize)
            {
                *BytesRemaining = ((*BytesRemaining + I3cLastData) << 1U);
            }
            else
            {
                *BytesRemaining += I3cLastData;
            }
        }
        else
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */
        {
            *BytesRemaining = Slave->BufferSize;
        }
    }

    return Slave->Status;
}

#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveGetDynamicAddressAndCause
* Description   : This function is used to get the slave's dynamic address when it was assigned, re-assigned, or reset.
*                 It may be called from the DA Changed notification.
*
* @implements     I3c_Ip_SlaveGetDynamicAddressAndCause_Activity
* END**************************************************************************/
void I3c_Ip_SlaveGetDynamicAddressAndCause(const uint8 Instance,
                                           uint8 * const DynamicAddress,
                                           I3c_Ip_SlaveDACauseType * const Cause)
{
    const I3C_Type * Base;
    uint32 RegValue;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
    DevAssert(DynamicAddress != NULL_PTR);
    DevAssert(Cause != NULL_PTR);
#endif
    Base = I3c_Ip_apxBase[Instance];

    RegValue = Base->SMAPCTRL0;
    *DynamicAddress = (uint8)((RegValue & I3C_SMAPCTRL0_DA_MASK) >> I3C_SMAPCTRL0_DA_SHIFT);
    switch ((RegValue & I3C_SMAPCTRL0_CAUSE_MASK) >> I3C_SMAPCTRL0_CAUSE_SHIFT)
    {
        case 4U:
            *Cause = I3C_IP_SLAVE_DA_CAUSE_AUTOMAP;
            break;
        case 3U:
            *Cause = I3C_IP_SLAVE_DA_CAUSE_RSTDAA;
            break;
        case 2U:
            *Cause = I3C_IP_SLAVE_DA_CAUSE_SET;
            break;
        case 1U:
            *Cause = I3C_IP_SLAVE_DA_CAUSE_ENTDAA;
            break;
        default:
            *Cause = I3C_IP_SLAVE_DA_NOT_CONFIGURED;
            break;
    }
}
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveSetBuffer
* Description   : This function is used to set the slave's buffer for the transfer data. It also set the default RX FIFO watermark.
*                 It could be called from the user callback provided at initialization.
*
* @implements     I3c_Ip_SlaveSetBuffer_Activity
* END**************************************************************************/
void I3c_Ip_SlaveSetBuffer(const uint8 Instance,
                           uint8 * DataBuffer,
                           const uint8 BufferSize)
{
    I3C_Type * Base;
    I3c_Ip_SlaveStateType * Slave;
    I3c_Ip_FifoTriggerLevelType RxFifoWatermark;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Slave = I3c_Ip_apxSlaveState[Instance];
    Base = I3c_Ip_apxBase[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Slave != NULL_PTR);
#endif

    Slave->DataBuffer = DataBuffer;
    Slave->BufferSize = BufferSize;

    /* Set Rx FIFO default watermarks. It will change accordingly during runtime. */
    RxFifoWatermark = I3c_Ip_SlaveConvertRxSizeToWatermark(Base, Slave->BufferSize);

    /* Enter critical section */
    SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_11();

    I3c_Ip_SlaveSetRxFifoWatermark(Base, RxFifoWatermark);

    /* Exit critical section */
    SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_11();
}

#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveSetMapAddress
* Description   : This function is used to configure a slave at runtime, after it was the main master and handed off the mastership.
*                 Multiple mapped addresses are used when the slave device supports group addressing, thus they contain the addresses
*                 within the group address.
*
* @implements     I3c_Ip_SlaveSetMapAddress_Activity
* END**************************************************************************/
void I3c_Ip_SlaveSetMapAddress(const uint8 Instance,
                               const uint8 MapIndex,
                               I3c_Ip_SlaveMapAddressType MapAddressOptions)
{
    I3C_Type * Base;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
    /* SMAPCTRL0 is readonly, so MapIndex must not be 0 */
    DevAssert(MapIndex != 0U);
    DevAssert(MapIndex <= I3C_IP_MAX_MAP_ADDRESSES);
#if (STD_ON == I3C_IP_SA10B_SUPPORT)
    DevAssert(MapAddressOptions.SA10BitExtension <= I3C_IP_MAX_SA10B);
#endif /* (STD_ON == I3C_IP_SA10B_SUPPORT) */
#endif /* (STD_ON == I3C_IP_DEV_ERROR_DETECT) */
    Base = I3c_Ip_apxBase[Instance];

    /* Enter critical section */
    SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_12();

    I3c_Ip_SlaveSetMapAddressFields(Base, MapIndex, MapAddressOptions);

    /* Exit critical section */
    SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_12();
}
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterGetError
* Description   : This function is used to get the master's error register.
*                 This may be called from the Error/Warning notification and dedicated defines should be used
*                 to filter the errors/warnings which occurred.
*
* @implements     I3c_Ip_MasterGetError_Activity
* END**************************************************************************/
uint32 I3c_Ip_MasterGetError(const uint8 Instance)
{
    const I3C_Type * Base;
    uint32 RegValue;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];

    RegValue = Base->MERRWARN;
    return RegValue;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveGetError
* Description   : This function is used to get the slave's error register.
*                 This may be called from the Error/Warning notification and dedicated defines should be used
*                 to filter the errors/warnings which occurred.
*
* @implements     I3c_Ip_SlaveGetError_Activity
* END**************************************************************************/
uint32 I3c_Ip_SlaveGetError(const uint8 Instance)
{
    const I3C_Type * Base;
    uint32 RegValue;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];

    RegValue = Base->SERRWARN;
    return RegValue;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlavePrepareTxFifo
* Description   : This function is used to enable Slave TX interrupt.
*                 This is used to fill the Slave TX FIFO before master request.
*                 Note that before calling this function, I3c_Ip_SlaveSetBuffer() must be called.
*
* @implements     I3c_Ip_SlavePrepareTxFifo_Activity
* END**************************************************************************/
void I3c_Ip_SlavePrepareTxFifo(const uint8 Instance)
{
    I3C_Type * Base;
    I3c_Ip_SlaveStateType * Slave;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Slave = I3c_Ip_apxSlaveState[Instance];
    Base = I3c_Ip_apxBase[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Slave != NULL_PTR);
#endif

    Slave->Direction = I3C_IP_WRITE;
    Slave->Status = I3C_IP_STATUS_BUSY;

    /* Enter critical section */
    SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_13();

    I3c_Ip_SlaveResetTxFIFO(Base);

    /* Exit critical section */
    SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_13();

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
    if (I3C_IP_USING_DMA == Slave->TransferType)
    {
        /* Start slave DMA transfer */
        I3c_Ip_SlaveTxDmaConfig(Base, Slave);
    }
    else
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */
    {
        I3c_Ip_SlaveSendMessage(Base, Slave);

        if (Slave->BufferSize > 0U)
        {
            /* Enter critical section */
            SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_13();

            I3c_Ip_SlaveSetTxFifoWatermark(Base, I3C_IP_FIFO_TRIGGER_ON_ALMOST_FULL);

            /* Exit critical section */
            SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_13();

            I3c_Ip_SlaveEnableInterrupts(Base, (uint32)I3C_SINTSET_TXSEND_MASK);
        }
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_IRQHandler
* Description   : Handler for both slave and master operations when I3C interrupts occur.
*
* @implements     I3c_Ip_IRQHandler_Activity
* END**************************************************************************/
void I3c_Ip_IRQHandler(const uint8 Instance)
{
    I3C_Type * Base;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif

    Base = I3c_Ip_apxBase[Instance];

    /* If both master and slave states are initialized, then de module is either a master that
       might share the mastership, or a master-capable device (acting as a slave now).
       In both cases, since I3c has a single interrupt line, all events shall be checked. */
    if ((I3c_Ip_apxMasterState[Instance] != NULL_PTR) && (I3c_Ip_apxSlaveState[Instance] != NULL_PTR))
    {
        /* Check if the interrupt was spurious */
        if (Base->MINTMASKED != 0U)
        {
            I3c_Ip_MasterIRQHandler(Instance);
        }
        else if (Base->SINTMASKED != 0U)
        {
            I3c_Ip_SlaveIRQHandler(Instance);
        }
        else
        {
            /* Do nothing */
        }
    }
    else if (I3c_Ip_apxMasterState[Instance] != NULL_PTR)
    {
        /* Check if the interrupt was spurious */
        if (Base->MINTMASKED != 0U)
        {
            I3c_Ip_MasterIRQHandler(Instance);
        }
    }
    else if (I3c_Ip_apxSlaveState[Instance] != NULL_PTR)
    {
        /* Check if the interrupt was spurious */
        if (Base->SINTMASKED != 0U)
        {
            I3c_Ip_SlaveIRQHandler(Instance);
        }
    }
    else
    {
        /* If the module is not initialized, clear all interrupt status flags */
        I3c_Ip_MasterClearAllStatusFlags(Base);
        I3c_Ip_SlaveClearAllStatusFlags(Base);
    }
}

#if (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterDmaErrorHandler
* Description   : Handle Master DMA error.
*
* END**************************************************************************/
void I3c_Ip_MasterDmaErrorHandler(const uint8 Instance)
{
    I3C_Type * Base;
    I3c_Ip_MasterStateType * Master;
    Dma_Ip_LogicChannelStatusType DmaStatus;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];
    Master = I3c_Ip_apxMasterState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Master != NULL_PTR);
#endif

    if (Master->TransferOption.Direction == I3C_IP_WRITE)
    {
        (void)Dma_Ip_GetLogicChannelStatus(Master->DmaTxChannel, &DmaStatus);
    }
    else
    {
        (void)Dma_Ip_GetLogicChannelStatus(Master->DmaRxChannel, &DmaStatus);
    }

    if (DmaStatus.ChStateValue == DMA_IP_CH_ERROR_STATE)
    {
        I3c_Ip_MasterEndTransfer(Base, Master, FALSE, TRUE);
        Master->Status = I3C_IP_STATUS_ERROR;
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveDmaErrorHandler
* Description   : Handle Slave DMA error.
*
* END**************************************************************************/
void I3c_Ip_SlaveDmaErrorHandler(const uint8 Instance)
{
    I3C_Type * Base;
    I3c_Ip_SlaveStateType * Slave;
    Dma_Ip_LogicChannelStatusType DmaStatus;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];
    Slave = I3c_Ip_apxSlaveState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Slave != NULL_PTR);
#endif

    if (Slave->Direction == I3C_IP_WRITE)
    {
        (void)Dma_Ip_GetLogicChannelStatus(Slave->DmaTxChannel, &DmaStatus);
    }
    else
    {
        (void)Dma_Ip_GetLogicChannelStatus(Slave->DmaRxChannel, &DmaStatus);
    }

    if (DmaStatus.ChStateValue == DMA_IP_CH_ERROR_STATE)
    {
        I3c_Ip_SlaveEndTransfer(Base, Slave, FALSE);
        Slave->Status = I3C_IP_STATUS_ERROR;
    }
}
#endif /* (STD_ON == I3C_IP_DMA_FEATURE_AVAILABLE) */

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveRequestEvent
* Description   : This function is used to request an In-Band Interrupt, Hot-Join, or Master Request.
*                 After a Slave request has been initialized, I3C_IP_SLAVE_EVENT_REQUEST callback can be used to check when the request was sent.
*                 In addition, SSTATUS[EVDET] can be checked to see Master's decision regarding the request(ACK or NACK).
*                 SSTATUS[EVDET] can be also checked to see if the master hasn't issued a START yet,
*                 or if the Slave device is still waiting for Bus-Available or Bus-Idle(HJ) condition.
*
* @implements     I3c_Ip_SlaveRequestEvent_Activity
* END**************************************************************************/
void I3c_Ip_SlaveRequestEvent(const uint8 Instance,
                              const I3c_Ip_SlaveRequestType Event,
                              const uint8 IbiData,
                              const uint8 * ExtData)
{
    I3C_Type * Base;
    const I3c_Ip_SlaveStateType * Slave;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Slave = I3c_Ip_apxSlaveState[Instance];
    Base = I3c_Ip_apxBase[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Slave != NULL_PTR);
#endif

    switch (Event)
    {
        case I3C_IP_SLAVE_IBI_REQUEST:
        {
            I3c_Ip_SlaveRequestIbi(Instance, IbiData, ExtData);
            break;
        }
        case I3C_IP_SLAVE_MASTER_REQUEST:
        {
            /* TODO: implement for MR */
            break;
        }
        default:
        {
            /* Do nothing */
            break;
        }

    }

    /* Slave request */
    Base->SCTRL = (Base->SCTRL & (~I3C_SCTRL_EVENT_MASK)) | I3C_SCTRL_EVENT(Event);
}
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterSetBaudRate
* Description   : This function is used to set the baud rate for all subsequent I3C communications.
*                 Using this function, the baud rates will be set as close as possible to the target ones.
*                 After calling this function, I3c_Ip_MasterGetBaudRate() shall be called to check which baud rates were actually set.
*
* @implements     I3c_Ip_MasterSetBaudRate_Activity
* END**************************************************************************/
I3c_Ip_StatusType I3c_Ip_MasterSetBaudRate(const uint8 Instance,
                                           const uint32 FunctionalClock,
                                           const I3c_Ip_MasterBaudRateType * BaudRates,
                                           const I3c_Ip_BusType BusType)
{
    I3C_Type * Base;
    I3c_Ip_StatusType ReturnStatus = I3C_IP_STATUS_BUSY;
    uint8 PpBaudCount;
    uint8 OdBaudCount;
    uint8 I2cBaudCount;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];

    /* Baud rates can be set only if the master is idle */
    if ((Base->MSTATUS & (uint32)I3C_MSTATUS_STATE_MASK) == 0U)
    {
        PpBaudCount = I3c_Ip_ComputePpBaudRate(Base, FunctionalClock, BaudRates->PushPullBaudRate, BusType);
        OdBaudCount = I3c_Ip_ComputeOdBaudRate(Base, PpBaudCount, FunctionalClock, BaudRates->OpenDrainBaudRate);
        I2cBaudCount = I3c_Ip_ComputeI2cBaudRate(PpBaudCount, OdBaudCount, FunctionalClock, BaudRates->I2cBaudRate);

        /* Enter critical section */
        SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_14();

        Base->MCONFIG = (Base->MCONFIG & (~(I3C_MCONFIG_PPBAUD_MASK     |
                                            I3C_MCONFIG_ODBAUD_MASK     |
                                            I3C_MCONFIG_I2CBAUD_MASK))) |
                                            I3C_MCONFIG_PPBAUD(PpBaudCount)  |
                                            I3C_MCONFIG_ODBAUD(OdBaudCount)  |
                                            I3C_MCONFIG_I2CBAUD(I2cBaudCount);
        /* Exit critical section */
        SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_14();

        ReturnStatus = I3C_IP_STATUS_SUCCESS;
    }

    return ReturnStatus;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterGetBaudRate
* Description   : This function is used to get the baud rates of the I3C module.
*
* @implements     I3c_Ip_MasterGetBaudRate_Activity
* END**************************************************************************/
void I3c_Ip_MasterGetBaudRate(const uint8 Instance,
                              const uint32 FunctionalClock,
                              I3c_Ip_MasterBaudRateType * BaudRates)
{
    const I3C_Type * Base;
    uint8 PpBaudCount;
    uint8 OdBaudCount;
    uint8 I2cBaudCount;
    uint8 PushPullLow;
    boolean OdHpp;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];

    PpBaudCount = (uint8)((Base->MCONFIG & (uint32)I3C_MCONFIG_PPBAUD_MASK) >> I3C_MCONFIG_PPBAUD_SHIFT);
    OdBaudCount = (uint8)((Base->MCONFIG & (uint32)I3C_MCONFIG_ODBAUD_MASK) >> I3C_MCONFIG_ODBAUD_SHIFT);
    I2cBaudCount = (uint8)((Base->MCONFIG & (uint32)I3C_MCONFIG_I2CBAUD_MASK) >> I3C_MCONFIG_I2CBAUD_SHIFT);
    PushPullLow = (uint8)((Base->MCONFIG & (uint32)I3C_MCONFIG_PPLOW_MASK) >> I3C_MCONFIG_PPLOW_SHIFT);
    OdHpp = ((Base->MCONFIG & (uint32)I3C_MCONFIG_ODHPP_MASK) != 0U) ? TRUE : FALSE;

    /* Get push-pull frequency, taking into consideration PPLOW */
    BaudRates->PushPullBaudRate = FunctionalClock / (((PpBaudCount + 1UL) << 1UL) + PushPullLow);

    /* Calculated open-drain frequency when high open-drain period is the same as the low open-drain period */
    BaudRates->OpenDrainBaudRate = FunctionalClock / (((PpBaudCount + 1UL) << 1UL) * (OdBaudCount + 1UL));

    /* Get I2c frequency, using open-drain frequency when high open-drain period is the same as the low open-drain period */
    BaudRates->I2cBaudRate = (BaudRates->OpenDrainBaudRate << 1U) / (I2cBaudCount + 2U);

    /* Calculated open-drain frequency when high open-drain period is the same as the high push-pull period */
    if (TRUE == OdHpp)
    {
        BaudRates->OpenDrainBaudRate = FunctionalClock / (((PpBaudCount + 1UL) * (OdBaudCount + 1UL)) + (PpBaudCount + 1UL));
    }
}

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterSetIbiOptions
* Description   : This function is used to set IBI options for the following Slave Requests.
*                 If this function is not used, then default IBI options set in configurator will be used.
*                 Note that for In-Band interrupts with data bytes, this function must be used to set the buffer
*                 in which IBI data bytes will be stored.
*
* @implements     I3c_Ip_MasterSetIbiOptions_Activity
* END**************************************************************************/
void I3c_Ip_MasterSetIbiOptions(const uint8 Instance,
                                const I3c_Ip_MasterIbiOptionType * const IbiOptions)
{
    I3c_Ip_MasterStateType * Master;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Master = I3c_Ip_apxMasterState[Instance];
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Master != NULL_PTR);
#endif

    Master->IbiOptions.IbiSendStop   = IbiOptions->IbiSendStop;
    Master->IbiOptions.EmitAutoIbi   = IbiOptions->EmitAutoIbi;
    Master->IbiOptions.IbiDataBuffer = IbiOptions->IbiDataBuffer;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterManualIbiResponse
* Description   : This function is used by the master to take a decision (ACK/NACK) for an In-Band Interrupt which needs manual intervention.
*                 Note that for Hot-Join and Master-Request, this function must be always called in I3C_IP_MASTER_EVENT_HOT_JOIN and I3C_IP_MASTER_EVENT_MASTER_REQUEST
*                 callbacks.
*
* @implements     I3c_Ip_MasterManualIbiResponse_Activity
* END**************************************************************************/
void I3c_Ip_MasterManualIbiResponse(const uint8 Instance,
                                    const I3c_Ip_IbiResponseType IbiResponse)
{
    I3C_Type * Base;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];
     /* Enter critical section */
    SchM_Enter_I3c_I3C_EXCLUSIVE_AREA_15();

    Base->MCTRL = (Base->MCTRL & (~(I3C_MCTRL_REQUEST_MASK          |
                                    I3C_MCTRL_IBIRESP_MASK)))       |
                                    I3C_MCTRL_IBIRESP(IbiResponse)  |
                                    I3C_MCTRL_REQUEST(I3C_IP_REQUEST_IBI_ACK_NACK);
    /* Exit critical section */
    SchM_Exit_I3c_I3C_EXCLUSIVE_AREA_15();
}
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_MasterGetStatus
* Description   : This function is used to get the master's status register.
*                 Dedicated defines can be used to filter and check each status bit field.
*
* @implements     I3c_Ip_MasterGetStatus_Activity
* END**************************************************************************/
uint32 I3c_Ip_MasterGetStatus(const uint8 Instance)
{
    const I3C_Type * Base;
    uint32 RegValue;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];

    RegValue = Base->MSTATUS;
    return RegValue;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SlaveGetStatus
* Description   : This function is used to get the slave's status register.
*                 Dedicated defines can be used to filter and check each status bit field.
*
* @implements     I3c_Ip_SlaveGetStatus_Activity
* END**************************************************************************/
uint32 I3c_Ip_SlaveGetStatus(const uint8 Instance)
{
    const I3C_Type * Base;
    uint32 RegValue;
#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
#endif
    Base = I3c_Ip_apxBase[Instance];

    RegValue = Base->SSTATUS;
    return RegValue;
}

#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Ip_SetMasterCapableHwUnitMode
* Description   : This function should be used after the mastership handoff, for the Hw Unit that became a master-capable device.
*                 When a device is able to share the mastership, either to pass it or to gain it from another device,
*                 both master and slave configurations are used for the same module.
*                 The slave configuration will be used to reinitialize the module after the mastership was passed to another device,
*                 whilst the master configuration will be used to reinitialize the module after the mastership was gained from the main master device.
*                 This function is used to configure the slave-related parameters and to disable the master functionality for the new master-capable device (old main master).
*                 It is the user's responsability to make sure this function is used for the correct I3C Hw Unit that was involved in the mastership handoff.
*                 Configuration of the master-related parameters is done using Now Master interrupt handler.
*                 For a slave device that became master, there is no need to call any additional API, because the state update is handled automatically by the driver using the now master interrupt.
*
* @implements     I3c_Ip_SetMasterCapableHwUnitMode_Activity
* END**************************************************************************/
void I3c_Ip_SetMasterCapableHwUnitMode(const uint8 Instance)
{
    I3C_Type * Base;

#if (STD_ON == I3C_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < I3C_INSTANCE_COUNT);
    DevAssert(I3C_IP_MASTER_MODE == I3c_Ip_aeChannelCurrentMode[Instance]);
#endif
    Base = I3c_Ip_apxBase[Instance];

    I3c_Ip_aeChannelCurrentMode[Instance] = I3C_IP_MASTER_CAPABLE_MODE;

    /* Enable the module as master-capable device */
    I3c_Ip_MasterEnable(Base, I3C_IP_MASTER_CAPABLE);
    I3c_Ip_SlaveEnable(Base, TRUE);
}
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES) */


#define I3C_STOP_SEC_CODE
#include "I3c_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
