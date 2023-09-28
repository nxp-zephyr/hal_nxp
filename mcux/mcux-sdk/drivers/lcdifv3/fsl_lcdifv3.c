/*
 * Copyright 2019-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_lcdifv3.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.lcdifv3"
#endif

#define LCDIFV3_LUT_MEM(base) \
    ((volatile uint32_t *)(((uint32_t)(base)) + (uint32_t)FSL_FEATURE_LCDIFV3_CLUT_RAM_OFFSET))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get instance number for LCDIF module.
 *
 * @param base LCDIF peripheral base address
 */
static uint32_t LCDIFV3_GetInstance(LCDIF_Type *base);

/*!
 * @brief Reset register value to default status.
 *
 * @param base LCDIF peripheral base address
 */
static void LCDIFV3_ResetRegister(LCDIF_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Pointers to LCDIF bases for each instance. */
static LCDIF_Type *const s_lcdifv3Bases[] = LCDIF_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to LCDIF clock for each instance. */
static const clock_ip_name_t s_lcdifv3Clocks[] = LCDIFV3_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Codes
 ******************************************************************************/
static uint32_t LCDIFV3_GetInstance(LCDIF_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_lcdifv3Bases); instance++)
    {
        if (s_lcdifv3Bases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_lcdifv3Bases));

    return instance;
}

static void LCDIFV3_ResetRegister(LCDIF_Type *base)
{

    base->DISP_PARA         = 0U;
    base->CTRL.RW            = 0x80000000U;
    base->DISP_SIZE         = 0U;
    base->HSYN_PARA         = 0x00C01803U;
    base->VSYN_PARA         = 0x00C01803U;
    base->INT_ENABLE_D0 = 0U;
    base->INT_ENABLE_D1 = 0U;

    base->CTRLDESCL_5[0] = 0U;
    base->CTRLDESCL_1[0] = 0U;
    base->CTRLDESCL_3[0] = 0U;

    base->CSC_CTRL[0] = 0x1U;
    base->CSC_COEF0[0] = 0;
    base->CSC_COEF1[0] = 0;
    base->CSC_COEF2[0] = 0;
    base->CSC_COEF3[0] = 0;
    base->CSC_COEF4[0] = 0;
    base->CSC_COEF5[0] = 0;

    /* Clear interrupt status. */
    base->INT_STATUS_D0= 0xFFFFFFFFU;
    base->INT_STATUS_D1 = 0xFFFFFFFFU;
}

/*!
 * brief Initializes the LCDIF v3.
 *
 * This function ungates the LCDIF v3 clock and release the peripheral reset.
 *
 * param base LCDIF v3 peripheral base address.
 */
void LCDIFV3_Init(LCDIF_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    uint32_t instance = LCDIFV3_GetInstance(base);
    /* Enable the clock. */
    CLOCK_EnableClock(s_lcdifv3Clocks[instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    LCDIFV3_ResetRegister(base);

    /* Out of reset. */
    base->CTRL.RW = 0U;
}

/*!
 * brief Deinitializes the LCDIF peripheral.
 *
 * param base LCDIF peripheral base address.
 */
void LCDIFV3_Deinit(LCDIF_Type *base)
{
    LCDIFV3_ResetRegister(base);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    uint32_t instance = LCDIFV3_GetInstance(base);
    /* Disable the clock. */
    CLOCK_DisableClock(s_lcdifv3Clocks[instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/*!
 * brief Reset the LCDIF v3.
 *
 * param base LCDIF peripheral base address.
 */
void LCDIFV3_Reset(LCDIF_Type *base)
{
    base->CTRL.RW = 0x80000000;
    base->CTRL.RW = 0U;
}

/*!
 * brief Gets the LCDIF display default configuration structure.
 *
 * param config Pointer to the LCDIF configuration structure.
 */
void LCDIFV3_DisplayGetDefaultConfig(lcdifv3_display_config_t *config)
{
    assert(NULL != config);

    config->panelWidth    = 0U;
    config->panelHeight   = 0U;
    config->hsw           = 3U;
    config->hfp           = 3U;
    config->hbp           = 3U;
    config->vsw           = 3U;
    config->vfp           = 3U;
    config->vbp           = 3U;
    config->polarityFlags = (uint32_t)kLCDIFV3_VsyncActiveHigh | (uint32_t)kLCDIFV3_HsyncActiveHigh |
                            (uint32_t)kLCDIFV3_DataEnableActiveHigh | (uint32_t)kLCDIFV3_DriveDataOnRisingClkEdge |
                            (uint32_t)kLCDIFV3_DataActiveHigh;
    config->lineOrder = kLCDIFV3_LineOrderRGB;
}

/*!
 * brief Set the LCDIF v3 display configurations.
 *
 * param base LCDIF peripheral base address.
 * param config Pointer to the LCDIF configuration structure.
 */
void LCDIFV3_SetDisplayConfig(LCDIF_Type *base, const lcdifv3_display_config_t *config)
{
    assert(NULL != config);

    /* Configure the parameters. */
    base->DISP_SIZE = ((uint32_t)config->panelWidth << LCDIF_DISP_SIZE_DELTA_X_SHIFT) |
                      ((uint32_t)config->panelHeight << LCDIF_DISP_SIZE_DELTA_Y_SHIFT);

    base->HSYN_PARA = ((uint32_t)config->hbp << LCDIF_HSYN_PARA_BP_H_SHIFT) |
                      ((uint32_t)config->hfp << LCDIF_HSYN_PARA_FP_H_SHIFT);

    base->VSYN_PARA = ((uint32_t)config->vbp << LCDIF_VSYN_PARA_BP_V_SHIFT) |
                      ((uint32_t)config->vfp << LCDIF_VSYN_PARA_FP_V_SHIFT);
 
    base->VSYN_HSYN_WIDTH = ((uint32_t)config->hsw << LCDIF_VSYN_HSYN_WIDTH_PW_H_SHIFT) |
                            ((uint32_t)config->vsw << LCDIF_VSYN_HSYN_WIDTH_PW_V_SHIFT);

    base->DISP_PARA = LCDIF_DISP_PARA_LINE_PATTERN(config->lineOrder);

    base->CTRL.RW = (uint32_t)(config->polarityFlags);
}

/*!
 * brief Set the color space conversion mode.
 *
 * Supports YUV2RGB and YCbCr2RGB.
 *
 * param base LCDIFv3 peripheral base address.
 * param mode The conversion mode.
 */
void LCDIFV3_SetCscMode(LCDIF_Type *base,lcdifv3_csc_mode_t mode)
{
    /* TO DO, below codes are for RT1170.*/
    assert(0);
#if 0
    /*
     * The equations used for Colorspace conversion are:
     *
     * R = C0*(Y+Y_OFFSET)                   + C1(V+UV_OFFSET)
     * G = C0*(Y+Y_OFFSET) + C3(U+UV_OFFSET) + C2(V+UV_OFFSET)
     * B = C0*(Y+Y_OFFSET) + C4(U+UV_OFFSET)
     */

    if (kLCDIFV3_CscYUV2RGB == mode)
    {
        base->LAYER[layerIndex].CSC_COEF0 = LCDIFV3_CSC_COEF0_ENABLE_MASK | LCDIFV3_CSC_COEF0_C0(0x100U) /* 1.00. */
                                            | LCDIFV3_CSC_COEF0_Y_OFFSET(0x0U)                           /* 0. */
                                            | LCDIFV3_CSC_COEF0_UV_OFFSET(0x0U);                         /* 0. */

        base->LAYER[layerIndex].CSC_COEF1 = LCDIFV3_CSC_COEF1_C1(0x0123U)    /* 1.140. */
                                            | LCDIFV3_CSC_COEF1_C4(0x0208U); /* 2.032. */
        base->LAYER[layerIndex].CSC_COEF2 = LCDIFV3_CSC_COEF2_C2(0x076BU)    /* -0.851. */
                                            | LCDIFV3_CSC_COEF2_C3(0x079BU); /* -0.394. */
    }
    else if (kLCDIFV3_CscYCbCr2RGB == mode)
    {
        base->LAYER[layerIndex].CSC_COEF0 = LCDIFV3_CSC_COEF0_ENABLE_MASK | LCDIFV3_CSC_COEF0_YCBCR_MODE_MASK |
                                            LCDIFV3_CSC_COEF0_C0(0x12AU)           /* 1.164. */
                                            | LCDIFV3_CSC_COEF0_Y_OFFSET(0x1F0U)   /* -16. */
                                            | LCDIFV3_CSC_COEF0_UV_OFFSET(0x180U); /* -128. */
        base->LAYER[layerIndex].CSC_COEF1 = LCDIFV3_CSC_COEF1_C1(0x0198U)          /* 1.596. */
                                            | LCDIFV3_CSC_COEF1_C4(0x0204U);       /* 2.017. */
        base->LAYER[layerIndex].CSC_COEF2 = LCDIFV3_CSC_COEF2_C2(0x0730U)          /* -0.813. */
                                            | LCDIFV3_CSC_COEF2_C3(0x079CU);       /* -0.392. */
    }
    else
    {
        base->LAYER[layerIndex].CSC_COEF0 = 0U;
        base->LAYER[layerIndex].CSC_COEF1 = 0U;
        base->LAYER[layerIndex].CSC_COEF2 = 0U;
    }
#endif
}

/*!
 * brief Set the layer source buffer configuration.
 *
 * param base LCDIFv3 peripheral base address.
 * param config Pointer to the configuration.
 */
void LCDIFV3_SetLayerBufferConfig(LCDIF_Type *base, const lcdifv3_buffer_config_t *config)
{
    assert(NULL != config);
    uint32_t reg;

    base->CTRLDESCL_3[0] = config->strideBytes | (0xA2 << 16);

    reg = base->CTRLDESCL_5[0];
    reg = (reg & ~(LCDIF_CTRLDESCL_5_BPP_MASK | LCDIF_CTRLDESCL_5_YUV_FORMAT_MASK)) | (uint32_t)config->pixelFormat;

    base->CTRLDESCL_5[0] = reg;
}

void LCDIFV3_SetStrideBytes(LCDIF_Type *base, uint16_t strideBytes)
{
    base->CTRLDESCL_3[0] = strideBytes;
}

void LCDIFV3_EnablePlanePanic(LCDIF_Type *base)
{
    uint32_t panic_thres, thres_low, thres_high;

    /* apb clock has been enabled */

    /* As suggestion, the thres_low should be 1/3 FIFO,
     * and thres_high should be 2/3 FIFO (The FIFO size
     * is 8KB = 512 * 128bit).
     * threshold = n * 128bit (n: 0 ~ 511)
     */
    thres_low  = 511 * 1 / 3;

    thres_high = 511 * 2 / 3;

    panic_thres = thres_low << LCDIF_PANIC_THRES_PANIC_THRES_LOW_SHIFT |
                  thres_high << LCDIF_PANIC_THRES_PANIC_THRES_HIGH_SHIFT;

    base->PANIC_THRES[0] = panic_thres;

    /* Enable Panic:
     *
     * As designed, the panic won't trigger an irq,
     * so it is unnecessary to handle this as an irq
     * and NoC + QoS modules will handle panic
     * automatically.
     */
    base->INT_ENABLE_D1 = LCDIF_INT_STATUS_D1_PLANE_PANIC_MASK;

}
