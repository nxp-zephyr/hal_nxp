/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*==========================================================================
Implementation file for Low level drivers for CM33-CE Driver
==========================================================================*/

#include "fsl_common.h"
#include "fsl_ce_cmd.h"

#define CE_MU MUA
#define CE_COMPUTE_DONE 0xD09EU

static ce_cmdbuffer_t *s_ce_cmdbuffer;

static inline void CE_CmdDelay(void)
{
    int i;
    for (i = 0; i < 100; i++)
    {
        __NOP();
    }
}

int CE_CmdInitBuffer(ce_cmdbuffer_t *psCmdBuffer,
                       uint32_t cmdbuffer[256],
                       uint32_t statusbuffer[134],
                       ce_cmd_mode_t cmdmode)
{
    s_ce_cmdbuffer = psCmdBuffer;

    s_ce_cmdbuffer->cmdmode = cmdmode;
    s_ce_cmdbuffer->buffer_base_ptr = (int *)cmdbuffer;
    s_ce_cmdbuffer->status_buffer_ptr = (int *)statusbuffer;
    CE_CmdReset();

    return 0;
}

int CE_CmdReset()
{
    int *cmd_base = s_ce_cmdbuffer->buffer_base_ptr;
    *cmd_base = 0xCCCC;
    s_ce_cmdbuffer->next_buffer_ptr = cmd_base + 1;
    s_ce_cmdbuffer->n_cmd = 0;

    return 0;
}

int CE_CmdAdd(ce_cmd_t cmd, ce_cmdstruct_t *cmdargs)
{
    int i, size, addstatus;
    short *nargsbase;
    int *cmdbase;
    void **ptrargbase;
    int *ptrparambase;

    if (s_ce_cmdbuffer->n_cmd < CE_CMD_MAX_CMDS_ZVQ)
    {
        size = sizeof(void *) * cmdargs->n_ptr_args +
           sizeof(int) * (cmdargs->n_param_args + 1) + sizeof(short) * 2;
        cmdbase = s_ce_cmdbuffer->next_buffer_ptr;

        *cmdbase = (unsigned int)cmd;

        nargsbase = (short *)(cmdbase + 1);
        *nargsbase = cmdargs->n_ptr_args;
        nargsbase += 1;
        *nargsbase = cmdargs->n_param_args;
        nargsbase += 1;

        ptrargbase = (void **)nargsbase;
        for (i = 0; i < cmdargs->n_ptr_args; i++)
        {
            *ptrargbase = cmdargs->arg_ptr_array[i];
            ptrargbase += 1;
        }

        ptrparambase = (int *)ptrargbase;
        for (i = 0; i < cmdargs->n_param_args; i++)
        {
            *ptrparambase = cmdargs->arg_param_array[i];
            ptrparambase += 1;
        }

        s_ce_cmdbuffer->n_cmd++;

        cmdbase += (size / sizeof(int));
        s_ce_cmdbuffer->next_buffer_ptr = cmdbase;

        addstatus = 0;
    }
    else
    {
        addstatus = -1;
    }

    return addstatus;
}

int CE_CmdLaunch(int force_launch)
{
    if (force_launch == 1)
    {
        if (s_ce_cmdbuffer->cmdmode > kCE_CmdModeMultipleNonBlocking)
        {
            return CE_CmdLaunchBlocking();
        }
        else
        {
            return CE_CmdLaunchNonBlocking();
        }
    }

    if (s_ce_cmdbuffer->cmdmode == kCE_CmdModeOneNonBlocking)
    {
        return CE_CmdLaunchNonBlocking();
    }

    if (s_ce_cmdbuffer->cmdmode == kCE_CmdModeOneBlocking)
    {
        return CE_CmdLaunchBlocking();
    }

    return 0;
}

int CE_CmdLaunchBlocking()
{
    unsigned int n_cmd;

    if (s_ce_cmdbuffer->n_cmd == 0)
    {
        return -2; /* no commands to send */
    }

    /* write number of commands via TX2 reg */
    CE_MU->TR[2] = s_ce_cmdbuffer->n_cmd;
    CE_CmdDelay();

    /* launch CE by sending MU interrupt */
    CE_MU->GCR = MU_GCR_GIR0_MASK;

    /* blocking: so poll till completion */
    /* completion is signaled when ZV2117 writes "D09E"to top of cmd buffer */
    n_cmd = *(s_ce_cmdbuffer->buffer_base_ptr);
    while (n_cmd != CE_COMPUTE_DONE)
    {
        CE_CmdDelay();
        n_cmd = *(s_ce_cmdbuffer->buffer_base_ptr);
    }

    CE_CmdReset();

    /* read the status register */
    return *(s_ce_cmdbuffer->status_buffer_ptr + 1);
}

int CE_CmdLaunchNonBlocking()
{
    /* placeholder for writing a register to signal "WKLOAD_RDY" Event
     *  need to synch with architeture team on how this connection works
     *  arm_register_write(EVENT_CE_WKLOAD_RDY);
     */

    /* non-blocking: so return and the ARM core can resume other tasks */
    return 0;
}

int CE_CmdCheckStatus()
{
    int status = -1;
    unsigned int n_cmd = *(s_ce_cmdbuffer->buffer_base_ptr);

    if (n_cmd > 0)
    {
        status = 1; /* still running */
    }
    else
    {
        status = 0; /* completed */
    }

    return status;
}
