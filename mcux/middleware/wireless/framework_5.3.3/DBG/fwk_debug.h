/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FWK_DEBUG_H__
#define __FWK_DEBUG_H__

#if (defined(gDbg_Enabled_d) && (gDbg_Enabled_d > 0))
#if defined(KW45B41Z83_NBU_SERIES)
// TODO : create a board_dbg.h for NBU , create a unified compile switch for both debug CM3, CM33
#include "nbu_dbg.h"
#else
#include "board_dbg.h"
#endif
#endif

#if defined(SERIAL_BTSNOOP)
#include "sbtsnoop.h"
#endif

/************************************************************************************
 * DEBUG - STUB MACROS
 ************************************************************************************/
/* Debug MACRO stubbed if not defined */
#ifndef BOARD_DBGLPIOSET
#define BOARD_DBGLPIOSET(__x, __y)
#endif

#ifndef BOARD_DBGINITSET
#define BOARD_DBGINITSET(__x, __y)
#endif

#ifndef BOARD_DBGINITDBGIO
#define BOARD_DBGINITDBGIO()
#endif

#ifndef BOARD_DBGAPPIOSET
#define BOARD_DBGAPPIOSET(__x, __y)
#endif

#ifndef BOARD_DBGTOGGLEDBGIO
#define BOARD_DBGTOGGLEDBGIO()
#endif

#ifndef BOARD_DBGINITDTEST
#define BOARD_DBGINITDTEST()
#endif

#ifndef BOARD_DBGSETDBGIOFORLOWPOWER
#define BOARD_DBGSETDBGIOFORLOWPOWER()
#endif

#ifndef BOARD_DBGCONFIGINIT
#define BOARD_DBGCONFIGINIT(__x)
#endif

#ifndef BOARD_DBGCONFIGINITNBU
#define BOARD_DBGCONFIGINITNBU(__x)
#endif

#ifndef APP_DBG_LOG
#define APP_DBG_LOG(...)
#endif

#ifndef PWR_DBG_LOG
#define PWR_DBG_LOG(...)
#endif

#ifndef MEM_DBG_LOG
#define MEM_DBG_LOG(...)
#endif

#ifndef DBG_DBG_LOG
#define DBG_DBG_LOG(...)
#endif

#ifndef INIT_DBG_LOG
#define INIT_DBG_LOG(...)
#endif

#ifndef HCI_DBG_LOG
#define HCI_DBG_LOG(...)
#endif

#ifndef PLAT_DBG_LOG
#define PLAT_DBG_LOG(...)
#endif

#ifndef DBG_LOG_DUMP
#define DBG_LOG_DUMP()
#endif

#ifndef DBG_LOG_DUMP_CHECK
#define DBG_LOG_DUMP_CHECK()
#endif

#ifndef DBG_LOG_WAKEUP_CHECK
#define DBG_LOG_WAKEUP_CHECK()
#endif

#ifndef BOARD_DBGLOGCOUNTERRUNNING
#define BOARD_DBGLOGCOUNTERRUNNING()
#endif

#ifndef BOARD_DBGINITRFACTIVE
#define BOARD_DBGINITRFACTIVE()
#endif

#endif /*  __FWK_DEBUG_H__ */
