/* -----------------------------------------------------------------------------
 * Copyright (c) 2004 - 2016 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * $Date:        26. August 2016
 * $Revision:    V1.00
 *
 * Project:      Flash Device Algorithm for NXP MCXN9XX Flash using IAP
 * --------------------------------------------------------------------------- */

#include "FlashOS.h" // FlashOS Structures

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "fsl_flash.h"
#include "fsl_flash_ffr.h"

static flash_config_t flashInstance;
extern struct FlashDevice const FlashDevice;

status_t CLOCK_SetupFROHFClocking(uint32_t iFreq);

// Memory Mapping Control
#ifdef MCXN9XX
#define MEMMAP (*((volatile unsigned char *)0x50000000))
#endif

// Main Clock
#ifdef MCXN9XX
#define MAINCLKSELA         (*((volatile unsigned long *)0x50000280))
#define MAINCLKSELB         (*((volatile unsigned long *)0x50000284))
#define AHBCLKDIV           (*((volatile unsigned long *)0x50000380))
#define PRESETCTRL0         (*((volatile unsigned long *)0x50000100))
#define PRESETCTRLSET0      (*((volatile unsigned long *)0x50000120))
#define PRESETCTRLCLR0      (*((volatile unsigned long *)0x50000140))
#define FLASH_MASK          ((1 << 7) | (1 << 8))
#define AHBCLKCTRL0         (*((volatile unsigned long *)0x50000200))
#define AUTOCLKGATEOVERRIDE (*((volatile unsigned long *)0x50000E04))
#endif

#define FLASH_ERASE_SIZE_BYTES   0x2000 // 8K
#define FLASH_PROGRAM_SIZE_BYTES 512    // 512B

#ifdef FLASH_2048
#define FLASH_SIZE (0x200000)
#endif
#ifdef FLASH_2048_S
#define FLASH_SIZE (0x200000)
#endif

#ifdef FLASH_1024
#define FLASH_SIZE (0x100000)
#endif
#ifdef FLASH_1024_S
#define FLASH_SIZE (0x100000)
#endif

#ifdef FLASH_768
#define FLASH_SIZE (0xC0000)
#endif
#ifdef FLASH_768_S
#define FLASH_SIZE (0xC0000)
#endif

#ifdef FLASH_512
#define FLASH_SIZE (0x80000)
#endif
#ifdef FLASH_512_S
#define FLASH_SIZE (0x80000)
#endif

#if !defined(FOUR_CHAR_CODE)
#define FOUR_CHAR_CODE(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))
#endif

/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init(unsigned long adr, unsigned long clk, unsigned long fnc)
{
    CLOCK_SetupFROHFClocking(48000000);
    SCG0->RCCR = SCG_RCCR_SCS(3);
    while ((SCG0->CSR & SCG_CSR_SCS_MASK) != SCG_CSR_SCS(3))
    {
    }

    // Initialize flash driver
    status_t status;
    status = FLASH_Init(&flashInstance);
    if (status != 0)
    {
        return 1;
    }

    status = FFR_Init(&flashInstance);
    if (status != 0)
    {
        return 1;
    }

    return 0;
}

/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit(unsigned long fnc)
{
    return (0);
}

/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip(void)
{
    status_t status;
    status = FLASH_Erase(&flashInstance, 0, FLASH_SIZE, kFLASH_ApiEraseKey);

    if (status == kStatus_FLASH_Success)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector(unsigned long adr)
{
    status_t status;

    status = FLASH_Erase(&flashInstance, (adr & 0x0FFFFFFF), FLASH_ERASE_SIZE_BYTES, kFLASH_ApiEraseKey);
    if (status == kStatus_FLASH_Success)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage(unsigned long adr, unsigned long sz, unsigned char *buf)
{
    if (0 != FLASH_Program(&flashInstance, (adr & 0x0FFFFFFF), (uint8_t *)buf, ((sz >= 512) ? sz : 512)))
    {
        return 1;
    }

    return 0;
}

/*
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */
#if 1
unsigned long Verify(unsigned long adr, unsigned long sz, unsigned char *buf)
{
    uint32_t status = 0;

    status = memcmp((void *)(adr & 0x0FFFFFFF), buf, sz);

    if (status)
    {
        return (adr);
    }
    else
    {
        return (adr + sz); // Finished without Errors
    }
}
#endif

#if 1
/*********************************************************************
 *
 *       BlankCheck
 *
 *  Function description
 *    Checks if a memory region is blank
 *
 *  Parameters
 *    Addr: Blank check start address
 *    NumBytes: Number of bytes to be checked
 *    BlankData: Pointer to the destination data
 *
 *  Return value
 *    0 O.K., blank
 *    1 O.K., *not* blank
 *    <  0 Error
 *
 */

int BlankCheck(unsigned long Addr, unsigned long NumBytes, unsigned char BlankData)
{
    if (0 != FLASH_VerifyErase(&flashInstance, (Addr & 0x0FFFFFFF), NumBytes))
    {
        return 1;
    }

    return 0;
}

#endif

status_t CLOCK_SetupFROHFClocking(uint32_t iFreq)
{
    if ((iFreq != 48000000U) && (iFreq != 144000000U))
    {
        return kStatus_Fail;
    }

    /* Select 48MHz or 144MHz for FIRC clock */
    SCG0->FIRCCFG = SCG_FIRCCFG_RANGE((iFreq == 48000000U) ? 0 : 1);

    /* Unlock FIRCCSR */
    SCG0->FIRCCSR &= ~SCG_FIRCCSR_LK_MASK;

    if (iFreq == 48000000U)
    {
        /* Enable FIRC 48 MHz clock for peripheral use */
        SCG0->FIRCCSR |= SCG_FIRCCSR_FIRC_SCLK_PERIPH_EN_MASK;
    }
    else if (iFreq == 144000000U)
    {
        /* Enable FIRC 144 MHz clock for peripheral use */
        SCG0->FIRCCSR |= SCG_FIRCCSR_FIRC_FCLK_PERIPH_EN_MASK;
    }

    /* Enable FIRC */
    SCG0->FIRCCSR |= SCG_FIRCCSR_FIRCEN_MASK;

    /* Wait for FIRC clock to be valid. */
    while (!(SCG0->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK))
    {
    }

    return kStatus_Success;
}
