/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_ccm32k.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.ccm32k"
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * brief Set the frequency trim value of 32kHz free-running oscillator by software.
 *
 * param base CCM32K peripheral base address.
 * param trimValue The frequency trim value.
 */
void CCM32K_Set32kFroTrimValue(CCM32K_Type *base, uint16_t trimValue)
{
    uint32_t tmp32;
    bool froEnabled;

    froEnabled = ((base->FRO32K_CTRL & CCM32K_FRO32K_CTRL_FRO_EN_MASK) != 0UL) ? true : false;
    if (froEnabled)
    {
        /* If the free-running oscillator is enabled, disable it temporarily. */
        CCM32K_Enable32kFro(base, false);
    }

    tmp32 = base->FRO32K_TRIM;
    tmp32 &= ~CCM32K_FRO32K_TRIM_FREQ_TRIM_MASK;
    tmp32 |= CCM32K_FRO32K_TRIM_FREQ_TRIM(trimValue);
    base->FRO32K_TRIM = tmp32;

    if (froEnabled)
    {
        /* If the free-running oscillator is enabled previously, remember to enable it again. */
        CCM32K_Enable32kFro(base, true);
    }
}

/*!
 * brief Config 32k Crystal Oscillator.
 *
 * note When the mode selected as \ref kCCM32K_Disable32kHzCrystalOsc or \ref kCCM32K_Bypass32kHzCrystalOsc
 *       the parameter config is useless, so it can be set as "NULL".
 *
 * param base CCM32K peripheral base address.
 * param mode The mode of 32k crystal oscillator.
 * param config The pointer to the structure \ref ccm32k_osc_config_t.
 */
void CCM32K_Set32kOscConfig(CCM32K_Type *base, ccm32k_osc_mode_t mode, const ccm32k_osc_config_t *config)
{
    uint32_t tmp32;

    if (mode == kCCM32K_Disable32kHzCrystalOsc)
    {
        base->OSC32K_CTRL &= ~(CCM32K_OSC32K_CTRL_OSC_MODE_MASK | CCM32K_OSC32K_CTRL_SOX_EN_MASK);
        while ((base->STATUS & CCM32K_STATUS_OSC32K_ACTIVE_MASK) != 0UL)
        {
        }
    }
    else if (mode == kCCM32K_Bypass32kHzCrystalOsc)
    {
        base->OSC32K_CTRL |= CCM32K_OSC32K_CTRL_OSC_MODE_MASK;
        while ((base->STATUS & CCM32K_STATUS_OSC32K_ACTIVE_MASK) == 0UL)
        {
        }
    }
    else
    {
        tmp32 = base->OSC32K_CTRL;

        if (config != NULL)
        {
            if (config->enableInternalCapBank)
            {
                tmp32 &= ~(CCM32K_OSC32K_CTRL_EXTAL_CAP_SEL_MASK | CCM32K_OSC32K_CTRL_XTAL_CAP_SEL_MASK);
                tmp32 |= CCM32K_OSC32K_CTRL_EXTAL_CAP_SEL(config->extalCap) |
                         CCM32K_OSC32K_CTRL_XTAL_CAP_SEL(config->xtalCap);
                tmp32 |= CCM32K_OSC32K_CTRL_CAP_SEL_EN_MASK;
            }
            else
            {
                /* Disable the internal capacitance bank. */
                tmp32 &= ~CCM32K_OSC32K_CTRL_CAP_SEL_EN_MASK;
            }

            tmp32 &= ~(CCM32K_OSC32K_CTRL_COARSE_AMP_GAIN_MASK);
            tmp32 |= CCM32K_OSC32K_CTRL_COARSE_AMP_GAIN(config->coarseAdjustment);
#if (defined(FSL_FEATURE_CCM32K_HAS_FINE_AMP_GAIN) && FSL_FEATURE_CCM32K_HAS_FINE_AMP_GAIN)
            tmp32 &= ~(CCM32K_OSC32K_CTRL_FINE_AMP_GAIN_MASK);
            tmp32 |= CCM32K_OSC32K_CTRL_FINE_AMP_GAIN(config->fineAdjustment);
#endif /* FSL_FEATURE_CCM32K_HAS_FINE_AMP_GAIN */
        }

        tmp32 |= CCM32K_OSC32K_CTRL_OSC_MODE(mode) | CCM32K_OSC32K_CTRL_SOX_EN_MASK;
        base->OSC32K_CTRL = tmp32;

#if (defined(FSL_FEATURE_CCM32K_HAS_CGC32K) && FSL_FEATURE_CCM32K_HAS_CGC32K)
        while ((base->STATUS & (uint32_t)kCCM32K_32kOscReadyStatusFlag) == 0UL)
        {
        }
#else
        while (base->STATUS != ((uint32_t)kCCM32K_32kOscReadyStatusFlag | (uint32_t)kCCM32K_32kOscActiveStatusFlag))
        {
        }
#endif /* FSL_FEATURE_CCM32K_HAS_CGC32K */
    }
}

#if (defined(FSL_FEATURE_CCM32K_HAS_CLKMON_CTRL) && FSL_FEATURE_CCM32K_HAS_CLKMON_CTRL)
/*!
 * brief Config clock monitor one time, including frequency trim value, divide trim value.
 *
 * param base CCM32K peripheral base address.
 * param config Pointer to @ref ccm32k_clock_monitor_config_t structure.
 */
void CCM32K_SetClockMonitorConfig(CCM32K_Type *base, const ccm32k_clock_monitor_config_t *config)
{
    assert(config);

    if (config->enableClockMonitor)
    {
        CCM32K_EnableClockMonitor(base, false);
    }

    CCM32K_SetClockMonitorFreqTrimValue(base, config->freqTrimValue);
    CCM32K_SetClockMonitorDivideTrimValue(base, config->divideTrimValue);
    CCM32K_EnableClockMonitor(base, config->enableClockMonitor);
}
#endif /* FSL_FEATURE_CCM32K_HAS_CLKMON_CTRL */

/*!
 * brief Get current state.
 *
 * param base CCM32K peripheral base address.
 * return The CCM32K's current state, please refer to \ref ccm32k_state_t for details.
 */
ccm32k_state_t CCM32K_GetCurrentState(CCM32K_Type *base)
{
    uint8_t state = 0U;

    if ((base->FRO32K_CTRL & CCM32K_FRO32K_CTRL_FRO_EN_MASK) != 0UL)
    {
        state |= (uint8_t)kCCM32K_Only32kFroEnabled;
    }

    if ((base->OSC32K_CTRL & CCM32K_OSC32K_CTRL_OSC_EN_MASK) != 0UL)
    {
        state |= (uint8_t)kCCM32K_Only32kOscEnabled;
    }

    return (ccm32k_state_t)state;
}

/*!
 * brief Return current clock source.
 *
 * param base CCM32K peripheral base address.
 * retval kCCM32K_ClockSourceNone The none clock source is selected.
 * retval kCCM32K_ClockSource32kFro 32kHz free-running oscillator is selected as clock source.
 * retval kCCM32K_ClockSource32kOsc 32kHz crystal oscillator is selected as clock source..
 */
ccm32k_clock_source_t CCM32K_GetClockSource(CCM32K_Type *base)
{
    uint32_t statusFlag;
    ccm32k_clock_source_t clockSource = kCCM32K_ClockSourceNone;

    statusFlag = CCM32K_GetStatusFlag(base);

    if (statusFlag == ((uint32_t)kCCM32K_32kOscActiveStatusFlag | (uint32_t)kCCM32K_32kOscReadyStatusFlag))
    {
        clockSource = kCCM32K_ClockSource32kOsc;
    }
    else if (statusFlag == (uint32_t)kCCM32K_32kFroActiveStatusFlag)
    {
        clockSource = kCCM32K_ClockSource32kFro;
    }
    else if (statusFlag == 0UL)
    {
        clockSource = kCCM32K_ClockSourceNone;
    }
    else
    {
        /*For MISRA C-2012 Rule 15.7*/
    }

    return clockSource;
}
