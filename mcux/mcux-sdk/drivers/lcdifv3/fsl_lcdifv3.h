/*
 * Copyright 2019-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_LCDIFV3_H_
#define _FSL_LCDIFV3_H_

#include "fsl_common.h"

#if defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
#include "fsl_memory.h"
#endif

/*!
 * @addtogroup lcdifv3
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief LCDIF v3 driver version */
#define FSL_LCDIFV3_DRIVER_VERSION (MAKE_VERSION(2, 2, 3))
/*@}*/

#if defined(FSL_FEATURE_LCDIFV3_LAYER_COUNT) && (!defined(LCDIFV3_LAYER_COUNT))
#define LCDIFV3_LAYER_COUNT FSL_FEATURE_LCDIFV3_LAYER_COUNT
#endif

#if defined(FSL_FEATURE_LCDIFV3_LAYER_CSC_COUNT) && (!defined(LCDIFV3_LAYER_CSC_COUNT))
#define LCDIFV3_LAYER_CSC_COUNT FSL_FEATURE_LCDIFV3_LAYER_CSC_COUNT
#endif

#if defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
#define LCDIFV3_ADDR_CPU_2_IP(addr) (MEMORY_ConvertMemoryMapAddress((uint32_t)(addr), kMEMORY_Local2DMA))
#else
#define LCDIFV3_ADDR_CPU_2_IP(addr) (addr)
#endif /* FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET */

/* LUT memory entery number. */
#define LCDIFV3_LUT_ENTRY_NUM 256U

/*!
 * @brief LCDIF v3 signal polarity flags
 */
enum _lcdifv3_polarity_flags
{
    kLCDIFV3_VsyncActiveLow            = LCDIF_CTRL_INV_VS_MASK,   /*!< VSYNC active low. */
    kLCDIFV3_VsyncActiveHigh           = 0U,                         /*!< VSYNC active high. */
    kLCDIFV3_HsyncActiveLow            = LCDIF_CTRL_INV_HS_MASK,   /*!< HSYNC active low. */
    kLCDIFV3_HsyncActiveHigh           = 0U,                         /*!< HSYNC active high. */
    kLCDIFV3_DataEnableActiveLow       = LCDIF_CTRL_INV_DE_MASK,   /*!< Data enable line active low. */
    kLCDIFV3_DataEnableActiveHigh      = 0U,                         /*!< Data enable line active high. */
    kLCDIFV3_DriveDataOnFallingClkEdge = LCDIF_CTRL_INV_PXCK_MASK, /*!< Output data on falling clock edge, capture
                                                                        data on rising clock edge. */
    kLCDIFV3_DriveDataOnRisingClkEdge = 0U,                          /*!< Output data on rising clock edge, capture data
                                                                          on falling clock edge. */
    kLCDIFV3_DataActiveLow  = LCDIF_CTRL_NEG_MASK,                 /*!< Data active high. */
    kLCDIFV3_DataActiveHigh = 0U,                                    /*!< Data active high. */
};

/*!
 * @brief The LCDIF v3 interrupts.
 */
enum _lcdifv3_interrupt
{
    kLCDIFV3_LayerFifoEmptyInterrupt  = (1U << 24U), /*!< FIFO empty. */
    kLCDIFV3_LayerDmaDoneInterrupt    = (1U << 16U),   /*!< DMA done. */
    kLCDIFV3_LayerDmaErrorInterrupt   = (1U << 8U),  /*!< DMA error. */
    kLCDIFV3_VerticalBlankingInterrupt = (1U << 2U), /*!< Start of vertical blanking period. */
    kLCDIFV3_OutputUnderrunInterrupt   = (1U << 1U), /*!< Output buffer underrun. */
    kLCDIFV3_VsyncEdgeInterrupt        = (1U << 0U), /*!< Interrupt at VSYNC edge. */
};

/*! @brief The LCDIF v3 output line order. */
typedef enum _lcdifv3_line_order
{
    kLCDIFV3_LineOrderRGB = 0, /*!< RGB */
    kLCDIFV3_LineOrderRBG,     /*!< RBG */
    kLCDIFV3_LineOrderGBR,     /*!< GBR */
    kLCDIFV3_LineOrderGRB,     /*!< GRB */
    kLCDIFV3_LineOrderBRG,     /*!< BRG */
    kLCDIFV3_LineOrderBGR,     /*!< BGR */
    kLCDIFV3_LineOrderRGB555,
    kLCDIFV3_LineOrderRGB565,
    kLCDIFV3_LineOrderYUV_1,   /*!YUYV at [16:0]. */
    kLCDIFV3_LineOrderYUV_2,   /*!UYVY at [16:0]. */
    kLCDIFV3_LineOrderYUV_3,   /*!YVYU at [16:0]. */
    kLCDIFV3_LineOrderYUV_4,   /*!YUYV at [16:0]. */
    kLCDIFV3_LineOrderYUV_5,   /*!YUYV at [23:8]. */
    kLCDIFV3_LineOrderYUV_6,   /*!UYVY at [23:8]. */
    kLCDIFV3_LineOrderYUV_7,   /*!YVYU at [23:8]. */
    kLCDIFV3_LineOrderYUV_8,   /*!YUYV at [23:8]. */
} lcdifv3_line_order_t;

/*!
 * @brief LCDIF v3 display configure structure.
 */
typedef struct _lcdifv3_display_config
{
    uint16_t panelWidth;    /*!< Display panel width, pixels per line. */
    uint16_t panelHeight;   /*!< Display panel height, how many lines per panel. */
    uint8_t hsw;            /*!< HSYNC pulse width. */
    uint8_t hfp;            /*!< Horizontal front porch. */
    uint8_t hbp;            /*!< Horizontal back porch. */
    uint8_t vsw;            /*!< VSYNC pulse width. */
    uint8_t vfp;            /*!< Vrtical front porch. */
    uint8_t vbp;            /*!< Vertical back porch. */
    uint32_t polarityFlags; /*!< OR'ed value of @ref _lcdifv3_polarity_flags, used to contol the signal polarity. */
    lcdifv3_line_order_t lineOrder; /*!< Line order. */
} lcdifv3_display_config_t;

/*! @brief LCDIF v3 color space conversion mode. */
typedef enum _lcdifv3_csc_mode
{
    kLCDIFV3_CscDisable = 0U, /*!< Disable the CSC. */
    kLCDIFV3_CscYUV2RGB,      /*!< YUV to RGB. */
    kLCDIFV3_CscYCbCr2RGB,    /*!< YCbCr to RGB. */
} lcdifv3_csc_mode_t;

/*! @brief LCDIF v3 pixel format. */
typedef enum _lcdifv3_pixel_format
{
    kLCDIFV3_PixelFormatRGB565    = LCDIF_CTRLDESCL_5_BPP(4U), /*!< RGB565, two pixels use 32 bits. */
    kLCDIFV3_PixelFormatARGB1555  = LCDIF_CTRLDESCL_5_BPP(5U), /*!< ARGB1555, two pixels use 32 bits. */
    kLCDIFV3_PixelFormatARGB4444  = LCDIF_CTRLDESCL_5_BPP(6U), /*!< ARGB4444, two pixels use 32 bits. */
    kLCDIFV3_PixelFormatYVYU      = LCDIF_CTRLDESCL_5_BPP(7U) |
                               LCDIF_CTRLDESCL_5_YUV_FORMAT(0U), /*!< YVYU */
    kLCDIFV3_PixelFormatYUYV = LCDIF_CTRLDESCL_5_BPP(7U) |
                               LCDIF_CTRLDESCL_5_YUV_FORMAT(1U), /*!< YUYV */
    kLCDIFV3_PixelFormatVYUY = LCDIF_CTRLDESCL_5_BPP(7U) |
                               LCDIF_CTRLDESCL_5_YUV_FORMAT(2U), /*!< VYUY */
    kLCDIFV3_PixelFormatUYVY = LCDIF_CTRLDESCL_5_BPP(7U) |
                               LCDIF_CTRLDESCL_5_YUV_FORMAT(3U), /*!< UYVY */
    kLCDIFV3_PixelFormatRGB888   = LCDIF_CTRLDESCL_5_BPP(8U),    /*!< RGB888 packed, one pixel uses 24 bits. */
    kLCDIFV3_PixelFormatARGB8888 = LCDIF_CTRLDESCL_5_BPP(9U),    /*!< ARGB8888 unpacked, one pixel uses 32 bits. */
    kLCDIFV3_PixelFormatABGR8888 = LCDIF_CTRLDESCL_5_BPP(10U),   /*!< ABGR8888 unpacked, one pixel uses 32 bits. */
} lcdifv3_pixel_format_t;

/*! @brief LCDIF v3 source buffer configuration. */
typedef struct _lcdifv3_buffer_config
{
    uint16_t strideBytes; /*!< Number of bytes between two vertically adjacent pixels, suggest 64-bit aligned. */
    lcdifv3_pixel_format_t pixelFormat; /*!< Source buffer pixel format. */
} lcdifv3_buffer_config_t;

/*******************************************************************************
 * APIs
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @name LCDIF v3 initialization and de-initialization
 * @{
 */

/*!
 * @brief Initializes the LCDIF v3.
 *
 * This function ungates the LCDIF v3 clock and release the peripheral reset.
 *
 * @param base LCDIF v3 peripheral base address.
 */
void LCDIFV3_Init(LCDIF_Type *base);

/*!
 * @brief Deinitializes the LCDIF peripheral.
 *
 * @param base LCDIF peripheral base address.
 */
void LCDIFV3_Deinit(LCDIF_Type *base);

/*!
 * @brief Reset the LCDIF v3.
 *
 * @param base LCDIF peripheral base address.
 */
void LCDIFV3_Reset(LCDIF_Type *base);

/* @} */

/*!
 * @name Display
 * @{
 */

/*!
 * @brief Gets the LCDIF display default configuration structure.
 *
 * This function sets the configuration structure to default values.
 * The default configuration is set to the following values.
 * @code
    config->panelWidth    = 0U;
    config->panelHeight   = 0U;
    config->hsw           = 3U;
    config->hfp           = 3U;
    config->hbp           = 3U;
    config->vsw           = 3U;
    config->vfp           = 3U;
    config->vbp           = 3U;
    config->polarityFlags = kLCDIFV3_VsyncActiveHigh | kLCDIFV3_HsyncActiveHigh | kLCDIFV3_DataEnableActiveHigh |
                            kLCDIFV3_DriveDataOnRisingClkEdge | kLCDIFV3_DataActiveHigh;
    config->lineOrder       = kLCDIFV3_LineOrderRGB;
    @endcode
 *
 * @param config Pointer to the LCDIF configuration structure.
 */
void LCDIFV3_DisplayGetDefaultConfig(lcdifv3_display_config_t *config);

/*!
 * @brief Set the LCDIF v3 display configurations.
 *
 * @param base LCDIF peripheral base address.
 * @param config Pointer to the LCDIF configuration structure.
 */
void LCDIFV3_SetDisplayConfig(LCDIF_Type *base, const lcdifv3_display_config_t *config);

/*!
 * @brief Enable or disable the display
 *
 * @param base LCDIF peripheral base address.
 * @param enable Enable or disable.
 */
static inline void LCDIFV3_EnableDisplay(LCDIF_Type *base, bool enable)
{
    if (enable)
    {
        base->DISP_PARA |= LCDIF_DISP_PARA_DISP_ON_MASK;
        base->CTRLDESCL_5[0] |= LCDIF_CTRLDESCL_5_EN_MASK;
    }
    else
    {
        base->CTRLDESCL_5[0] &= ~LCDIF_CTRLDESCL_5_EN_MASK;
        base->DISP_PARA &= ~LCDIF_DISP_PARA_DISP_ON_MASK;
    }
}

/* @} */

/*!
 * @name Interrupts
 * @{
 */

/*!
 * @brief Enables LCDIF interrupt requests.
 *
 * @param base LCDIF peripheral base address.
 * @param mask interrupt source, OR'ed value of _lcdifv3_interrupt.
 */
static inline void LCDIFV3_EnableInterrupts(LCDIF_Type *base, uint8_t domain, uint32_t mask)
{
    if(domain == 0)
    {
        base->INT_ENABLE_D0 |= mask;
    }
    else
    {
	base->INT_ENABLE_D1 |= mask;
    }
}

/*!
 * @brief Disables LCDIF interrupt requests.
 *
 * @param base LCDIF peripheral base address.
 * @param mask interrupt source, OR'ed value of _lcdifv3_interrupt.
 */
static inline void LCDIFV3_DisableInterrupts(LCDIF_Type *base, uint8_t domain, uint32_t mask)
{
    if(domain == 0)
    {
        base->INT_ENABLE_D0 &= ~mask;
    }
    else
    {
	base->INT_ENABLE_D1 &= ~mask;
    }
}

/*!
 * @brief Get LCDIF interrupt peding status.
 *
 * @param base LCDIF peripheral base address.
 * @return Interrupt pending status, OR'ed value of _lcdifv3_interrupt.
 */
static inline uint32_t LCDIFV3_GetInterruptStatus(LCDIF_Type *base, uint8_t domain)
{
    if(domain == 0)
    {
        return base->INT_STATUS_D0;
    }
    else
    {
	return base->INT_STATUS_D1;
    }
}

/*!
 * @brief Clear LCDIF interrupt peding status.
 *
 * @param base LCDIF peripheral base address.
 * @param domain CPU domain the interrupt signal routed to.
 * @param mask of the flags to clear, OR'ed value of _lcdifv3_interrupt.
 */
static inline void LCDIFV3_ClearInterruptStatus(LCDIF_Type *base, uint8_t domain, uint32_t mask)
{
    if(domain == 0)
    {
        base->INT_STATUS_D0 = mask;
    }
    else
    {
	base->INT_STATUS_D1 = mask;
    }
}

/* @} */

/*!
 * @name LUT
 * @{
 */

/*!
 * @brief Set the LUT data.
 *
 * This function sets the specific layer LUT data, if @p useShadowLoad is true,
 * call @ref LCDIFV3_TriggerLayerShadowLoad after this function, the
 * LUT will be loaded to the hardware during next vertical blanking period.
 * If @p useShadowLoad is false, the LUT data is loaded to hardware directly.
 *
 * @param base LCDIF v3 peripheral base address.
 * @param layerIndex Which layer to set.
 * @param lutData The LUT data to load.
 * @param count Count of @p lutData.
 * @param useShadowLoad Use shadow load.
 * @retval kStatus_Success Set success.
 * @retval kStatus_Fail Previous LUT data is not loaded to hardware yet.
 */
status_t LCDIFV3_SetLut(
    LCDIF_Type *base, uint8_t layerIndex, const uint32_t *lutData, uint16_t count, bool useShadowLoad);

/* @} */

/*!
 * @name Layer operation
 * @{
 */

/*!
 * @brief Set the layer dimension.
 *
 * @param base LCDIFv3 peripheral base address.
 * @param width Layer width in pixel.
 * @param height Layer height.
 *
 * @note The layer width must be in multiples of the number of pixels that can be stored in 32 bits
 */
static inline void LCDIFV3_SetLayerSize(LCDIF_Type *base, uint16_t width, uint16_t height)
{
    base->CTRLDESCL_1[0] =
        ((uint32_t)height << LCDIF_CTRLDESCL_1_HEIGHT_SHIFT) | ((uint32_t)width << LCDIF_CTRLDESCL_1_WIDTH_SHIFT);
}

/*!
 * @brief Set the layer source buffer configuration.
 *
 * @param base LCDIFv3 peripheral base address.
 * @param config Pointer to the configuration.
 */
void LCDIFV3_SetLayerBufferConfig(LCDIF_Type *base, const lcdifv3_buffer_config_t *config);
void LCDIFV3_SetStrideBytes(LCDIF_Type *base, uint16_t strideBytes);
void LCDIFV3_EnablePlanePanic(LCDIF_Type *base);

/*!
 * @brief Set the layer source buffer address.
 *
 * This function is used for fast runtime source buffer change.
 *
 * @param base LCDIFv3 peripheral base address.
 * @param addr The new source buffer address passed to the layer, should be 64-bit aligned.
 */
static inline void LCDIFV3_SetLayerBufferAddr(LCDIF_Type *base, uint64_t addr)
{
    base->CTRLDESCL_LOW_4[0] = addr & 0xFFFFFFFF;
    base->CTRLDESCL_HIGH_4[0] = addr >> 32;
}

/*!
 * @brief Enable or disable the layer.
 *
 * @param base LCDIFv3 peripheral base address.
 * @param layerIndex Layer layerIndex.
 * @param enable Pass in true to enable, false to disable.
 */
static inline void LCDIFV3_EnableLayer(LCDIF_Type *base, bool enable)
{
    if (enable)
    {
        base->CTRLDESCL_5[0] |= LCDIF_CTRLDESCL_5_EN_MASK;
    }
    else
    {
        base->CTRLDESCL_5[0] &= ~LCDIF_CTRLDESCL_5_EN_MASK;
    }
}

/*!
 * @brief Trigger the layer configuration shadow load.
 *
 * The new layer configurations are written to the shadow registers first,
 * When all configurations written finished, call this function, then shadowed
 * control registers are updated to the active control registers on VSYNC of
 * next frame.
 *
 * @param base LCDIFv3 peripheral base address.
 */
static inline void LCDIFV3_TriggerLayerShadowLoad(LCDIF_Type *base)
{
    base->CTRLDESCL_5[0] |= LCDIF_CTRLDESCL_5_SHADOW_LOAD_EN_MASK;
}

/*!
 * @brief Set the color space conversion mode.
 *
 * Supports YUV2RGB and YCbCr2RGB.
 *
 * @param base LCDIFv3 peripheral base address.
 * @param mode The conversion mode.
 */
void LCDIFV3_SetCscMode(LCDIF_Type *base, lcdifv3_csc_mode_t mode);

/* @} */

/*!
 * @name Porter Duff
 * @{
 */

/* @} */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/* @} */

#endif /*_FSL_LCDIFV3_H_*/
