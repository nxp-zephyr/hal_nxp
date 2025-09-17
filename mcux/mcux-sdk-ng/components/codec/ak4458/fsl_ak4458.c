/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_ak4458.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief ak4458 device opertaion pointer */
const codec_operation_t ak4458 = {
    .Init          = AK4458_Init,
    .SetFormat     = AK4458_ConfigDataFormat,
    .ModuleControl = AK4458_ModuleControl,
    .Deinit        = AK4458_Deinit,
};
/*******************************************************************************
 * Code
 ******************************************************************************/
static void Delay(void)
{
    uint32_t i;
    for (i = 0; i < 1000; i++)
    {
        __NOP();
    }
}

void AK4458_DefaultConfig(ak4458_config_t *config)
{
    config->ak4458Mode = kAK4458_PcmMode;
    config->dacSelect  = kAK4458_DacEnableAll;
    /* PCM mode setting. */
    config->pcmConfig.pcmSampleFreqMode = kAK4458_AutoSettingMode;
    config->pcmConfig.pcmSdataFormat    = kAK4458_32BitI2S;
    config->pcmConfig.pcmTdmMode        = kAK4458_Normal;
    config->pcmConfig.pcmSdsSlot        = kAK4458_L1R1;
    /* DSD mode setting. */
    config->dsdConfig.dsdMclk         = kAK4458_mclk512fs;
    config->dsdConfig.dsdPlaybackPath = kAK4458_NormalPath;
    config->dsdConfig.dsdDataMute     = kAK4458_DsdMuteDisable;
    config->dsdConfig.dsdDclkPolarity = kAK4458_FallingEdge;
    /* DAC1 setting. */
    config->dac1.OutputPhaseMode = kAK4458_AllDisable;
    config->dac1.DacMode         = kAK4458_Stereo;
    config->dac1.dataChannelMode = kAK4458_NormalMode;
    /* DAC2 setting. */
    config->dac2.OutputPhaseMode = kAK4458_AllDisable;
    config->dac2.DacMode         = kAK4458_Stereo;
    config->dac2.dataChannelMode = kAK4458_NormalMode;
    /* DAC3 setting. */
    config->dac3.OutputPhaseMode = kAK4458_AllDisable;
    config->dac3.DacMode         = kAK4458_Stereo;
    config->dac3.dataChannelMode = kAK4458_NormalMode;
    /* DAC4 setting. */
    config->dac4.OutputPhaseMode = kAK4458_AllDisable;
    config->dac4.DacMode         = kAK4458_Stereo;
    config->dac4.dataChannelMode = kAK4458_NormalMode;
}

status_t AK4458_Init(codec_handle_t *handle, void *ak4458_config)
{
    ak4458_config_t *config = (ak4458_config_t *)ak4458_config;

    /* use default slave address if application not specify */
    if (handle->config->slaveAddress == 0U)
    {
        handle->config->slaveAddress = AK4458_I2C_ADDR;
    }

    AK4458_ModifyReg(handle, AK4458_CONTROL2, AK4458_CONTROL2_SMUTE_MASK,
                     1U << AK4458_CONTROL2_SMUTE_SHIFT); /* Soft ware mute */

    AK4458_ModifyReg(handle, AK4458_CONTROL6, AK4458_CONTROL6_PW1_MASK,
                     ((config->dacSelect & 0x2) >> 1U) << AK4458_CONTROL6_PW1_SHIFT);  /* Enable or disable DAC1 */
    AK4458_ModifyReg(handle, AK4458_CONTROL6, AK4458_CONTROL6_PW2_MASK,
                     ((config->dacSelect & 0x4) >> 2U) << AK4458_CONTROL6_PW2_SHIFT);  /* Enable or disable DAC2 */
    AK4458_ModifyReg(handle, AK4458_CONTROL7, AK4458_CONTROL7_PW3_MASK,
                     ((config->dacSelect & 0x8) >> 3U) << AK4458_CONTROL7_PW3_SHIFT);  /* Enable or disable DAC3 */
    AK4458_ModifyReg(handle, AK4458_CONTROL7, AK4458_CONTROL7_PW4_MASK,
                     ((config->dacSelect & 0x10) >> 4U) << AK4458_CONTROL7_PW4_SHIFT); /* Enable or disable DAC4 */
    /* Only do the configuration when the DAC is enabled. */
    if (config->dacSelect & 0x2) // DAC1
    {
        AK4458_ModifyReg(handle, AK4458_CONTROL4, AK4458_CONTROL4_INVL1_MASK | AK4458_CONTROL4_INVR1_MASK,
                         config->dac1.OutputPhaseMode << AK4458_CONTROL4_INVR1_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL3, AK4458_CONTROL3_MONO1_MASK,
                         config->dac1.DacMode << AK4458_CONTROL3_MONO1_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL3, AK4458_CONTROL3_SELLR1_MASK,
                         config->dac1.dataChannelMode << AK4458_CONTROL3_SELLR1_SHIFT);
    }
    if (config->dacSelect & 0x4) // DAC2
    {
        AK4458_ModifyReg(handle, AK4458_CONTROL4, AK4458_CONTROL4_INVL2_MASK | AK4458_CONTROL4_INVR2_MASK,
                         config->dac2.OutputPhaseMode << AK4458_CONTROL4_INVR2_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL9, AK4458_CONTROL9_MONO2_MASK,
                         config->dac2.DacMode << AK4458_CONTROL9_MONO2_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL4, AK4458_CONTROL4_SELLR2_MASK,
                         config->dac2.dataChannelMode << AK4458_CONTROL4_SELLR2_SHIFT);
    }
    if (config->dacSelect & 0x8) // DAC3
    {
        AK4458_ModifyReg(handle, AK4458_CONTROL8, AK4458_CONTROL8_INVL3_MASK | AK4458_CONTROL8_INVR3_MASK,
                         config->dac3.OutputPhaseMode << AK4458_CONTROL8_INVR3_SHIFT |
                             ((config->dac3.OutputPhaseMode & 0x2) >> 1U) << AK4458_CONTROL8_INVR3_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL9, AK4458_CONTROL9_MONO3_MASK,
                         config->dac3.DacMode << AK4458_CONTROL9_MONO3_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL9, AK4458_CONTROL9_SELLR3_MASK,
                         config->dac3.dataChannelMode << AK4458_CONTROL9_SELLR3_SHIFT);
    }
    if (config->dacSelect & 0x10) // DAC4
    {
        AK4458_ModifyReg(handle, AK4458_CONTROL8, AK4458_CONTROL8_INVL4_MASK | AK4458_CONTROL8_INVR4_MASK,
                         config->dac4.OutputPhaseMode << AK4458_CONTROL8_INVR4_SHIFT |
                             ((config->dac4.OutputPhaseMode & 0x2) >> 1U) << AK4458_CONTROL8_INVR4_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL9, AK4458_CONTROL9_MONO4_MASK,
                         config->dac4.DacMode << AK4458_CONTROL9_MONO4_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL9, AK4458_CONTROL9_SELLR4_MASK,
                         config->dac4.dataChannelMode << AK4458_CONTROL9_SELLR4_SHIFT);
    }

    if (config->ak4458Mode == kAK4458_PcmMode) /* PCM mode*/
    {
        AK4458_ModifyReg(handle, AK4458_CONTROL1,
                         AK4458_CONTROL1_DIF0_MASK | AK4458_CONTROL1_DIF1_MASK | AK4458_CONTROL1_DIF2_MASK,
                         config->pcmConfig.pcmSdataFormat << AK4458_CONTROL1_DIF0_SHIFT);

        if (config->pcmConfig.pcmSampleFreqMode != kAK4458_ManualSettingMode)
        {
            if (config->pcmConfig.pcmSampleFreqMode == kAK4458_AutoSettingMode)
            {
                AK4458_ModifyReg(handle, AK4458_CONTROL1, AK4458_CONTROL1_ACKS_MASK, 1U << AK4458_CONTROL1_ACKS_SHIFT);
            }
        }
        AK4458_ModifyReg(handle, AK4458_CONTROL6, AK4458_CONTROL6_TDM0_MASK | AK4458_CONTROL6_TDM1_MASK,
                         config->pcmConfig.pcmTdmMode << AK4458_CONTROL6_TDM0_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL8, AK4458_CONTROL7_SDS0_MASK,
                         (config->pcmConfig.pcmSdsSlot & 0x1) << AK4458_CONTROL7_SDS0_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL7, AK4458_CONTROL6_SDS1_MASK,
                         ((config->pcmConfig.pcmSdsSlot & 0x2) >> 1U) << AK4458_CONTROL6_SDS1_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL7, AK4458_CONTROL6_SDS2_MASK,
                         ((config->pcmConfig.pcmSdsSlot & 0x4) >> 2U) << AK4458_CONTROL6_SDS2_SHIFT);
    }
    else /*DSD mode*/
    {
        AK4458_ModifyReg(handle, AK4458_CONTROL3, AK4458_CONTROL3_DP_MASK, 1U << AK4458_CONTROL3_DP_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL3, AK4458_CONTROL3_DCKS_MASK,
                         config->dsdConfig.dsdMclk << AK4458_CONTROL3_DCKS_SHIFT);
        AK4458_ModifyReg(handle, AK4458_DSD1, AK4458_DSD1_DSDD_MASK,
                         config->dsdConfig.dsdPlaybackPath << AK4458_DSD1_DSDD_SHIFT);
        AK4458_ModifyReg(handle, AK4458_DSD1, AK4458_DSD1_DDM_MASK,
                         config->dsdConfig.dsdDataMute << AK4458_DSD1_DDM_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL3, AK4458_CONTROL3_DCKB_MASK,
                         config->dsdConfig.dsdDclkPolarity << AK4458_CONTROL3_DCKB_SHIFT);
    }

    AK4458_ModifyReg(handle, AK4458_CONTROL2, AK4458_CONTROL2_SMUTE_MASK,
                     0U << AK4458_CONTROL2_SMUTE_SHIFT); /* Normal Operation */

    AK4458_ModifyReg(handle, AK4458_CONTROL1, AK4458_CONTROL1_RSTN_MASK,
                     0U << AK4458_CONTROL1_RSTN_SHIFT); /* Rest the ak4458 */
    Delay(); /* Need to wait to ensure the ak4458 has updated the above registers. */
    AK4458_ModifyReg(handle, AK4458_CONTROL1, AK4458_CONTROL1_RSTN_MASK,
                     1U << AK4458_CONTROL1_RSTN_SHIFT); /* Normal Operation */
    Delay();

    return kStatus_Success;
}

status_t AK4458_SetEncoding(codec_handle_t *handle, uint8_t format)
{
    ak4458_config_t *config = (ak4458_config_t *)handle->config->codecConfig;

    if (format == kAK4458_DsdMode)
    {
        config->ak4458Mode = kAK4458_DsdMode;
        AK4458_ModifyReg(handle, AK4458_CONTROL3, AK4458_CONTROL3_DP_MASK, 1U << AK4458_CONTROL3_DP_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL3, AK4458_CONTROL3_DCKS_MASK,
                         config->dsdConfig.dsdMclk << AK4458_CONTROL3_DCKS_SHIFT);
        AK4458_ModifyReg(handle, AK4458_DSD1, AK4458_DSD1_DSDD_MASK,
                         config->dsdConfig.dsdPlaybackPath << AK4458_DSD1_DSDD_SHIFT);
        AK4458_ModifyReg(handle, AK4458_DSD1, AK4458_DSD1_DDM_MASK,
                         config->dsdConfig.dsdDataMute << AK4458_DSD1_DDM_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL3, AK4458_CONTROL3_DCKB_MASK,
                         config->dsdConfig.dsdDclkPolarity << AK4458_CONTROL3_DCKB_SHIFT);
    }

    if (format == kAK4458_PcmMode)
    {
        config->ak4458Mode = kAK4458_PcmMode;
        AK4458_ModifyReg(handle, AK4458_CONTROL6, AK4458_CONTROL6_TDM0_MASK | AK4458_CONTROL6_TDM1_MASK,
                         config->pcmConfig.pcmTdmMode << AK4458_CONTROL6_TDM0_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL8, AK4458_CONTROL7_SDS0_MASK,
                         (config->pcmConfig.pcmSdsSlot & 0x1) << AK4458_CONTROL7_SDS0_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL7, AK4458_CONTROL6_SDS1_MASK,
                         ((config->pcmConfig.pcmSdsSlot & 0x2) >> 1U) << AK4458_CONTROL6_SDS1_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL7, AK4458_CONTROL6_SDS2_MASK,
                         ((config->pcmConfig.pcmSdsSlot & 0x4) >> 2U) << AK4458_CONTROL6_SDS2_SHIFT);
        AK4458_ModifyReg(handle, AK4458_CONTROL3, AK4458_CONTROL3_DP_MASK, 0U << AK4458_CONTROL3_DP_SHIFT);
    }

    return kStatus_Success;
}

status_t AK4458_ConfigDataFormat(codec_handle_t *handle, uint32_t mclk, uint32_t sampleRate, uint32_t bitWidth)
{
    ak4458_pcm_samplefreqselect_t samplefreq;
    ak4458_dsd_dclk_t dsdsel;
    ak4458_pcm_sdata_format_t sdataFormat;
    ak4458_config_t *config = (ak4458_config_t *)handle->config->codecConfig;
    if (config->ak4458Mode == kAK4458_DsdMode)
    {
        switch (sampleRate * bitWidth)
        {
            case 2048000U:
            case 2822400U:
            case 3072000U:
                dsdsel = kAK4458_dclk64fs;
                break;
            case 4096000U:
            case 5644800U:
            case 6144000U:
                dsdsel = kAK4458_dclk128fs;
                break;
            case 8192000U:
            case 11289600U:
            case 12288000U:
                dsdsel = kAK4458_dclk256fs;
                break;
            default:
                return kStatus_Fail;
        }
        AK4458_ModifyReg(handle, AK4458_DSD1, AK4458_DSD1_DSDSEL0_MASK,
                         (dsdsel & 0x1) << AK4458_DSD1_DSDSEL0_SHIFT);         /* Set DSDSEL0 */
        AK4458_ModifyReg(handle, AK4458_DSD2, AK4458_DSD2_DSDSEL1_MASK,
                         ((dsdsel & 0x2) >> 1U) << AK4458_DSD2_DSDSEL1_SHIFT); /* Set DSDSEL1 */
    }
    else                                                                       /* PCM mode */
    {
        switch (sampleRate)
        {
            case 8000U:
            case 11025U:
            case 16000U:
            case 22050U:
            case 32000U:
            case 44100U:
            case 48000U:
                samplefreq = kAK4458_NormalSpeed;
                break;
            case 88200U:
            case 96000U:
                samplefreq = kAK4458_DoubleSpeed;
                break;
            case 176400U:
            case 192000U:
                samplefreq = kAK4458_QuadSpeed;
                break;
            case 352800U:
            case 384000U:
                samplefreq = kAK4458_OctSpeed;
                break;
            case 705600U:
            case 768000U:
                samplefreq = kAK4458_HexSpeed;
                break;
            default:
                return kStatus_Fail;
        }
        switch (bitWidth)
        {
            /* For PCM, only strero mode supported. */
            case 16U:
            case 24U:
                sdataFormat = kAK4458_16_24BitI2S;
                break;
            case 32U:
                sdataFormat = kAK4458_32BitI2S;
                break;
            default:
                return kStatus_Fail;
        }
        AK4458_ModifyReg(handle, AK4458_CONTROL2, AK4458_CONTROL2_DFS0_MASK | AK4458_CONTROL2_DFS1_MASK,
                         (samplefreq & 0x3) << AK4458_CONTROL2_DFS0_SHIFT);         /* Set DFS[1:0] */
        AK4458_ModifyReg(handle, AK4458_CONTROL4, AK4458_CONTROL4_DFS2_MASK | AK4458_CONTROL4_DFS2_MASK,
                         ((samplefreq & 0x4) >> 2U) << AK4458_CONTROL4_DFS2_SHIFT); /* Set DFS[2] */
        AK4458_ModifyReg(handle, AK4458_CONTROL1,
                         AK4458_CONTROL1_DIF0_MASK | AK4458_CONTROL1_DIF1_MASK | AK4458_CONTROL1_DIF2_MASK,
                         sdataFormat << AK4458_CONTROL1_DIF0_SHIFT);
    }

    AK4458_ModifyReg(handle, AK4458_CONTROL1, AK4458_CONTROL1_RSTN_MASK,
                     0U << AK4458_CONTROL1_RSTN_SHIFT); /* Rest the ak4458 */

    Delay();

    AK4458_ModifyReg(handle, AK4458_CONTROL1, AK4458_CONTROL1_RSTN_MASK,
                     1U << AK4458_CONTROL1_RSTN_SHIFT); /* Normal Operation */
    Delay();

    return kStatus_Success;
}

status_t AK4458_SetDAC1Volume(codec_handle_t *handle, uint8_t value)
{
    status_t retval = kStatus_Success;
    /*
     * 255 levels, 0.5dB setp + mute (value = 0)
     */
    retval = AK4458_WriteReg(handle, AK4458_L1CHATT, value);
    retval = AK4458_WriteReg(handle, AK4458_R1CHATT, value);

    return retval;
}

status_t AK4458_GetDAC1Volume(codec_handle_t *handle, uint8_t *value)
{
    status_t retval = kStatus_Success;
    /*
     * 255 levels, 0.5dB setp + mute (value = 0);
     * R-channel volume regarded the same as the L-channel, here just read the L-channel value.
     */
    retval = AK4458_ReadReg(handle, AK4458_L1CHATT, value);

    return retval;
}

status_t AK4458_ModuleControl(codec_handle_t *handle, codec_module_ctrl_cmd_t cmd, uint32_t data)
{
    status_t ret = kStatus_Success;

    switch (cmd)
    {
        case kCODEC_ModuleSwitchI2SInInterface:
            ret = AK4458_SetEncoding(handle, data);
            break;
        default:
            return kStatus_InvalidArgument;
    }

    return ret;
}

status_t AK4458_Deinit(codec_handle_t *handle)
{
    AK4458_ModifyReg(handle, AK4458_CONTROL2, AK4458_CONTROL2_SMUTE_MASK,
                     1U << AK4458_CONTROL2_SMUTE_SHIFT); /* Soft ware mute */

    return kStatus_Success;
}
status_t AK4458_WriteReg(codec_handle_t *handle, uint8_t reg, uint8_t val)
{
    assert(handle->config);
    assert(handle->config->I2C_SendFunc);

    status_t retval = kStatus_Success;
    Delay(); /* Ensure the Codec I2C bus free before writing the slave. */

    retval = handle->config->I2C_SendFunc(handle->config->slaveAddress, reg, kCODEC_RegAddr8Bit, (uint8_t const *)&val,
                                          kCODEC_RegWidth8Bit);
    return retval;
}

status_t AK4458_ReadReg(codec_handle_t *handle, uint8_t reg, uint8_t *val)
{
    assert(handle->config);
    assert(handle->config->I2C_SendFunc);

    status_t retval = kStatus_Success;
    Delay(); /* Ensure the Codec I2C bus free before reading the slave. */

    retval = handle->config->I2C_ReceiveFunc(handle->config->slaveAddress, reg, kCODEC_RegAddr8Bit, val,
                                             kCODEC_RegWidth8Bit);

    return retval;
}

status_t AK4458_ModifyReg(codec_handle_t *handle, uint8_t reg, uint8_t mask, uint8_t val)
{
    status_t retval = kStatus_Success;
    uint8_t reg_val = 0;
    retval          = AK4458_ReadReg(handle, reg, &reg_val);
    if (retval != kStatus_Success)
    {
        return kStatus_Fail;
    }
    reg_val &= (uint8_t)~mask;
    reg_val |= val;
    retval = AK4458_WriteReg(handle, reg, reg_val);
    if (retval != kStatus_Success)
    {
        return kStatus_Fail;
    }
    return kStatus_Success;
}
