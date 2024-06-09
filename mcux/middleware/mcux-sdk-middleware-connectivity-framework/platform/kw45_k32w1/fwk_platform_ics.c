/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <stdarg.h>

#include "fsl_common.h"
#include "fwk_platform_ics.h"
#include "fwk_platform.h"
#include "FunctionLib.h"
#include "fsl_adapter_rpmsg.h"
#include "fwk_debug.h"

#if defined(NBU_VERSION_DBG) && (NBU_VERSION_DBG == 1)
#include "fsl_debug_console.h"
#endif

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

/* Number of loops we spin waiting for NBU processor to respond */
#define MAX_WAIT_NBU_RESPONSE_LOOPS 10000U

/* API wait loop counter */
#define MAX_WAIT_NBU_API_RESPONSE_LOOPS 100000000U

/* maximum size for API parameters */
#define NBU_API_MAX_PARAM_LENGTH 40U

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */
static hal_rpmsg_return_status_t PLATFORM_FwkSrv_RxCallBack(void *param, uint8_t *data, uint32_t len);
static bool                      FwkSrv_MsgTypeInExpectedSet(uint8_t msg_type);

/* -------------------------------------------------------------------------- */
/*                         Private memory declarations                        */
/* -------------------------------------------------------------------------- */

static RPMSG_HANDLE_DEFINE(fwkRpmsgHandle);
static const hal_rpmsg_config_t fwkRpmsgConfig = {
    .local_addr  = 110,
    .remote_addr = 100,
    .callback    = PLATFORM_FwkSrv_RxCallBack,
    .param       = NULL,
};

/* flag notifying of NBU Infor reception from CM3 */
static volatile bool g_nbu_info_resp_received = false;
static NbuInfo_t *   g_nbu_info_p             = (NbuInfo_t *)NULL;
static volatile bool g_nbu_init_done          = false;

static uint32_t         m_nbu_api_dbg_max_wait_loop = 0; /* maximum loop counter logged */
static volatile bool    m_nbu_api_ind_received;
static volatile bool    m_nbu_api_rpmsg_status;
static volatile int32_t m_nbu_api_return_status;

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

int PLATFORM_FwkSrvInit(void)
{
    int result = 0;

    static bool_t      mFwkSrvInit  = FALSE;
    hal_rpmsg_config_t rpmsg_config = fwkRpmsgConfig;

    uint32_t irqMask = DisableGlobalIRQ();

    do
    {
        if (mFwkSrvInit == TRUE)
        {
            result = 1;
            break;
        }
        if (kStatus_HAL_RpmsgSuccess != HAL_RpmsgInit((hal_rpmsg_handle_t)fwkRpmsgHandle, &rpmsg_config))
        {
            result = -2;
            break;
        }
        /* Flag initialization on module */
        mFwkSrvInit = TRUE;
    } while (false);

    EnableGlobalIRQ(irqMask);

    return result;
}

int PLATFORM_FwkSrvSendPacket(eFwkSrvMsgType msg_type, void *msg, uint16_t msg_lg)
{
    uint8_t *buf    = NULL;
    int      result = 0;
    uint32_t sz     = ((uint32_t)msg_lg + 1U);

    /* Make sure remote CPU is awake */
    PLATFORM_RemoteActiveReq();

    do
    {
        buf = HAL_RpmsgAllocTxBuffer((hal_rpmsg_handle_t)fwkRpmsgHandle, sz);

        if (NULL == buf)
        {
            result = -1;
            break;
        }

        buf[0] = (uint8_t)msg_type;

        if (msg != NULL && msg_lg != 0U)
        {
            FLib_MemCpy(&buf[1], (uint8_t *)msg, msg_lg);
        }

        if (kStatus_HAL_RpmsgSuccess != HAL_RpmsgNoCopySend((hal_rpmsg_handle_t)fwkRpmsgHandle, buf, (uint32_t)sz))
        {
            result = -2;
            break;
        }
    } while (false);

    /* Release wake up to other CPU */
    PLATFORM_RemoteActiveRel();

    return result;
}

/* Returns negative value if info is not available, 0 otherwise */
int PLATFORM_GetNbuInfo(NbuInfo_t *nbu_info_p)
{
    int st = -1;

    do
    {
        uint32_t cnt;

        g_nbu_info_resp_received = false;

        /* Need a storage supplied by the caller to copy result from RPMSG memory */
        if (nbu_info_p == NULL)
        {
            break;
        }
        g_nbu_info_p = nbu_info_p;
        st           = PLATFORM_FwkSrvSendPacket(gFwkSrvNbuVersionRequest_c, (void *)NULL, 0);
        if (st != 0)
        {
            break;
        }
        /* Wait for NBU / CM3 to answer but not forever */
        for (cnt = MAX_WAIT_NBU_RESPONSE_LOOPS * 10U; cnt > 0U; cnt--)
        {
            if (g_nbu_info_resp_received)
            {
                st = 0;
                break;
            }
        }
    } while (false);
    /* The Rx Call back has already filled the structure the global pointer can
     be cleared. Should the indication arrive late - becasue of a breakpoint in
     the CM3 for instance, the callback would simply drop the indication  */
    g_nbu_info_p = NULL;
    assert(st == 0);
    return st;
}

int PLATFORM_SendChipRevision(void)
{
    uint8_t chip_rev_reg = Chip_GetVersion();
    uint8_t chip_rev     = 0xFF;
    int     ret          = -3;

    if (chip_rev_reg == DEVICE_REVISION_A0)
    {
        chip_rev = 0u;
    }
    else if (chip_rev_reg == DEVICE_REVISION_A1)
    {
        chip_rev = 1u;
    }
    else
    {
        /* MISRA compliance */
    }

    if (chip_rev != 0xFFu)
    {
        ret = PLATFORM_FwkSrvSendPacket(gFwkSrvHostChipRevision_c, (void *)&chip_rev, 1);
    }

    PWR_DBG_LOG("chip rev sent:%d", chip_rev);

    return ret;
}

bool_t PLATFORM_NbuApiReq(int32_t *api_return, uint16_t api_id, const uint8_t *fmt, uint32_t *tab)
{
    bool rpmsg_status = true;
    bool nbu_rpmsg_status;

    /* Make sure remote CPU is awake */
    PLATFORM_RemoteActiveReq();

    do
    {
        /* build the message payload */
        uint8_t data[2 + NBU_API_MAX_PARAM_LENGTH];

        /* start with API identifier */
        data[0U] = (uint8_t)api_id;
        data[1U] = (uint8_t)(api_id >> 8U) & 0xffU;

        uint16_t data_len = 2U;
        uint32_t param;
        uint32_t j = 0U;

        for (uint32_t i = 0U; fmt[i] != 0U; i++)
        {
            if ((data_len + (uint16_t)fmt[i]) > NBU_API_MAX_PARAM_LENGTH)
            {
                /* too many arguments */
                rpmsg_status = false;
                break;
            }
            else
            {
                switch (fmt[i])
                {
                    case 1U:
                        param = tab[j];
                        j++;
                        if (param > 0xFFU)
                        {
                            rpmsg_status = false;
                        }
                        else
                        {
                            data[data_len++] = (uint8_t)param;
                        }
                        break;

                    case 2U:
                        param = tab[j];
                        j++;
                        if (param > 0xFFFFU)
                        {
                            rpmsg_status = false;
                        }
                        else
                        {
                            data[data_len++] = (uint8_t)param;
                            data[data_len++] = (uint8_t)(param >> 8U);
                        }
                        break;

                    case 4U:
                        param = tab[j];
                        j++;
                        data[data_len++] = (uint8_t)param;
                        data[data_len++] = (uint8_t)(param >> 8U);
                        data[data_len++] = (uint8_t)(param >> 16U);
                        data[data_len++] = (uint8_t)(param >> 24U);
                        break;

                    default:
                        rpmsg_status = false;
                        break;
                }
            }
        }

        if (!rpmsg_status)
        {
            break;
        }

        /* send to NBU */
        m_nbu_api_ind_received = false;
        if (0 != PLATFORM_FwkSrvSendPacket(gFwkSrvNbuApiRequest_c, (void *)&data, data_len))
        {
            rpmsg_status = false;
            break;
        }

        /* Wait for NBU / CM3 to answer but not forever */
        uint32_t cnt          = 0;
        bool     nbu_received = m_nbu_api_ind_received;

        while ((!m_nbu_api_ind_received) && (cnt < MAX_WAIT_NBU_API_RESPONSE_LOOPS))
        {
            // wait loop
            cnt++;
            assert(cnt != MAX_WAIT_NBU_API_RESPONSE_LOOPS);
        }
        if (m_nbu_api_dbg_max_wait_loop < cnt)
        {
            // log for debug purpose
            m_nbu_api_dbg_max_wait_loop = cnt;
        }

        assert(m_nbu_api_rpmsg_status);
        nbu_rpmsg_status = m_nbu_api_rpmsg_status;

        nbu_received = m_nbu_api_ind_received;
        rpmsg_status = nbu_received && nbu_rpmsg_status;

        /* API executed */
        *api_return = m_nbu_api_return_status;
    } while (0U != 0U);

    /* Release wake up to other CPU */
    PLATFORM_RemoteActiveRel();

    /* return rmpsg status, API status in *api_status */
    assert(rpmsg_status);
    return rpmsg_status;
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static hal_rpmsg_return_status_t PLATFORM_FwkSrv_RxCallBack(void *param, uint8_t *data, uint32_t len)
{
    hal_rpmsg_return_status_t res = kStatus_HAL_RL_RELEASE;
    uint8_t                   msg_type;
    uint32_t                  temp = 0U;
    msg_type = data[0];

    if (FwkSrv_MsgTypeInExpectedSet(msg_type))
    {
        switch (msg_type)
        {
            case (uint8_t)gFwkSrvNbuVersionIndication_c:
                if (g_nbu_info_p != NULL)
                {
                    g_nbu_info_resp_received = true;
                    FLib_MemCpy(g_nbu_info_p, &data[1], sizeof(NbuInfo_t));

#if defined(NBU_VERSION_DBG) && (NBU_VERSION_DBG == 1)
                    PRINTF("NBU v%d.%d.%d\r\n", g_nbu_info_p->versionNumber[0], g_nbu_info_p->versionNumber[1],
                           g_nbu_info_p->versionNumber[2]);
                    PRINTF("NBU SHA %02x%02x%02x%02x\r\n", g_nbu_info_p->repo_digest[0], g_nbu_info_p->repo_digest[1],
                           g_nbu_info_p->repo_digest[2], g_nbu_info_p->repo_digest[3]);
#endif
                    res = kStatus_HAL_RL_RELEASE;
                    /* no longer required to hold since copy is done in allocated pointer */
                }
                break;
            case (uint8_t)gFwkSrvNbuInitDone_c:
                g_nbu_init_done = true;
#if defined(NBU_VERSION_DBG) && (NBU_VERSION_DBG == 1)
                PRINTF("NBU Init Done\r\n");
#endif
                break;
            case (uint8_t)gFwkSrvNbuApiIndication_c:
            {
                /* NBU API response received */
                assert(len == 6U);
                m_nbu_api_ind_received = true;
                m_nbu_api_rpmsg_status = (data[1] == 0U) ? false : true;

                for (uint8_t i = 5U; i > 1U; i--)
                {
                    temp = (temp << 8) | (uint32_t)(data[i] & 0xFFU);
                }
                m_nbu_api_return_status = (int32_t)temp;
                res                     = kStatus_HAL_RL_RELEASE;
            }
            break;

            default:; /* For MISRA compliance */
                break;
        }
    }
    return res;
}

static bool FwkSrv_MsgTypeInExpectedSet(uint8_t msg_type)
{
    return (msg_type > (uint8_t)gFwkSrvNbu2HostFirst_c && msg_type < (uint8_t)gFwkSrvNbu2HostLast_c);
}
