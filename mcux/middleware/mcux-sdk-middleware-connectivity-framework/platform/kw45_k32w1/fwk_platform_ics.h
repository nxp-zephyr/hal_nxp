/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FWK_PLATFORM_ICS_H__
#define __FWK_PLATFORM_ICS_H__

/*!
 * @addtogroup FWK_Platform_module
 * @{
 */
/*!
 * @addtogroup FWK_Platform_ICS
 * The FWK_Platform_ICS module
 *
 * Framework platform inter core service module provides services to manage communication between the 2 device cores.
 * @{
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "EmbeddedTypes.h"

/* -------------------------------------------------------------------------- */
/*                                Public macros                               */
/* -------------------------------------------------------------------------- */
#define MAX_TAG_SZ        40
#define MAX_SHA_SZ        20
#define MAX_VARIANT_SZ    20
#define MAX_BUILD_TYPE_SZ 10

#ifndef NBU_VERSION_DBG
#define NBU_VERSION_DBG 0
#endif

/* -------------------------------------------------------------------------- */
/*                           Public type definitions                          */
/* -------------------------------------------------------------------------- */

/*! \brief  NBU core build information.
 *
 * Data structure containing NBU build information.
 * WARNING: this must be in sync with equivalent NBU structure
 */
typedef PACKED_STRUCT NbuInfo_tag
{
    uint8_t versionNumber[3];
    uint8_t repo_digest[MAX_SHA_SZ];
    char    repo_tag[MAX_TAG_SZ];
    char    variant[MAX_VARIANT_SZ];
    char    build_type[MAX_BUILD_TYPE_SZ];
}
NbuInfo_t;

/*! \brief  FWK ICS message type.
 *
 *  \details enumarated values for FWK ICS messages
 */
typedef enum
{
    gFwkSrvNbu2HostFirst_c = 0U,
    gFwkSrvNbuInitDone_c,
    gFwkSrvNbuVersionIndication_c,
    gFwkSrvNbuApiIndication_c,
    gFwkSrvNbu2HostLast_c,
    gFwkSrvHost2NbuFirst_c = 0x80U,
    gFwkSrvNbuVersionRequest_c,
    gFwkSrvXtal32MTrimIndication_c,
    gFwkSrvNbuApiRequest_c,
    gFwkTemperatureIndication_c, /*!< Receive Temperature value */
    gFwkSrvHostChipRevision_c,   /*!< Receive Chip Revision value */
    gFwkSrvNbuSecureModeRequest_c,
    gFwkSrvHost2NbuLast_c,
} eFwkSrvMsgType;

/* -------------------------------------------------------------------------- */
/*                              Public prototypes                             */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Initialization of the PLATFORM Service channel
 *
 * \return int 0 if success, 1 if already initialized, negative value if error.
 */
int PLATFORM_FwkSrvInit(void);

/*!
 * \brief Sends message to peer CPU
 *
 * \param[in] msg_type Message type (from eFwkSrvMsgType enum)
 * \param[in] msg pointer to message buffer
 * \param[in] msg_lg size of the message (in bytes)
 * \return int 0 if success, -1 if out of memory, -2 if sending failed
 */
int PLATFORM_FwkSrvSendPacket(eFwkSrvMsgType msg_type, void *msg, uint16_t msg_lg);

/*!
 * \brief Send NbuInfo request to NBU and spin waiting for response.
 *
 * \param[in] nbu_info_p pointer to externally allocated NbuInfo_t
 * \return int 0 if OK, < 0 in case of error.
 */
int PLATFORM_GetNbuInfo(NbuInfo_t *nbu_info_p);

/*!
 * \brief Send NbuInfo request to NBU and spin waiting for response.
 *
 * \return int 0 if OK, < 0 in case of error.
 */
int PLATFORM_SendChipRevision(void);

/*!
 * \brief Call API located on the NBU core. This function will wait unti the response
 *        from the NBU is received.
 * \param[out] api_return Return status from remote API execution
 * \param[in]  api_id Function API identifier
 * \param[in]  fmt Size in bytes of argument (1, 2 or 4)
 * \param[in]  tab array of argument
 * \return bool_t 0 if OK otherwise error.\n
 *         In case of success, g_nbu_api_ind_status contains the returned value of the API.
 */
bool_t PLATFORM_NbuApiReq(int32_t *api_return, uint16_t api_id, const uint8_t *fmt, uint32_t *tab);

/*!
 * @}  end of FWK_Platform_ICS addtogroup
 */
/*!
 * @}  end of FWK_Platform_module addtogroup
 */
#endif /* __FWK_PLATFORM_ICS_H__ */
