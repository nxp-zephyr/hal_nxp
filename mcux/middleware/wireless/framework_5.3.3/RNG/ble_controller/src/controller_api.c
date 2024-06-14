/*! *********************************************************************************
 * \defgroup CONTROLLER
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
*
* Copyright 2021-2022 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "fwk_platform_ics.h"
#include "controller_api.h"
#include "RNG_Interface.h"

/************************************************************************************
*************************************************************************************
* Definitions
*************************************************************************************
************************************************************************************/

typedef enum
{
    API_Controller_SetTxPowerLevel,
    API_Controller_SetTxPowerLevelDbm,
    API_Controller_SetMaxTxPower,
    API_Controller_SetRandomSeed,
    API_Controller_ConfigureAdvCodingScheme,
    API_Controller_ConfigureConnCodingScheme,
    API_Controller_ConfigureInvalidPduHandling,
    API_Controller_SetConnNotificationMode,
    API_Controller_SetChannelSelectionAlgo2,
    API_Last
} PLATFORM_NbuApiId;


/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
osa_status_t Controller_SetTxPowerLevel(uint8_t level, txChannelType_t channel)
{
    osa_status_t api_return = KOSA_StatusSuccess;
    int32_t      ret        = 0;
    bool rpmsg_status;
    uint32_t tab[2U] = {(uint32_t)level, (uint32_t)channel};
    rpmsg_status = PLATFORM_NbuApiReq(&ret, API_Controller_SetTxPowerLevel,
                                (const uint8_t*)"\x01\x01", tab);
    assert(rpmsg_status);
    (void)rpmsg_status;

    if ( ret!=0 )
    {
        api_return = KOSA_StatusError;
    }
    return api_return;
}

osa_status_t Controller_SetTxPowerLevelDbm(int8_t level_dbm, txChannelType_t channel)
{
    osa_status_t api_return = KOSA_StatusSuccess;
    int32_t      ret        = 0;
    bool rpmsg_status;
    uint32_t tab[2U] = {(uint32_t)level_dbm, (uint32_t)channel};
    rpmsg_status = PLATFORM_NbuApiReq(&ret, API_Controller_SetTxPowerLevelDbm,
                                (const uint8_t*)"\x01\x01", tab);
    assert(rpmsg_status);
    (void)rpmsg_status;

    if ( ret!=0 )
    {
        api_return = KOSA_StatusError;
    }
    return api_return;
}

osa_status_t Controller_SetMaxTxPower(int8_t power_dBm, uint8_t ldo_ant_trim)
{
    osa_status_t api_return = KOSA_StatusSuccess;
    int32_t      ret        = 0;
    bool rpmsg_status;
    uint32_t tab[2U] = {(uint32_t)power_dBm, (uint32_t)ldo_ant_trim};
    rpmsg_status = PLATFORM_NbuApiReq(&ret, API_Controller_SetMaxTxPower,
                                (const uint8_t*)"\x01\x01", tab);
    assert(rpmsg_status);
    (void)rpmsg_status;

    if ( ret!=0 )
    {
        api_return = KOSA_StatusError;
    }
    return api_return;
}

osa_status_t Controller_SetRandomSeed(void)
{
    osa_status_t api_return = KOSA_StatusSuccess;
    int32_t      ret        = 0;
    bool rpmsg_status;
    uint32_t seed;
    RNG_GetPseudoRandomNo((uint8_t*)&seed, sizeof(seed), NULL);
    rpmsg_status = PLATFORM_NbuApiReq(&ret, API_Controller_SetRandomSeed,
                                (const uint8_t*)"\x04", &seed);
    assert(rpmsg_status);
    (void)rpmsg_status;

    if ( ret!=0 )
    {
        api_return = KOSA_StatusError;
    }
    return api_return;
}

osa_status_t Controller_ConfigureAdvCodingScheme(advCodingScheme_t codingSch)
{
    osa_status_t api_return    = KOSA_StatusSuccess;
    int32_t      ret           = 0;
    uint32_t     coding_scheme = (uint32_t)codingSch;
    bool rpmsg_status = PLATFORM_NbuApiReq(&ret, API_Controller_ConfigureAdvCodingScheme,
                                (const uint8_t*)"\x01", &coding_scheme);
    assert(rpmsg_status);
    (void)rpmsg_status;

    if ( ret!=0 )
    {
        api_return = KOSA_StatusError;
    }
    return api_return;
}

osa_status_t Controller_ConfigureConnCodingScheme(connCodingScheme_t codingSch)
{
    osa_status_t api_return = KOSA_StatusSuccess;
    int32_t      ret        = 0;
    uint32_t     coding_scheme = (uint32_t)codingSch;
    bool rpmsg_status = PLATFORM_NbuApiReq(&ret, API_Controller_ConfigureConnCodingScheme,
                                (const uint8_t*)"\x01", &coding_scheme);
    assert(rpmsg_status);
    (void)rpmsg_status;

    if ( ret!=0 )
    {
        api_return = KOSA_StatusError;
    }
    return api_return;
}

osa_status_t Controller_ConfigureInvalidPduHandling(uint32_t pdu_handling_type)
{
    osa_status_t api_return = KOSA_StatusSuccess;
    int32_t      ret        = 0;
    bool rpmsg_status = PLATFORM_NbuApiReq(&ret, API_Controller_ConfigureInvalidPduHandling,
                                (const uint8_t*)"\x01", &pdu_handling_type);
    assert(rpmsg_status);
    (void)rpmsg_status;

    if ( ret!=0 )
    {
        api_return = KOSA_StatusError;
    }
    return api_return;
}

osa_status_t Controller_SetConnNotificationMode(uint32_t mode)
{
    osa_status_t api_return = KOSA_StatusSuccess;
    int32_t      ret        = 0;
    bool rpmsg_status = PLATFORM_NbuApiReq(&ret, API_Controller_SetConnNotificationMode,
                                (const uint8_t*)"\x04", &mode);
    assert(rpmsg_status);
    (void)rpmsg_status;

    if ( ret!=0 )
    {
        api_return = KOSA_StatusError;
    }
    return api_return;
}

osa_status_t Controller_SetChannelSelectionAlgo2(bool enable)
{
    osa_status_t api_return = KOSA_StatusSuccess;
    int32_t      ret        = 0;
    bool rpmsg_status = PLATFORM_NbuApiReq(&ret, API_Controller_SetChannelSelectionAlgo2,
                                (const uint8_t*)"\x01", (uint32_t *)&enable);
    assert(rpmsg_status);
    (void)rpmsg_status;

    if ( ret!=0 )
    {
        api_return = KOSA_StatusError;
    }
    return api_return;
}    
    
osa_status_t Controller_GetNextLinLayerEventUsUnsafe(uint32_t* remaining_time)
{
    osa_status_t api_return = KOSA_StatusError;
    * remaining_time = 0u; /* no event */

    return api_return;
}
