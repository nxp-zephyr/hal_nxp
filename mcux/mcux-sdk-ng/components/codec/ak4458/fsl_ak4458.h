/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_AK4458_H_
#define _FSL_AK4458_H_

#include "fsl_common.h"
#include "fsl_codec_common.h"
/*!
 * @addtogroup ak4458
 * @ingroup codec
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @name Driver version */
/*! @{ */
/*! @brief CLOCK driver version 2.1.0 */
#define FSL_AK4458_DRIVER_VERSION (MAKE_VERSION(2, 1, 0))
/*! @} */

/*! @brief define the registers offset of AK4458. */
#define AK4458_CONTROL1     (0x00)
#define AK4458_CONTROL2     (0x01)
#define AK4458_CONTROL3     (0x02)
#define AK4458_L1CHATT      (0x03)
#define AK4458_R1CHATT      (0x04)
#define AK4458_CONTROL4     (0x05)
#define AK4458_DSD1         (0x06)
#define AK4458_CONTROL5     (0x07)
#define AK4458_SOUNDCONTROL (0x08)
#define AK4458_DSD2         (0x09)
#define AK4458_CONTROL6     (0x0A)
#define AK4458_CONTROL7     (0x0B)
#define AK4458_CONTROL8     (0x0C)
#define AK4458_CONTROL9     (0x0D)
#define AK4458_CONTROL10    (0x0E)
#define AK4458_L2CHATT      (0x0F)
#define AK4458_R2CHATT      (0x10)
#define AK4458_L3CHATT      (0x11)
#define AK4458_R3CHATT      (0x12)
#define AK4458_L4CHATT      (0x13)
#define AK4458_R4CHATT      (0x14)
/*! @brief define the registers offset of AK4458. */
#define AK4458_CONTROL1_RSTN_MASK  (0x1U)
#define AK4458_CONTROL1_RSTN_SHIFT (0U)
#define AK4458_CONTROL1_DIF0_MASK  (0x2U)
#define AK4458_CONTROL1_DIF0_SHIFT (1U)
#define AK4458_CONTROL1_DIF1_MASK  (0x4U)
#define AK4458_CONTROL1_DIF1_SHIFT (2U)
#define AK4458_CONTROL1_DIF2_MASK  (0x8U)
#define AK4458_CONTROL1_DIF2_SHIFT (3U)
#define AK4458_CONTROL1_ACKS_MASK  (0x80U)
#define AK4458_CONTROL1_ACKS_SHIFT (7U)

#define AK4458_CONTROL2_SMUTE_MASK  (0x1U)
#define AK4458_CONTROL2_SMUTE_SHIFT (0U)
#define AK4458_CONTROL2_DEM10_MASK  (0x2U)
#define AK4458_CONTROL2_DEM10_SHIFT (1U)
#define AK4458_CONTROL2_DEM11_MASK  (0x4U)
#define AK4458_CONTROL2_DEM11_SHIFT (2U)
#define AK4458_CONTROL2_DFS0_MASK   (0x8U)
#define AK4458_CONTROL2_DFS0_SHIFT  (3U)
#define AK4458_CONTROL2_DFS1_MASK   (0x10U)
#define AK4458_CONTROL2_DFS1_SHIFT  (4U)
#define AK4458_CONTROL2_SD_MASK     (0x20U)
#define AK4458_CONTROL2_SD_SHIFT    (5U)

#define AK4458_CONTROL3_SLOW_MASK    (0x1U)
#define AK4458_CONTROL3_SLOW_SHIFT   (0U)
#define AK4458_CONTROL3_SELLR1_MASK  (0x2U)
#define AK4458_CONTROL3_SELLR1_SHIFT (1U)
#define AK4458_CONTROL3_DZFB_MASK    (0x4U)
#define AK4458_CONTROL3_DZFB_SHIFT   (2U)
#define AK4458_CONTROL3_MONO1_MASK   (0x8U)
#define AK4458_CONTROL3_MONO1_SHIFT  (3U)
#define AK4458_CONTROL3_DCKB_MASK    (0x10U)
#define AK4458_CONTROL3_DCKB_SHIFT   (4U)
#define AK4458_CONTROL3_DCKS_MASK    (0x20U)
#define AK4458_CONTROL3_DCKS_SHIFT   (5U)
#define AK4458_CONTROL3_DP_MASK      (0x80U)
#define AK4458_CONTROL3_DP_SHIFT     (7U)

#define AK4458_CONTROL4_SSLOW_MASK   (0x1U)
#define AK4458_CONTROL4_SSLOW_SHIFT  (0U)
#define AK4458_CONTROL4_DFS2_MASK    (0x2U)
#define AK4458_CONTROL4_DFS2_SHIFT   (1U)
#define AK4458_CONTROL4_SELLR2_MASK  (0x8U)
#define AK4458_CONTROL4_SELLR2_SHIFT (3U)
#define AK4458_CONTROL4_INVR2_MASK   (0x10U)
#define AK4458_CONTROL4_INVR2_SHIFT  (4U)
#define AK4458_CONTROL4_INVL2_MASK   (0x20U)
#define AK4458_CONTROL4_INVL2_SHIFT  (5U)
#define AK4458_CONTROL4_INVR1_MASK   (0x40U)
#define AK4458_CONTROL4_INVR1_SHIFT  (6U)
#define AK4458_CONTROL4_INVL1_MASK   (0x80U)
#define AK4458_CONTROL4_INVL1_SHIFT  (7U)

#define AK4458_DSD1_DSDSEL0_MASK  (0x1U)
#define AK4458_DSD1_DSDSEL0_SHIFT (0U)
#define AK4458_DSD1_DSDD_MASK     (0x2U)
#define AK4458_DSD1_DSDD_SHIFT    (1U)
#define AK4458_DSD1_DMRE_MASK     (0x8U)
#define AK4458_DSD1_DMRE_SHIFT    (3U)
#define AK4458_DSD1_DMC_MASK      (0x10U)
#define AK4458_DSD1_DMC_SHIFT     (4U)
#define AK4458_DSD1_DMR1_MASK     (0x20U)
#define AK4458_DSD1_DMR1_SHIFT    (5U)
#define AK4458_DSD1_DML1_MASK     (0x40U)
#define AK4458_DSD1_DML1_SHIFT    (6U)
#define AK4458_DSD1_DDM_MASK      (0x80U)
#define AK4458_DSD1_DDM_SHIFT     (7U)

#define AK4458_CONTROL5_SYNCE_MASK  (0x1U)
#define AK4458_CONTROL5_SYNCE_SHIFT (0U)
#define AK4458_CONTROL5_R4_MASK     (0x10U)
#define AK4458_CONTROL5_R4_SHIFT    (4U)
#define AK4458_CONTROL5_L4_MASK     (0x20U)
#define AK4458_CONTROL5_L4_SHIFT    (5U)
#define AK4458_CONTROL5_R3_MASK     (0x40U)
#define AK4458_CONTROL5_R3_SHIFT    (6U)
#define AK4458_CONTROL5_L3_MASK     (0x80U)
#define AK4458_CONTROL5_L3_SHIFT    (7U)

#define AK4458_SOUNDCONTROL_SC0_MASK  (0x1U)
#define AK4458_SOUNDCONTROL_SC0_SHIFT (0U)
#define AK4458_SOUNDCONTROL_SC1_MASK  (0x2U)
#define AK4458_SOUNDCONTROL_SC1_SHIFT (1U)
#define AK4458_SOUNDCONTROL_R2_MASK   (0x10U)
#define AK4458_SOUNDCONTROL_R2_SHIFT  (4U)
#define AK4458_SOUNDCONTROL_L2_MASK   (0x20U)
#define AK4458_SOUNDCONTROL_L2_SHIFT  (5U)
#define AK4458_SOUNDCONTROL_R1_MASK   (0x40U)
#define AK4458_SOUNDCONTROL_R1_SHIFT  (6U)
#define AK4458_SOUNDCONTROL_L1_MASK   (0x80U)
#define AK4458_SOUNDCONTROL_L1_SHIFT  (7U)

#define AK4458_DSD2_DSDSEL1_MASK  (0x1U)
#define AK4458_DSD2_DSDSEL1_SHIFT (0U)
#define AK4458_DSD2_DSDF_MASK     (0x2U)
#define AK4458_DSD2_DSDF_SHIFT    (1U)
#define AK4458_DSD2_DMR4_MASK     (0x4U)
#define AK4458_DSD2_DMR4_SHIFT    (2U)
#define AK4458_DSD2_DML4_MASK     (0x8U)
#define AK4458_DSD2_DML4_SHIFT    (3U)
#define AK4458_DSD2_DMR3_MASK     (0x10U)
#define AK4458_DSD2_DMR3_SHIFT    (4U)
#define AK4458_DSD2_DML3_MASK     (0x20U)
#define AK4458_DSD2_DML3_SHIFT    (5U)
#define AK4458_DSD2_DMR2_MASK     (0x40U)
#define AK4458_DSD2_DMR2_SHIFT    (6U)
#define AK4458_DSD2_DML2_MASK     (0x80U)
#define AK4458_DSD2_DML2_SHIFT    (7U)

#define AK4458_CONTROL6_DEM20_MASK  (0x1U)
#define AK4458_CONTROL6_DEM20_SHIFT (0U)
#define AK4458_CONTROL6_DEM21_MASK  (0x2U)
#define AK4458_CONTROL6_DEM21_SHIFT (1U)
#define AK4458_CONTROL6_PW1_MASK    (0x4U)
#define AK4458_CONTROL6_PW1_SHIFT   (2U)
#define AK4458_CONTROL6_PW2_MASK    (0x8U)
#define AK4458_CONTROL6_PW2_SHIFT   (3U)
#define AK4458_CONTROL6_SDS2_MASK   (0x10U)
#define AK4458_CONTROL6_SDS2_SHIFT  (4U)
#define AK4458_CONTROL6_SDS1_MASK   (0x20U)
#define AK4458_CONTROL6_SDS1_SHIFT  (5U)
#define AK4458_CONTROL6_TDM0_MASK   (0x40U)
#define AK4458_CONTROL6_TDM0_SHIFT  (6U)
#define AK4458_CONTROL6_TDM1_MASK   (0x80U)
#define AK4458_CONTROL6_TDM1_SHIFT  (7U)

#define AK4458_CONTROL7_DCHAIN_MASK  (0x2U)
#define AK4458_CONTROL7_DCHAIN_SHIFT (1U)
#define AK4458_CONTROL7_PW3_MASK     (0x4U)
#define AK4458_CONTROL7_PW3_SHIFT    (2U)
#define AK4458_CONTROL7_PW4_MASK     (0x8U)
#define AK4458_CONTROL7_PW4_SHIFT    (3U)
#define AK4458_CONTROL7_SDS0_MASK    (0x10U)
#define AK4458_CONTROL7_SDS0_SHIFT   (4U)
#define AK4458_CONTROL7_ATS0_MASK    (0x40U)
#define AK4458_CONTROL7_ATS0_SHIFT   (6U)
#define AK4458_CONTROL7_ATS1_MASK    (0x80U)
#define AK4458_CONTROL7_ATS1_SHIFT   (7U)

#define AK4458_CONTROL8_FIR0_MASK   (0x1U)
#define AK4458_CONTROL8_FIR0_SHIFT  (0U)
#define AK4458_CONTROL8_FIR1_MASK   (0x2U)
#define AK4458_CONTROL8_FIR1_SHIFT  (1U)
#define AK4458_CONTROL8_FIR2_MASK   (0x4U)
#define AK4458_CONTROL8_FIR2_SHIFT  (2U)
#define AK4458_CONTROL8_INVL3_MASK  (0x10U)
#define AK4458_CONTROL8_INVL3_SHIFT (4U)
#define AK4458_CONTROL8_INVR3_MASK  (0x20U)
#define AK4458_CONTROL8_INVR3_SHIFT (5U)
#define AK4458_CONTROL8_INVL4_MASK  (0x40U)
#define AK4458_CONTROL8_INVL4_SHIFT (6U)
#define AK4458_CONTROL8_INVR4_MASK  (0x80U)
#define AK4458_CONTROL8_INVR4_SHIFT (7U)

#define AK4458_CONTROL9_SELLR3_MASK  (0x4U)
#define AK4458_CONTROL9_SELLR3_SHIFT (2U)
#define AK4458_CONTROL9_SELLR4_MASK  (0x8U)
#define AK4458_CONTROL9_SELLR4_SHIFT (3U)
#define AK4458_CONTROL9_MONO2_MASK   (0x20U)
#define AK4458_CONTROL9_MONO2_SHIFT  (5U)
#define AK4458_CONTROL9_MONO3_MASK   (0x40U)
#define AK4458_CONTROL9_MONO3_SHIFT  (6U)
#define AK4458_CONTROL9_MONO4_MASK   (0x80U)
#define AK4458_CONTROL9_MONO4_SHIFT  (7U)

#define AK4458_CONTROL10_DEM30_MASK  (0x10U)
#define AK4458_CONTROL10_DEM30_SHIFT (4U)
#define AK4458_CONTROL10_DEM31_MASK  (0x20U)
#define AK4458_CONTROL10_DEM31_SHIFT (5U)
#define AK4458_CONTROL10_DEM40_MASK  (0x40U)
#define AK4458_CONTROL10_DEM40_SHIFT (6U)
#define AK4458_CONTROL10_DEM41_MASK  (0x80U)
#define AK4458_CONTROL10_DEM41_SHIFT (7U)

/*! @brief AK4458 I2C address. */
#define AK4458_I2C_ADDR (0x10)
/*! @brief The numbers of DAC for AK4458 */
#define AK4458_DAC_NUM (4U)

/*! @brief aak4458 operation function pointer */
extern const codec_operation_t ak4458_ops;

/*! @brief The AK4458 playback mode */
typedef enum _ak4458_mode
{
    kAK4458_PcmMode = 0x0,
    kAK4458_DsdMode = 0x1,
} ak4458_mode_t;

/*! @brief The DAC output enable selection of AK4458
 * AK4458 has 4 DAC modules, using DacMask to select which one to be used.
 */
typedef enum _ak4458_dac_selection
{
    kAK4458_DacDiableAll = 1 << 0U,
    kAK4458_Dac1Mask     = 1 << 1U,
    kAK4458_Dac2Mask     = 1 << 2U,
    kAK4458_Dac3Mask     = 1 << 3U,
    kAK4458_Dac4Mask     = 1 << 4U,
    kAK4458_DacEnableAll = 0xF << 1U,
} ak4458_dac_selection_t;

/*! @brief The AOUTR output phase inverting, defined by INVL,INVR*/
typedef enum _ak4458_output_phase_mode
{
    kAK4458_AllDisable    = 0x0, /*!< Disable */
    kAK4458_JustRchEnable = 0x1, /*!< Just R-channel enable output phase inverting */
    kAK4458_JustLchEnable = 0x2, /*!< Just L-channel enable output phase inverting */
    kAK4458_AllEnable     = 0x3, /*!< All enable output phase inverting */
} ak4458_output_phase_mode_t;

/*! @brief The DAC mode, defined by MONO bit */
typedef enum _ak4458_dac_mode
{
    kAK4458_Stereo = 0x0, /*!< Sterreo mode. */
    kAK4458_Mono   = 0x1, /*!< MONO mode. */
} ak4458_dac_mode_t;

/*! @brief The Data selection of L-channel and R-channel for DSD mode, defined by SELLR bit */
typedef enum _ak4458_data_channel_mode
{
    kAK4458_NormalMode   = 0x0, /*!< L-channel output L-channel data, R-channel output R-channel data. */
    kAK4458_ExchangeMode = 0x1, /*!< L-channel output R-channel data, R-channel output L-channel data. */
} ak4458_data_channel_mode_t;
/*! @brief The MCLK select for DSD mode, defined by DCKS bit */
typedef enum _ak4458_dsd_mclk
{
    kAK4458_mclk512fs = 0x0, /*!< MCLK equals 512fs. */
    kAK4458_mclk768fs = 0x1, /*!< MCLK equals 768fs. */
} ak4458_dsd_mclk_t;

/*! @brief The DCLK select for DSD mode, defined by DSDSEL[1:0] */
typedef enum _ak4458_dsd_dclk
{
    kAK4458_dclk64fs  = 0x0, /*!< DCLK equals 64fs. */
    kAK4458_dclk128fs = 0x1, /*!< DCLK equals 128fs. */
    kAK4458_dclk256fs = 0x2, /*!< DCLK equals 256fs. */
} ak4458_dsd_dclk_t;

/*! @brief DSD playback path */
typedef enum _ak4458_dsd_playback_path
{
    kAK4458_NormalPath   = 0x0, /*!< Normal path mode. */
    kAK4458_VolumeBypass = 0x1, /*!< Volume Bypass mode. */
} ak4458_dsd_playback_path_t;

/*! @brief DSD mute flag */
typedef enum _ak4458_dsd_data_mute
{
    kAK4458_DsdMuteDisable = 0x0,
    kAK4458_DsdMuteEnable  = 0x1,
} ak4458_dsd_data_mute_t;

/*! @brief DSD bclk polarity */
typedef enum _ak4458_dsd_dclk_polarity
{
    kAK4458_FallingEdge = 0x0, /*!< DSD data is output from DCLK falling edge. */
    kAK4458_RisingEdge  = 0x1, /*!< DSD data is output from DCLK rising edge. */
} ak4458_dsd_dclk_polarity_t;
/*! @brief The sampling frequency mode for PCM and EXDF mode, defined by CR01[AFSD], CR00[ACKS]*/
typedef enum _ak4458_pcm_samplefreqmode
{
    kAK4458_ManualSettingMode = 0x0, /*!< Manual setting mode */
    kAK4458_AutoSettingMode   = 0x1, /*!< Auto setting mode */
} ak4458_pcm_samplefreqmode_t;
/*! @brief The sampling speed select, defined by DFS[2:0]*/
typedef enum _ak4458_pcm_samplefreqselect
{
    kAK4458_NormalSpeed = 0x0, /*!< 8kHZ ~ 54kHZ */
    kAK4458_DoubleSpeed = 0x1, /*!< 54kHZ ~ 108kHZ */
    kAK4458_QuadSpeed   = 0x2, /*!< 120kHZ ~ 216kHZ, note that value 3 also stands for Quad Speed Mode */
    kAK4458_OctSpeed    = 0x4, /*!< 384kHZ, note that value 6 also stands for Oct Speed Mode */
    kAK4458_HexSpeed    = 0x5, /*!< 768kHZ, note that value 7 also stands for Hex Speed Mode */
} ak4458_pcm_samplefreqselect_t;

/*! @brief The audio data interface modes, defined by DIF[2:0]*/
typedef enum _ak4458_pcm_sdata_format
{
    kAK4458_16BitLSB    = 0x0, /*!< 16-bit LSB justified */
    kAK4458_20BitLSB    = 0x1, /*!< 20-bit LSB justified */
    kAK4458_24BitMSB    = 0x2, /*!< 24-bit MSB justified */
    kAK4458_16_24BitI2S = 0x3, /*!< 16 and 24-bit I2S compatible */
    kAK4458_24BitLSB    = 0x4, /*!< 24-bit LSB justified */
    kAK4458_32BitLSB    = 0x5, /*!< 32-bit LSB justified */
    kAK4458_32BitMSB    = 0x6, /*!< 32-bit MSB justified */
    kAK4458_32BitI2S    = 0x7, /*!< 32-bit I2S compatible */
} ak4458_pcm_sdata_format_t;

/*! @brief The TDM mode select, defined by TDM[1:0]*/
typedef enum _ak4458_pcm_tdm_mode
{
    kAK4458_Normal = 0x0, /*!< Normal mode */
    kAK4458_TDM128 = 0x1, /*!< BCLK is fixed to 128fs */
    kAK4458_TDM256 = 0x2, /*!< BCLK is fixed to 256fs */
    kAK4458_TDM512 = 0x3, /*!< BCLK is fixed to 512fs */
} ak4458_pcm_tdm_mode_t;

/*! @brief The audio data slot selection, defined by SDS[2:0]*/
typedef enum _ak4458_pcm_sds_select
{
    kAK4458_L1R1 = 0x0,
    kAK4458_L2R2 = 0x1,
    kAK4458_L3R3 = 0x2,
    kAK4458_L4R4 = 0x3,
    kAK4458_L5R5 = 0x4,
    kAK4458_L6R6 = 0x5,
    kAK4458_L7R7 = 0x6,
    kAK4458_L8R8 = 0x7,
} ak4458_pcm_sds_select_t;

/*! @brief The De-emphasis filter, defined by DEM[1:0], the mode only valid in PCM Normal Speed Mode*/
typedef enum _ak4458_pcm_deemphasis_mode
{
    kAK4458_Fs44100 = 0x0, /*!< Filter is available for 44.1KHzNormal mode */
    kAK4458_Off     = 0x1, /*!< OFF */
    kAK4458_Fs48000 = 0x2, /*!< Filter is available for 48KHz*/
    kAK4458_Fs32000 = 0x3, /*!< Filter is available for 32KHz*/
} ak4458_pcm_deemphasis_mode_t;

/*! @brief Initialize DAC configuration structure of AK4458 since it has 4 DAC modules */
typedef struct _ak4458_dac_config
{
    ak4458_output_phase_mode_t OutputPhaseMode;
    ak4458_dac_mode_t DacMode;
    ak4458_data_channel_mode_t dataChannelMode;
} ak4458_dac_config_t;

/*! @brief Initialize DSD mode structure of AK4458 */
typedef struct _ak4458_dsd_config
{
    ak4458_dsd_mclk_t dsdMclk;
    ak4458_dsd_playback_path_t dsdPlaybackPath;
    ak4458_dsd_data_mute_t dsdDataMute;
    ak4458_dsd_dclk_polarity_t dsdDclkPolarity;
} ak4458_dsd_config_t;

/*! @brief Initialize PCM mode structure of AK4458 */
typedef struct _ak4458_pcm_config
{
    ak4458_pcm_samplefreqmode_t pcmSampleFreqMode;
    ak4458_pcm_sdata_format_t pcmSdataFormat;
    ak4458_pcm_tdm_mode_t pcmTdmMode;
    ak4458_pcm_sds_select_t pcmSdsSlot;
} ak4458_pcm_config_t;

/*! @brief Initialize structure of AK4458 */
typedef struct _ak4458_config
{
    ak4458_mode_t ak4458Mode;
    ak4458_dac_selection_t dacSelect;
    ak4458_pcm_config_t pcmConfig;
    ak4458_dsd_config_t dsdConfig;
    ak4458_dac_config_t dac1;
    ak4458_dac_config_t dac2;
    ak4458_dac_config_t dac3;
    ak4458_dac_config_t dac4;
} ak4458_config_t;
/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Default initializes AK4458.
 *
 * @param config AK4458 configure structure.
 */
void AK4458_DefaultConfig(ak4458_config_t *config);
/*!
 * @brief Initializes AK4458.
 *
 * @param handle AK4458 handle structure.
 * @param config AK4458 configure structure.
 */
status_t AK4458_Init(codec_handle_t *handle, void *config);
/*!
 * @brief Set the codec PCM mode or DSD mode based on the format info
 *
 * This function would configure the codec playback mode.
 *
 * @param handle AK4458 handle structure pointer.
 * @param format info.
 */
status_t AK4458_SetEncoding(codec_handle_t *handle, uint8_t format);
/*!
 * @brief Configure the data format of audio data.
 *
 * This function would configure the registers about the sample rate, bit depths.
 *
 * @param handle AK4458 handle structure pointer.
 * @param mclk system clock of the codec which can be generated by MCLK or PLL output.
 * @param sampleRate Sample rate of audio file running in AK4458.
 * @param bitWidth Bit depth of audio file.
 */
status_t AK4458_ConfigDataFormat(codec_handle_t *handle, uint32_t mclk, uint32_t sampleRate, uint32_t bitWidth);

/*!
 * @brief Set the volume of different modules in AK4458.
 *
 * This function would set the volume of AK4458 modules. Users need to appoint the module.
 * The function assume that left channel and right channel has the same volume.
 *
 * @param handle AK4458 handle structure.
 * @param value Volume value need to be set.
 */
status_t AK4458_SetDAC1Volume(codec_handle_t *handle, uint8_t value);

/*!
 * @brief Get the volume of DAC1 in AK4458.
 *
 * This function gets the volume of DAC1 in AK4458. Users need to appoint the module.
 * The function assume that left channel and right channel has the same volume.
 *
 * @param handle AK4458 handle structure.
 * @param value DAC volume value
 * @return value value of the module.
 */
status_t AK4458_GetDAC1Volume(codec_handle_t *handle, uint8_t *value);

/*!
 * @brief AK4497 codec module control.
 *
 * @param handle AK4497 handle structure pointer.
 * @param cmd module control command, support cmd kCODEC_ModuleSwitchDigitalInterface.
 * @param data control data, support data kCODEC_ModuleDigitalInterfacePCM/kCODEC_ModuleDigitalInterfaceDSD.
 */
status_t AK4458_ModuleControl(codec_handle_t *handle, codec_module_ctrl_cmd_t cmd, uint32_t data);

/*!
 * @brief Deinit the AK4458 codec.
 *
 * This function close all modules in AK4458 to save power.
 *
 * @param handle AK4458 handle structure pointer.
 */
status_t AK4458_Deinit(codec_handle_t *handle);
/*!
 * @brief Write register to AK4458 using I2C.
 *
 * @param handle AK4458 handle structure.
 * @param reg The register address in AK4458.
 * @param val Value needs to write into the register.
 */
status_t AK4458_WriteReg(codec_handle_t *handle, uint8_t reg, uint8_t val);

/*!
 * @brief Read register from AK4458 using I2C.
 * @param handle AK4458 handle structure.
 * @param reg The register address in AK4458.
 * @param val Value written to.
 */
status_t AK4458_ReadReg(codec_handle_t *handle, uint8_t reg, uint8_t *val);

/*!
 * @brief Modify some bits in the register using I2C.
 * @param handle AK4458 handle structure.
 * @param reg The register address in AK4458.
 * @param mask The mask code for the bits want to write. The bit you want to write should be 0.
 * @param val Value needs to write into the register.
 */
status_t AK4458_ModifyReg(codec_handle_t *handle, uint8_t reg, uint8_t mask, uint8_t val);

#if defined(__cplusplus)
}
#endif

/*! @} */
#endif /* _FSL_AK4458_H_ */
