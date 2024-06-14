/*! *********************************************************************************
 * \defgroup CONTROLLER - Controller Interface
 * @{
 ********************************************************************************** */
/*!
 * Copyright 2021-2022 NXP
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CONTROLLER_API_H_
#define _CONTROLLER_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************
* Include
************************************************************************************/

#include <stdint.h>
#include "fsl_os_abstraction.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! Default Tx Power on the advertising channel in dBm.
 *   Max Tx power is defined as gAppMaxTxPowerDbm_c
*/
#ifndef mAdvertisingDefaultTxPower_c
#define mAdvertisingDefaultTxPower_c    gAppMaxTxPowerDbm_c
#endif

/*! Default Tx Power on the connection channel in dBm.
 *   Max Tx power is defined as gAppMaxTxPowerDbm_c
*/
#ifndef mConnectionDefaultTxPower_c
#define mConnectionDefaultTxPower_c     gAppMaxTxPowerDbm_c
#endif

/*! By default PDUs with invalid parameter are ignored.
 */
#ifndef gLlInvalidPduHandlingType_c
#define gLlInvalidPduHandlingType_c     0U
#endif

typedef enum txChannelType_tag {
    gAdvTxChannel_c,
    gConnTxChannel_c
} txChannelType_t;

typedef enum advCodingScheme_tag {
    gAdv_CodingScheme_S8_S8_c = 0x00, /* primary adv coding scheme S8, secondary adv coding scheme S8 */
    gAdv_CodingScheme_S8_S2_c = 0x01, /* primary adv coding scheme S8, secondary adv coding scheme S2 */
    gAdv_CodingScheme_S2_S8_c = 0x02, /* primary adv coding scheme S2, secondary adv coding scheme S8 */
    gAdv_CodingScheme_S2_S2_c = 0x03, /* primary adv coding scheme S2, secondary adv coding scheme S2 */
} advCodingScheme_t;

typedef enum connCodingScheme_tag {
    gConn_CodingScheme_S8_S8_c = 0x00, /* initiation coding scheme S8, connection coding scheme S8 */
    gConn_CodingScheme_S8_S2_c = 0x01, /* initiation coding scheme S8, connection coding scheme S2 */
    gConn_CodingScheme_S2_S8_c = 0x02, /* initiation coding scheme S2, connection coding scheme S8 */
    gConn_CodingScheme_S2_S2_c = 0x03, /* initiation coding scheme S2, connection coding scheme S2 */
} connCodingScheme_t;

typedef enum connNotifMode_tag {
    gBleConnNotifAll_c = 0x00,     /* allow notification of any non-empty PDU */
    gBleConnNotifDataOnly_c = 0x01 /* allow notification of Data PDU only */
} connNotifMode_t;
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
 * \brief  Sets the BD_ADDR.
 * \param[in]  packetType HCI packet Type
 *
 * \return KOSA_StatusSuccess or KOSA_StatusError
 ********************************************************************************** */
osa_status_t Controller_SetDeviceAddress(uint8_t* bdAddr);


/*! *********************************************************************************
 * \brief  Sets the TX Power on the advertising or connection channel.
 * \param[in]  level    Power level (range 0-X) as defined in the table bellow.
 *                      (X=15 for MKW40 and X=31 for MKW41 and MKW38)
 * \param[in]  channel  Advertising or connection channel.
 *
 * \return KOSA_StatusSuccess or KOSA_StatusError.
 *
 * \remarks This function executes synchronously.
 *
 * \remarks For MKW40Z BLE controller there are 16 possible power levels 0 <= N <= 15
 * for which the output power is distributed evenly between minimum and maximum power levels.
 * For further details see the silicon datasheet.
 *
 * \remarks For MKW41Z BLE controller there are 32 possible power levels 0 <= N <= 31
 * for which the output power is distributed evenly between minimum and maximum power levels.
 * For further details see the silicon datasheet.
 *
 * \remarks For MKW38/KW45 BLE controller there are 32 possible power levels 0 <= N <= 31
 * for which the output power is distributed evenly between minimum and maximum power levels.
 * For further details see the silicon datasheet. For this latter platform, the preferred
 * interface to set the TX power is Controller_SetTxPowerLevelDbm (see below) which does not
 * require any knowledge of the power tables.
 ********************************************************************************** */
osa_status_t Controller_SetTxPowerLevel(uint8_t level, txChannelType_t channel);

/*! *********************************************************************************
 * \brief  Sets the TX Power on the advertising or connection channel.
  * \param[in]  level_dbm    Power level in dBm.
 * \param[in]  channel      Advertising or connection channel.
 *
 * \return KOSA_StatusSuccess or KOSA_StatusError.
 *
 * \remarks This function executes synchronously.
 ********************************************************************************** */
osa_status_t Controller_SetTxPowerLevelDbm(int8_t level_dbm, txChannelType_t channel);


/*! *********************************************************************************
 * \brief  Set the XCVR ANALOG LDO ANT TRIM register value.
 * \param[in]  power_dBm       maximum target tx power
 * \param[in]  ldo_ant_trim    LDO ANT TRIM to set
 *
 * \return KOSA_StatusSuccess or KOSA_StatusError.
 ********************************************************************************** */
osa_status_t Controller_SetMaxTxPower(int8_t power_dBm, uint8_t ldo_ant_trim);


/*! *********************************************************************************
 * \brief  Set the seed for the PRNG.
 * \param[in]  seed    seed to be set for PRNG initial state
 *
 * \return KOSA_StatusSuccess or KOSA_StatusError.
 ********************************************************************************** */
osa_status_t Controller_SetRandomSeed(void);


/*! *********************************************************************************
 * \brief  Configures the handling of PDU with invalid parameter.
 * \param[in]  pdu_handling_type    handling type for invalid PDU (0: ignore PDU, 1: disconnect link)
 *
 * \return KOSA_StatusSuccess or KOSA_StatusError.
 ********************************************************************************** */
osa_status_t Controller_ConfigureInvalidPduHandling(uint32_t pdu_handling_type);


/*! *********************************************************************************
 * \brief  Configures the advertising Coding scheme.
 * \param[in]  advCodingScheme_t    coding scheme
 *
 * \return KOSA_StatusSuccess or KOSA_StatusError.
 ********************************************************************************** */
osa_status_t Controller_ConfigureAdvCodingScheme(advCodingScheme_t codingSch);


/*! *********************************************************************************
 * \brief  Configures the connection Coding scheme.
 * \param[in]  advCodingScheme_t    coding scheme
 *
 * \return KOSA_StatusSuccess or KOSA_StatusError.
 ********************************************************************************** */
osa_status_t Controller_ConfigureConnCodingScheme(connCodingScheme_t codingSch);

/*! *********************************************************************************
 * \brief  Configures the connection enhanced notification mode.
 * \param[in]  uint32    mode
 *
 * \return KOSA_StatusSuccess or KOSA_StatusError.
 ********************************************************************************** */
osa_status_t Controller_SetConnNotificationMode(uint32_t mode);

/*! *********************************************************************************
 * \brief  Enables the security feature.
 * \param[in]  None
 *
 * \return KOSA_StatusSuccess or KOSA_StatusError.
 ********************************************************************************** */
osa_status_t Controller_EnableSecurityFeature(void);


/*! *********************************************************************************
 * \brief  Get Next Link Layer event in us.
 * \param[in]  remaining_time until next event
 *         0 if no next event, >0 otheriwse.
 *      @Warning immediate next event shall always be >0
 *      @Warning API shall be called with OS scheduler disabled or interrupts masked
 *
 * \return KOSA_StatusSuccess or KOSA_StatusError.
 ********************************************************************************** */
osa_status_t Controller_GetNextLinLayerEventUsUnsafe(uint32_t* remaining_time);


/*! *********************************************************************************
 * \brief  request Link layer to enter low power.
 *      @Warning API shall be called with OS scheduler disabled or interrupts masked
 *
 * \return KOSA_StatusSuccess or KOSA_StatusError.
 ********************************************************************************** */
osa_status_t Controller_EnterLowpower(void);

/*! *********************************************************************************
 * \brief  request Link layer to exit low power.
 *      @Warning API shall be called with OS scheduler disabled or interrupts masked
 *
 * \return KOSA_StatusSuccess or KOSA_StatusError.
 ********************************************************************************** */
osa_status_t Controller_ExitLowpower(void);


#ifdef __cplusplus
}
#endif

#endif /*_CONTROLLER_API_H_*/

/*! *********************************************************************************
* @}
********************************************************************************** */
