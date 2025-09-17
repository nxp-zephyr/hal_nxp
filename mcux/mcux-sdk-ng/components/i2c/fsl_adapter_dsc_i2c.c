/*
 * Copyright 2018 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_i2c.h"

#include "fsl_adapter_i2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief i2c master state structure. */
typedef struct _hal_i2c_master
{
    hal_i2c_master_transfer_callback_t callback;
    void *callbackParam;
    i2c_master_transfer_handle_t hardwareHandle;
    uint8_t instance;
} hal_i2c_master_t;

/*! @brief i2c slave state structure. */
typedef struct _hal_i2c_slave
{
    hal_i2c_slave_transfer_callback_t callback;
    void *callbackParam;
    hal_i2c_slave_transfer_t transfer;
    i2c_slave_transfer_handle_t hardwareHandle;
    uint8_t instance;
} hal_i2c_slave_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
hal_i2c_status_t HAL_I2cGetStatus(status_t status);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Pointers to i2c bases for each instance. */
static I2C_Type *const s_i2cBases[] = I2C_BASE_PTRS;

/*******************************************************************************
 * Code
 ******************************************************************************/

hal_i2c_status_t HAL_I2cGetStatus(status_t status)
{
    hal_i2c_status_t returnStatus;
    switch (status)
    {
        case kStatus_Success:
        {
            returnStatus = kStatus_HAL_I2cSuccess;
            break;
        }
        case kStatus_I2C_Busy:
        {
            returnStatus = kStatus_HAL_I2cBusy;
            break;
        }
        case kStatus_I2C_Idle:
        {
            returnStatus = kStatus_HAL_I2cIdle;
            break;
        }
        case kStatus_I2C_Nak:
        {
            returnStatus = kStatus_HAL_I2cNak;
            break;
        }
        case kStatus_I2C_ArbitrationLost:
        {
            returnStatus = kStatus_HAL_I2cArbitrationLost;
            break;
        }
        case kStatus_I2C_Timeout:
        {
            returnStatus = kStatus_HAL_I2cTimeout;
            break;
        }
        case kStatus_I2C_Addr_Nak:
        {
            returnStatus = kStatus_HAL_I2cAddrressNak;
            break;
        }
        default:
        {
            returnStatus = kStatus_HAL_I2cError;
            break;
        }
    }
    return returnStatus;
}

static void HAL_I2cMasterCallback(i2c_master_transfer_handle_t *psHandle)
{
    hal_i2c_master_t *i2cMasterHandle;
    assert(I2C_GET_TRANSFER_USER_DATA(psHandle));

    i2cMasterHandle = (hal_i2c_master_t *)I2C_GET_TRANSFER_USER_DATA(psHandle);

    if (NULL != i2cMasterHandle->callback)
    {
        i2cMasterHandle->callback(i2cMasterHandle, HAL_I2cGetStatus(I2C_GET_TRANSFER_COMPLETION_STATUS(psHandle)),
                                  i2cMasterHandle->callbackParam);
    }
}

static void HAL_I2cSlaveCallback(i2c_slave_transfer_handle_t *psHandle)
{
    hal_i2c_slave_t *i2cSlaveHandle;
    assert(I2C_GET_TRANSFER_USER_DATA(psHandle));

    i2cSlaveHandle = (hal_i2c_slave_t *)I2C_GET_TRANSFER_USER_DATA(psHandle);

    if (NULL != i2cSlaveHandle->callback)
    {
        i2cSlaveHandle->transfer.event    = (hal_i2c_slave_transfer_event_t)I2C_GET_SLAVE_TRANSFER_EVENT(psHandle);
        i2cSlaveHandle->transfer.data     = I2C_GET_SLAVE_TRANSFER_DATA_POINTER(psHandle);
        i2cSlaveHandle->transfer.dataSize = I2C_GET_SLAVE_TRANSFER_DATASIZE(psHandle);
        i2cSlaveHandle->transfer.completionStatus = HAL_I2cGetStatus(I2C_GET_TRANSFER_COMPLETION_STATUS(psHandle));
        i2cSlaveHandle->transfer.transferredCount = I2C_GET_SLAVE_TRANSFERRED_COUNT(psHandle);
        i2cSlaveHandle->callback(i2cSlaveHandle, &i2cSlaveHandle->transfer, i2cSlaveHandle->callbackParam);
        I2C_GET_SLAVE_TRANSFER_DATA_POINTER(psHandle) = i2cSlaveHandle->transfer.data;
        I2C_GET_SLAVE_TRANSFER_DATASIZE(psHandle)     = (uint16_t)(i2cSlaveHandle->transfer.dataSize);
    }
}

hal_i2c_status_t HAL_I2cMasterInit(hal_i2c_master_handle_t handle, const hal_i2c_master_config_t *halI2cConfig)
{
    hal_i2c_master_t *i2cMasterHandle;
    i2c_master_config_t i2cConfig;

    assert(handle);
    assert(halI2cConfig);
    assert(halI2cConfig->instance < ARRAY_SIZE(s_i2cBases));
    assert(HAL_I2C_MASTER_HANDLE_SIZE >= sizeof(hal_i2c_master_t));

    i2cMasterHandle = (hal_i2c_master_t *)handle;

    I2C_MasterGetDefaultConfig(&i2cConfig, halI2cConfig->srcClock_Hz);
    i2cConfig.bEnableModule   = halI2cConfig->enableMaster;
    i2cConfig.u32BaudRateBps  = halI2cConfig->baudRate_Bps;
    i2cMasterHandle->instance = halI2cConfig->instance;

    I2C_MasterInit(s_i2cBases[i2cMasterHandle->instance], &i2cConfig);

    return kStatus_HAL_I2cSuccess;
}

hal_i2c_status_t HAL_I2cSlaveInit(hal_i2c_slave_handle_t handle, const hal_i2c_slave_config_t *halI2cConfig)
{
    hal_i2c_slave_t *i2cSlaveHandle;
    i2c_slave_config_t i2cConfig;

    assert(handle);
    assert(halI2cConfig);
    assert(halI2cConfig->instance < ARRAY_SIZE(s_i2cBases));
    assert(HAL_I2C_SLAVE_HANDLE_SIZE >= sizeof(hal_i2c_slave_t));

    i2cSlaveHandle = (hal_i2c_slave_t *)handle;

    I2C_SlaveGetDefaultConfig(&i2cConfig, halI2cConfig->slaveAddress, halI2cConfig->srcClock_Hz);
    i2cConfig.bEnableModule  = halI2cConfig->enableSlave;
    i2cSlaveHandle->instance = halI2cConfig->instance;

    I2C_SlaveInit(s_i2cBases[i2cSlaveHandle->instance], &i2cConfig);

    return kStatus_HAL_I2cSuccess;
}

hal_i2c_status_t HAL_I2cMasterDeinit(hal_i2c_master_handle_t handle)
{
    hal_i2c_master_t *i2cMasterHandle;

    assert(handle);

    i2cMasterHandle = (hal_i2c_master_t *)handle;

    I2C_MasterDeinit(s_i2cBases[i2cMasterHandle->instance]);

    return kStatus_HAL_I2cSuccess;
}

hal_i2c_status_t HAL_I2cSlaveDeinit(hal_i2c_slave_handle_t handle)
{
    hal_i2c_slave_t *i2cSlaveHandle;

    assert(handle);

    i2cSlaveHandle = (hal_i2c_slave_t *)handle;

    I2C_SlaveDeinit(s_i2cBases[i2cSlaveHandle->instance]);

    return kStatus_HAL_I2cSuccess;
}

hal_i2c_status_t HAL_I2cMasterWriteBlocking(hal_i2c_master_handle_t handle,
                                            const uint8_t *txBuff,
                                            size_t txSize,
                                            uint32_t flags)
{
    hal_i2c_master_t *i2cMasterHandle;

    assert(handle);

    i2cMasterHandle = (hal_i2c_master_t *)handle;

    bool bSendStop = ((((uint32_t)kI2C_TransferNoStopFlag) & flags) == 0U);

    return HAL_I2cGetStatus(
        I2C_MasterWriteBlocking(s_i2cBases[i2cMasterHandle->instance], txBuff, (uint16_t)txSize, bSendStop));
}

hal_i2c_status_t HAL_I2cMasterReadBlocking(hal_i2c_master_handle_t handle,
                                           uint8_t *rxBuff,
                                           size_t rxSize,
                                           uint32_t flags)
{
    hal_i2c_master_t *i2cMasterHandle;

    assert(handle);

    i2cMasterHandle = (hal_i2c_master_t *)handle;

    bool bSendStop = ((((uint32_t)kI2C_TransferNoStopFlag) & flags) == 0U);

    return HAL_I2cGetStatus(
        I2C_MasterReadBlocking(s_i2cBases[i2cMasterHandle->instance], rxBuff, (uint16_t)rxSize, bSendStop));
}

hal_i2c_status_t HAL_I2cSlaveWriteBlocking(hal_i2c_slave_handle_t handle, const uint8_t *txBuff, size_t txSize)
{
    hal_i2c_slave_t *i2cSlaveHandle;

    assert(handle);

    i2cSlaveHandle = (hal_i2c_slave_t *)handle;

    return HAL_I2cGetStatus(I2C_SlaveWriteBlocking(s_i2cBases[i2cSlaveHandle->instance], txBuff, (uint16_t)txSize));
}

hal_i2c_status_t HAL_I2cSlaveReadBlocking(hal_i2c_slave_handle_t handle, uint8_t *rxBuff, size_t rxSize)
{
    hal_i2c_slave_t *i2cSlaveHandle;

    assert(handle);

    i2cSlaveHandle = (hal_i2c_slave_t *)handle;

    return HAL_I2cGetStatus(I2C_SlaveReadBlocking(s_i2cBases[i2cSlaveHandle->instance], rxBuff, (uint16_t)rxSize));
}

hal_i2c_status_t HAL_I2cMasterTransferBlocking(hal_i2c_master_handle_t handle, hal_i2c_master_transfer_t *xfer)
{
    hal_i2c_master_t *i2cMasterHandle;
    i2c_master_transfer_t transfer;
    uint8_t u8Addr[sizeof(xfer->subaddress)] = {0};

    assert(handle);
    assert(xfer);
    assert(xfer->subaddressSize <= sizeof(xfer->subaddress));

    i2cMasterHandle = (hal_i2c_master_t *)handle;

    /* We need big endian in DSC paltform, bacause driver API use pointer. */
    for (uint8_t i = 0; i < (xfer->subaddressSize); i++)
    {
        u8Addr[i] = (uint8_t)(xfer->subaddress >> (8U * (xfer->subaddressSize - 1U - i)));
    }

    transfer.u8ControlFlagMask = (uint8_t)xfer->flags;
    transfer.u8SlaveAddress    = xfer->slaveAddress;
    transfer.eDirection        = (kHAL_I2cRead == xfer->direction) ? kI2C_MasterReceive : kI2C_MasterTransmit;
    transfer.pu8Command        = u8Addr;
    transfer.u8CommandSize     = xfer->subaddressSize;
    transfer.pu8Data           = xfer->data;
    transfer.u16DataSize       = (uint16_t)(xfer->dataSize);

    return HAL_I2cGetStatus(I2C_MasterTransferBlocking(s_i2cBases[i2cMasterHandle->instance], &transfer));
}

hal_i2c_status_t HAL_I2cMasterTransferInstallCallback(hal_i2c_master_handle_t handle,
                                                      hal_i2c_master_transfer_callback_t callback,
                                                      void *callbackParam)
{
    hal_i2c_master_t *i2cMasterHandle;

    assert(handle);

    i2cMasterHandle = (hal_i2c_master_t *)handle;

    i2cMasterHandle->callback      = callback;
    i2cMasterHandle->callbackParam = callbackParam;
    I2C_MasterTransferCreateHandle(s_i2cBases[i2cMasterHandle->instance], &i2cMasterHandle->hardwareHandle,
                                   HAL_I2cMasterCallback, i2cMasterHandle);

    return kStatus_HAL_I2cSuccess;
}

hal_i2c_status_t HAL_I2cMasterTransferNonBlocking(hal_i2c_master_handle_t handle, hal_i2c_master_transfer_t *xfer)
{
    hal_i2c_master_t *i2cMasterHandle;
    i2c_master_transfer_t transfer;
    uint8_t u8Addr[sizeof(xfer->subaddress)] = {0};

    assert(handle);
    assert(xfer);
    assert(xfer->subaddressSize <= sizeof(xfer->subaddress));

    i2cMasterHandle = (hal_i2c_master_t *)handle;

    /* We need big endian in DSC paltform, bacause driver API use pointer. */
    for (uint8_t i = 0; i < (xfer->subaddressSize); i++)
    {
        u8Addr[i] = (uint8_t)(xfer->subaddress >> (8U * (xfer->subaddressSize - 1U - i)));
    }

    transfer.u8ControlFlagMask = (uint8_t)(xfer->flags);
    transfer.u8SlaveAddress    = xfer->slaveAddress;
    transfer.eDirection        = (kHAL_I2cRead == xfer->direction) ? kI2C_MasterReceive : kI2C_MasterTransmit;
    transfer.pu8Command        = u8Addr;
    transfer.u8CommandSize     = xfer->subaddressSize;
    transfer.pu8Data           = xfer->data;
    transfer.u16DataSize       = (uint16_t)(xfer->dataSize);
    return HAL_I2cGetStatus(I2C_MasterTransferNonBlocking(&i2cMasterHandle->hardwareHandle, &transfer));
}

hal_i2c_status_t HAL_I2cMasterTransferGetCount(hal_i2c_master_handle_t handle, size_t *count)
{
    hal_i2c_master_t *i2cMasterHandle;

    assert(handle);
    assert(count);

    i2cMasterHandle = (hal_i2c_master_t *)handle;
    uint16_t u16Count;
    status_t sts = I2C_MasterTransferGetCount(&i2cMasterHandle->hardwareHandle, &u16Count);
    if (kStatus_Success == sts)
    {
        *count = u16Count;
    }
    return HAL_I2cGetStatus(sts);
}

hal_i2c_status_t HAL_I2cMasterTransferAbort(hal_i2c_master_handle_t handle)
{
    hal_i2c_master_t *i2cMasterHandle;

    assert(handle);

    i2cMasterHandle = (hal_i2c_master_t *)handle;
    return HAL_I2cGetStatus(I2C_MasterTransferAbort(&i2cMasterHandle->hardwareHandle));
}

hal_i2c_status_t HAL_I2cSlaveTransferInstallCallback(hal_i2c_slave_handle_t handle,
                                                     hal_i2c_slave_transfer_callback_t callback,
                                                     void *callbackParam)
{
    hal_i2c_slave_t *i2cSlaveHandle;

    assert(handle);

    i2cSlaveHandle = (hal_i2c_slave_t *)handle;

    i2cSlaveHandle->callback      = callback;
    i2cSlaveHandle->callbackParam = callbackParam;
    I2C_SlaveTransferCreateHandle(s_i2cBases[i2cSlaveHandle->instance], &i2cSlaveHandle->hardwareHandle,
                                  HAL_I2cSlaveCallback, i2cSlaveHandle);

    return kStatus_HAL_I2cSuccess;
}

hal_i2c_status_t HAL_I2cSlaveTransferNonBlocking(hal_i2c_slave_handle_t handle, uint32_t eventMask)
{
    hal_i2c_slave_t *i2cSlaveHandle;

    assert(handle);

    i2cSlaveHandle = (hal_i2c_slave_t *)handle;

    i2c_slave_transfer_t sTransfer;
    sTransfer.u8EventMask = (uint8_t)eventMask;
    sTransfer.pu8Data     = NULL;
    sTransfer.u16DataSize = 0U;

    return HAL_I2cGetStatus(I2C_SlaveTransferNonBlocking(&i2cSlaveHandle->hardwareHandle, &sTransfer));
}

hal_i2c_status_t HAL_I2cSlaveTransferAbort(hal_i2c_slave_handle_t handle)
{
    hal_i2c_slave_t *i2cSlaveHandle;

    assert(handle);

    i2cSlaveHandle = (hal_i2c_slave_t *)handle;

    I2C_SlaveTransferAbort(&i2cSlaveHandle->hardwareHandle);

    return kStatus_HAL_I2cSuccess;
}

hal_i2c_status_t HAL_I2cSlaveTransferGetCount(hal_i2c_slave_handle_t handle, size_t *count)
{
    hal_i2c_slave_t *i2cSlaveHandle;

    assert(handle);
    assert(count);

    i2cSlaveHandle = (hal_i2c_slave_t *)handle;
    uint16_t u16Count;
    status_t sts = I2C_SlaveTransferGetCount(&i2cSlaveHandle->hardwareHandle, &u16Count);
    if (kStatus_Success == sts)
    {
        *count = u16Count;
    }
    return HAL_I2cGetStatus(sts);
}
