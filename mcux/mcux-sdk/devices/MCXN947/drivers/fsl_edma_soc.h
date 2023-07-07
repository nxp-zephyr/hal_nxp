/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_EDMA_SOC_H_
#define _FSL_EDMA_SOC_H_

#include "fsl_common.h"

/*!
 * @addtogroup edma_soc
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @name Driver version */
/*@{*/
/*! @brief Driver version 1.0.0. */
#define FSL_EDMA_SOC_DRIVER_VERSION (MAKE_VERSION(1, 0, 0))
/*@}*/

/*!@brief DMA IP version */
#define FSL_EDMA_SOC_IP_DMA3 (1)
#define FSL_EDMA_SOC_IP_DMA4 (0)

/*!@brief DMA base table */
#define EDMA_BASE_PTRS \
    {                  \
        DMA0, DMA1     \
    }

#define EDMA_CHN_IRQS                                                                                               \
    {                                                                                                               \
        {EDMA_0_CH0_IRQn,  EDMA_0_CH1_IRQn,  EDMA_0_CH2_IRQn,  EDMA_0_CH3_IRQn, EDMA_0_CH4_IRQn,  EDMA_0_CH5_IRQn,  \
         EDMA_0_CH6_IRQn,  EDMA_0_CH7_IRQn,  EDMA_0_CH8_IRQn,  EDMA_0_CH9_IRQn, EDMA_0_CH10_IRQn, EDMA_0_CH11_IRQn, \
         EDMA_0_CH12_IRQn, EDMA_0_CH13_IRQn, EDMA_0_CH14_IRQn, EDMA_0_CH15_IRQn},                                   \
        {                                                                                                           \
            EDMA_1_CH0_IRQn, EDMA_1_CH1_IRQn, EDMA_1_CH2_IRQn, EDMA_1_CH3_IRQn, EDMA_1_CH4_IRQn, EDMA_1_CH5_IRQn,   \
                EDMA_1_CH6_IRQn, EDMA_1_CH7_IRQn, EDMA_1_CH8_IRQn, EDMA_1_CH9_IRQn, EDMA_1_CH10_IRQn,               \
                EDMA_1_CH11_IRQn, EDMA_1_CH12_IRQn, EDMA_1_CH13_IRQn, EDMA_1_CH14_IRQn, EDMA_1_CH15_IRQn            \
        }                                                                                                           \
    }

/*!@brief dma request source */
typedef enum _dma_request_source
{
    kDmaRequestDisabled                        = 0U,   /**< Disabled*/
    kDmaRequestMuxFlexSpi0Rx                   = 1U,   /**< FlexSPI0 Receive event */
    kDmaRequestMuxFlexSpi0Tx                   = 2U,   /**< FlexSPI0 Transmit event */
    kDmaRequestMuxPinInt0                      = 3U,   /**< PinInt0 */
    kDmaRequestMuxPinInt1                      = 4U,   /**< PinInt1 */
    kDmaRequestMuxPinInt2                      = 5U,   /**< PinInt2 */
    kDmaRequestMuxPinInt3                      = 6U,   /**< PinInt3 */
    kDmaRequestMuxCtimer0M0                    = 7U,   /**< Ctimer0_M0 */
    kDmaRequestMuxCtimer0M1                    = 8U,   /**< Ctimer0_M1 */
    kDmaRequestMuxCtimer1M0                    = 9U,   /**< Ctimer1_M0 */
    kDmaRequestMuxCtimer1M1                    = 10U,  /**< Ctimer1_M1 */
    kDmaRequestMuxCtimer2M0                    = 11U,  /**< Ctimer2_M0 */
    kDmaRequestMuxCtimer2M1                    = 12U,  /**< Ctimer2_M1 */
    kDmaRequestMuxCtimer3M0                    = 13U,  /**< Ctimer3_M0 */
    kDmaRequestMuxCtimer3M1                    = 14U,  /**< Ctimer3_M1 */
    kDmaRequestMuxCtimer4M0                    = 15U,  /**< Ctimer4_M0 */
    kDmaRequestMuxCtimer5M1                    = 16U,  /**< Ctimer4_M1 */
    kDmaRequestMuxWuu0                         = 17U,  /**< Wake up event */
    kDmaRequestMuxMicfil0FifoRequest           = 18U,  /**< MICFIL0 FIFO_request */
    kDmaRequestMuxSct0Dma0                     = 19U,  /**< SCT0 DMA0 */
    kDmaRequestMuxSct0Dma1                     = 20U,  /**< SCT0 DMA1 */
    kDmaRequestMuxAdc0FifoARequest             = 21U,  /**< ADC0 FIFO A request */
    kDmaRequestMuxAdc0FifoBRequest             = 22U,  /**< ADC0 FIFO B request */
    kDmaRequestMuxAdc1FifoARequest             = 23U,  /**< ADC1 FIFO A request */
    kDmaRequestMuxAdc1FifoBRequest             = 24U,  /**< ADC1 FIFO B request */
    kDmaRequestMuxDac0FifoRequest              = 25U,  /**< DAC0 FIFO_request */
    kDmaRequestMuxDac1FifoRequest              = 26U,  /**< DAC1 FIFO_request */
    kDmaRequestMuxDac2FifoRequest              = 27U,  /**< HP DAC0 FIFO_request */
    kDmaRequestMuxHsCmp0DmaRequest             = 28U,  /**< HS CMP0 DMA_request */
    kDmaRequestMuxHsCmp1DmaRequest             = 29U,  /**< HS CMP0 DMA_request */
    kDmaRequestMuxHsCmp2DmaRequest             = 30U,  /**< HS CMP0 DMA_request */
    kDmaRequestMuxEvtg0Out0A                   = 31U,  /**< EVTG0 OUT0A */
    kDmaRequestMuxEvtg0Out0B                   = 32U,  /**< EVTG0 OUT0B */
    kDmaRequestMuxEvtg0Out1A                   = 33U,  /**< EVTG0 OUT1A */
    kDmaRequestMuxEvtg0Out1B                   = 34U,  /**< EVTG0 OUT1B */
    kDmaRequestMuxEvtg0Out2A                   = 35U,  /**< EVTG0 OUT2A */
    kDmaRequestMuxEvtg0Out2B                   = 36U,  /**< EVTG0 OUT2B */
    kDmaRequestMuxEvtg0Out3A                   = 37U,  /**< EVTG0 OUT3A */
    kDmaRequestMuxEvtg0Out3B                   = 38U,  /**< EVTG0 OUT3B */
    kDmaRequestMuxFlexPwm0ReqCapt0             = 39U,  /**< FlexPWM0 Req_capt0 */
    kDmaRequestMuxFlexPwm0ReqCapt1             = 40U,  /**< FlexPWM0 Req_capt1 */
    kDmaRequestMuxFlexPwm0ReqCapt2             = 41U,  /**< FlexPWM0 Req_capt2 */
    kDmaRequestMuxFlexPwm0ReqCapt3             = 42U,  /**< FlexPWM0 Req_capt3 */
    kDmaRequestMuxFlexPwm0ReqVal0              = 43U,  /**< FlexPWM0 Req_val0 */
    kDmaRequestMuxFlexPwm0ReqVal1              = 44U,  /**< FlexPWM0 Req_val1 */
    kDmaRequestMuxFlexPwm0ReqVal2              = 45U,  /**< FlexPWM0 Req_val2 */
    kDmaRequestMuxFlexPwm0ReqVal3              = 46U,  /**< FlexPWM0 Req_val3 */
    kDmaRequestMuxFlexPwm1ReqCapt0             = 47U,  /**< FlexPWM1 Req_capt0 */
    kDmaRequestMuxFlexPwm1ReqCapt1             = 48U,  /**< FlexPWM1 Req_capt1 */
    kDmaRequestMuxFlexPwm1ReqCapt2             = 49U,  /**< FlexPWM1 Req_capt2 */
    kDmaRequestMuxFlexPwm1ReqCapt3             = 50U,  /**< FlexPWM1 Req_capt3 */
    kDmaRequestMuxFlexPwm1ReqVal0              = 51U,  /**< FlexPWM1 Req_val0 */
    kDmaRequestMuxFlexPwm1ReqVal1              = 52U,  /**< FlexPWM1 Req_val1 */
    kDmaRequestMuxFlexPwm1ReqVal2              = 53U,  /**< FlexPWM1 Req_val2 */
    kDmaRequestMuxFlexPwm1ReqVal3              = 54U,  /**< FlexPWM1 Req_val3 */
    kDmaRequestMuxItrc0TmprOut0                = 55U,  /**< ITRC0 TMPR_OUT0 */
    kDmaRequestMuxItrc0TmprOut1                = 56U,  /**< ITRC0 TMPR_OUT1 */
    kDmaRequestMuxLptmr0                       = 57U,  /**< LPTMR0 Counter match event */
    kDmaRequestMuxLptmr1                       = 58U,  /**< LPTMR1 Counter match event */
    kDmaRequestMuxFlexCan0DmaRequest           = 59U,  /**< FlexCAN0 DMA request */
    kDmaRequestMuxFlexCan1DmaRequest           = 60U,  /**< FlexCAN1 DMA request */
    kDmaRequestMuxFlexIO0ShiftRegister0Request = 61U,  /**< FlexIO0 Shift Register 0 request */
    kDmaRequestMuxFlexIO0ShiftRegister1Request = 62U,  /**< FlexIO0 Shift Register 1 request */
    kDmaRequestMuxFlexIO0ShiftRegister2Request = 63U,  /**< FlexIO0 Shift Register 2 request */
    kDmaRequestMuxFlexIO0ShiftRegister3Request = 64U,  /**< FlexIO0 Shift Register 3 request */
    kDmaRequestMuxFlexIO0ShiftRegister4Request = 65U,  /**< FlexIO0 Shift Register 4 request */
    kDmaRequestMuxFlexIO0ShiftRegister5Request = 66U,  /**< FlexIO0 Shift Register 5 request */
    kDmaRequestMuxFlexIO0ShiftRegister6Request = 67U,  /**< FlexIO0 Shift Register 6 request */
    kDmaRequestMuxFlexIO0ShiftRegister7Request = 68U,  /**< FlexIO0 Shift Register 7 request */
    kDmaRequestMuxLpFlexcomm0Rx                = 69U,  /**< LP_FLEXCOMM0 Receive request */
    kDmaRequestMuxLpFlexcomm0Tx                = 70U,  /**< LP_FLEXCOMM0 Transmit request */
    kDmaRequestMuxLpFlexcomm1Rx                = 71U,  /**< LP_FLEXCOMM1 Receive request */
    kDmaRequestMuxLpFlexcomm1Tx                = 72U,  /**< LP_FLEXCOMM1 Transmit request */
    kDmaRequestMuxLpFlexcomm2Rx                = 73U,  /**< LP_FLEXCOMM2 Receive request */
    kDmaRequestMuxLpFlexcomm2Tx                = 74U,  /**< LP_FLEXCOMM2 Transmit request */
    kDmaRequestMuxLpFlexcomm3Rx                = 75U,  /**< LP_FLEXCOMM3 Receive request */
    kDmaRequestMuxLpFlexcomm3Tx                = 76U,  /**< LP_FLEXCOMM3 Transmit request */
    kDmaRequestMuxLpFlexcomm4Rx                = 77U,  /**< LP_FLEXCOMM4 Receive request */
    kDmaRequestMuxLpFlexcomm4Tx                = 78U,  /**< LP_FLEXCOMM4 Transmit request */
    kDmaRequestMuxLpFlexcomm5Rx                = 79U,  /**< LP_FLEXCOMM5 Receive request */
    kDmaRequestMuxLpFlexcomm5Tx                = 80U,  /**< LP_FLEXCOMM5 Transmit request */
    kDmaRequestMuxLpFlexcomm6Rx                = 81U,  /**< LP_FLEXCOMM6 Receive request */
    kDmaRequestMuxLpFlexcomm6Tx                = 82U,  /**< LP_FLEXCOMM6 Transmit request */
    kDmaRequestMuxLpFlexcomm7Rx                = 83U,  /**< LP_FLEXCOMM7 Receive request */
    kDmaRequestMuxLpFlexcomm7Tx                = 84U,  /**< LP_FLEXCOMM7 Transmit request */
    kDmaRequestMuxLpFlexcomm8Rx                = 85U,  /**< LP_FLEXCOMM8 Receive request */
    kDmaRequestMuxLpFlexcomm8Tx                = 86U,  /**< LP_FLEXCOMM8 Transmit request */
    kDmaRequestMuxLpFlexcomm9Rx                = 87U,  /**< LP_FLEXCOMM9 Receive request */
    kDmaRequestMuxLpFlexcomm9Tx                = 88U,  /**< LP_FLEXCOMM9 Transmit request */
    kDmaRequestMuxESpi0Ch0                     = 89U,  /**< eSPI0 channel 0 */
    kDmaRequestMuxESpi0Ch1                     = 90U,  /**< eSPI0 channel 1 */
    kDmaRequestMuxEmvSim0Rx                    = 91U,  /**< EMVSIM0 Receive request */
    kDmaRequestMuxEmvSim0Tx                    = 92U,  /**< EMVSIM0 Transmit request */
    kDmaRequestMuxEmvSim1Rx                    = 93U,  /**< EMVSIM1 Receive request */
    kDmaRequestMuxEmvSim1Tx                    = 94U,  /**< EMVSIM1 Transmit request */
    kDmaRequestMuxI3c0Rx                       = 95U,  /**< I3C0 Receive request */
    kDmaRequestMuxI3c0Tx                       = 96U,  /**< I3C0 Transmit request */
    kDmaRequestMuxI3c1Rx                       = 97U,  /**< I3C0 Receive request */
    kDmaRequestMuxI3c1Tx                       = 98U,  /**< I3C0 Transmit request */
    kDmaRequestMuxSai0Rx                       = 99U,  /**< SAI0 Receive request */
    kDmaRequestMuxSai0Tx                       = 100U, /**< SAI0 Receive request */
    kDmaRequestMuxSai1Rx                       = 101U, /**< SAI1 Receive request */
    kDmaRequestMuxSai1Tx                       = 102U, /**< SAI1 Receive request */
    kDmaRequestMuxSinc0IpdReqSincAlt0          = 103U, /**< SINC0 ipd_req_sinc[0] or ipd_req_alt [0] */
    kDmaRequestMuxSinc0IpdReqSincAlt1          = 104U, /**< SINC0 ipd_req_sinc[1] or ipd_req_alt [1] */
    kDmaRequestMuxSinc0IpdReqSincAlt2          = 105U, /**< SINC0 ipd_req_sinc[2] or ipd_req_alt [2] */
    kDmaRequestMuxSinc0IpdReqSincAlt3          = 106U, /**< SINC0 ipd_req_sinc[3] or ipd_req_alt [3] */
    kDmaRequestMuxSinc0IpdReqSincAlt4          = 107U, /**< SINC0 ipd_req_sinc[4] or ipd_req_alt [4] */
    kDmaRequestMuxGpio0PinEventRequest0        = 108U, /**< GPIO0 Pin event request 0 */
    kDmaRequestMuxGpio0PinEventRequest1        = 109U, /**< GPIO0 Pin event request 1 */
    kDmaRequestMuxGpio1PinEventRequest0        = 110U, /**< GPIO1 Pin event request 0 */
    kDmaRequestMuxGpio1PinEventRequest1        = 111U, /**< GPIO1 Pin event request 1 */
    kDmaRequestMuxGpio2PinEventRequest0        = 112U, /**< GPIO2 Pin event request 0 */
    kDmaRequestMuxGpio2PinEventRequest1        = 113U, /**< GPIO2 Pin event request 1 */
    kDmaRequestMuxGpio3PinEventRequest0        = 114U, /**< GPIO3 Pin event request 0 */
    kDmaRequestMuxGpio3PinEventRequest1        = 115U, /**< GPIO3 Pin event request 1 */
    kDmaRequestMuxGpio4PinEventRequest0        = 116U, /**< GPIO4 Pin event request 0 */
    kDmaRequestMuxGpio4PinEventRequest1        = 117U, /**< GPIO4 Pin event request 1 */
    kDmaRequestMuxGpio5PinEventRequest0        = 118U, /**< GPIO5 Pin event request 0 */
    kDmaRequestMuxGpio5PinEventRequest1        = 119U, /**< GPIO5 Pin event request 1 */
    kDmaRequestMuxTsi0EndOfScan                = 120U, /**< TSI0 End of Scan */
    kDmaRequestMuxTsi0OutOfRange               = 121U, /**< TSI0 Out of Range */

    kDma0RequestDisabled                        = 0U,   /**< Disabled*/
    kDma0RequestMuxFlexSpi0Rx                   = 1U,   /**< FlexSPI0 Receive event */
    kDma0RequestMuxFlexSpi0Tx                   = 2U,   /**< FlexSPI0 Transmit event */
    kDma0RequestMuxPinInt0                      = 3U,   /**< PinInt0 */
    kDma0RequestMuxPinInt1                      = 4U,   /**< PinInt1 */
    kDma0RequestMuxPinInt2                      = 5U,   /**< PinInt2 */
    kDma0RequestMuxPinInt3                      = 6U,   /**< PinInt3 */
    kDma0RequestMuxCtimer0M0                    = 7U,   /**< Ctimer0_M0 */
    kDma0RequestMuxCtimer0M1                    = 8U,   /**< Ctimer0_M1 */
    kDma0RequestMuxCtimer1M0                    = 9U,   /**< Ctimer1_M0 */
    kDma0RequestMuxCtimer1M1                    = 10U,  /**< Ctimer1_M1 */
    kDma0RequestMuxCtimer2M0                    = 11U,  /**< Ctimer2_M0 */
    kDma0RequestMuxCtimer2M1                    = 12U,  /**< Ctimer2_M1 */
    kDma0RequestMuxCtimer3M0                    = 13U,  /**< Ctimer3_M0 */
    kDma0RequestMuxCtimer3M1                    = 14U,  /**< Ctimer3_M1 */
    kDma0RequestMuxCtimer4M0                    = 15U,  /**< Ctimer4_M0 */
    kDma0RequestMuxCtimer5M1                    = 16U,  /**< Ctimer4_M1 */
    kDma0RequestMuxWuu0                         = 17U,  /**< Wake up event */
    kDma0RequestMuxMicfil0FifoRequest           = 18U,  /**< MICFIL0 FIFO_request */
    kDma0RequestMuxSct0Dma0                     = 19U,  /**< SCT0 DMA0 */
    kDma0RequestMuxSct0Dma1                     = 20U,  /**< SCT0 DMA1 */
    kDma0RequestMuxAdc0FifoARequest             = 21U,  /**< ADC0 FIFO A request */
    kDma0RequestMuxAdc0FifoBRequest             = 22U,  /**< ADC0 FIFO B request */
    kDma0RequestMuxAdc1FifoARequest             = 23U,  /**< ADC1 FIFO A request */
    kDma0RequestMuxAdc1FifoBRequest             = 24U,  /**< ADC1 FIFO B request */
    kDma0RequestMuxDac0FifoRequest              = 25U,  /**< DAC0 FIFO_request */
    kDma0RequestMuxDac1FifoRequest              = 26U,  /**< DAC1 FIFO_request */
    kDma0RequestMuxDac2FifoRequest              = 27U,  /**< HP DAC0 FIFO_request */
    kDma0RequestMuxHsCmp0DmaRequest             = 28U,  /**< HS CMP0 DMA_request */
    kDma0RequestMuxHsCmp1DmaRequest             = 29U,  /**< HS CMP0 DMA_request */
    kDma0RequestMuxHsCmp2DmaRequest             = 30U,  /**< HS CMP0 DMA_request */
    kDma0RequestMuxEvtg0Out0A                   = 31U,  /**< EVTG0 OUT0A */
    kDma0RequestMuxEvtg0Out0B                   = 32U,  /**< EVTG0 OUT0B */
    kDma0RequestMuxEvtg0Out1A                   = 33U,  /**< EVTG0 OUT1A */
    kDma0RequestMuxEvtg0Out1B                   = 34U,  /**< EVTG0 OUT1B */
    kDma0RequestMuxEvtg0Out2A                   = 35U,  /**< EVTG0 OUT2A */
    kDma0RequestMuxEvtg0Out2B                   = 36U,  /**< EVTG0 OUT2B */
    kDma0RequestMuxEvtg0Out3A                   = 37U,  /**< EVTG0 OUT3A */
    kDma0RequestMuxEvtg0Out3B                   = 38U,  /**< EVTG0 OUT3B */
    kDma0RequestMuxFlexPwm0ReqCapt0             = 39U,  /**< FlexPWM0 Req_capt0 */
    kDma0RequestMuxFlexPwm0ReqCapt1             = 40U,  /**< FlexPWM0 Req_capt1 */
    kDma0RequestMuxFlexPwm0ReqCapt2             = 41U,  /**< FlexPWM0 Req_capt2 */
    kDma0RequestMuxFlexPwm0ReqCapt3             = 42U,  /**< FlexPWM0 Req_capt3 */
    kDma0RequestMuxFlexPwm0ReqVal0              = 43U,  /**< FlexPWM0 Req_val0 */
    kDma0RequestMuxFlexPwm0ReqVal1              = 44U,  /**< FlexPWM0 Req_val1 */
    kDma0RequestMuxFlexPwm0ReqVal2              = 45U,  /**< FlexPWM0 Req_val2 */
    kDma0RequestMuxFlexPwm0ReqVal3              = 46U,  /**< FlexPWM0 Req_val3 */
    kDma0RequestMuxFlexPwm1ReqCapt0             = 47U,  /**< FlexPWM1 Req_capt0 */
    kDma0RequestMuxFlexPwm1ReqCapt1             = 48U,  /**< FlexPWM1 Req_capt1 */
    kDma0RequestMuxFlexPwm1ReqCapt2             = 49U,  /**< FlexPWM1 Req_capt2 */
    kDma0RequestMuxFlexPwm1ReqCapt3             = 50U,  /**< FlexPWM1 Req_capt3 */
    kDma0RequestMuxFlexPwm1ReqVal0              = 51U,  /**< FlexPWM1 Req_val0 */
    kDma0RequestMuxFlexPwm1ReqVal1              = 52U,  /**< FlexPWM1 Req_val1 */
    kDma0RequestMuxFlexPwm1ReqVal2              = 53U,  /**< FlexPWM1 Req_val2 */
    kDma0RequestMuxFlexPwm1ReqVal3              = 54U,  /**< FlexPWM1 Req_val3 */
    kDma0RequestMuxItrc0TmprOut0                = 55U,  /**< ITRC0 TMPR_OUT0 */
    kDma0RequestMuxItrc0TmprOut1                = 56U,  /**< ITRC0 TMPR_OUT1 */
    kDma0RequestMuxLptmr0                       = 57U,  /**< LPTMR0 Counter match event */
    kDma0RequestMuxLptmr1                       = 58U,  /**< LPTMR1 Counter match event */
    kDma0RequestMuxFlexCan0DmaRequest           = 59U,  /**< FlexCAN0 DMA request */
    kDma0RequestMuxFlexCan1DmaRequest           = 60U,  /**< FlexCAN1 DMA request */
    kDma0RequestMuxFlexIO0ShiftRegister0Request = 61U,  /**< FlexIO0 Shift Register 0 request */
    kDma0RequestMuxFlexIO0ShiftRegister1Request = 62U,  /**< FlexIO0 Shift Register 1 request */
    kDma0RequestMuxFlexIO0ShiftRegister2Request = 63U,  /**< FlexIO0 Shift Register 2 request */
    kDma0RequestMuxFlexIO0ShiftRegister3Request = 64U,  /**< FlexIO0 Shift Register 3 request */
    kDma0RequestMuxFlexIO0ShiftRegister4Request = 65U,  /**< FlexIO0 Shift Register 4 request */
    kDma0RequestMuxFlexIO0ShiftRegister5Request = 66U,  /**< FlexIO0 Shift Register 5 request */
    kDma0RequestMuxFlexIO0ShiftRegister6Request = 67U,  /**< FlexIO0 Shift Register 6 request */
    kDma0RequestMuxFlexIO0ShiftRegister7Request = 68U,  /**< FlexIO0 Shift Register 7 request */
    kDma0RequestMuxLpFlexcomm0Rx                = 69U,  /**< LP_FLEXCOMM0 Receive request */
    kDma0RequestMuxLpFlexcomm0Tx                = 70U,  /**< LP_FLEXCOMM0 Transmit request */
    kDma0RequestMuxLpFlexcomm1Rx                = 71U,  /**< LP_FLEXCOMM1 Receive request */
    kDma0RequestMuxLpFlexcomm1Tx                = 72U,  /**< LP_FLEXCOMM1 Transmit request */
    kDma0RequestMuxLpFlexcomm2Rx                = 73U,  /**< LP_FLEXCOMM2 Receive request */
    kDma0RequestMuxLpFlexcomm2Tx                = 74U,  /**< LP_FLEXCOMM2 Transmit request */
    kDma0RequestMuxLpFlexcomm3Rx                = 75U,  /**< LP_FLEXCOMM3 Receive request */
    kDma0RequestMuxLpFlexcomm3Tx                = 76U,  /**< LP_FLEXCOMM3 Transmit request */
    kDma0RequestMuxLpFlexcomm4Rx                = 77U,  /**< LP_FLEXCOMM4 Receive request */
    kDma0RequestMuxLpFlexcomm4Tx                = 78U,  /**< LP_FLEXCOMM4 Transmit request */
    kDma0RequestMuxLpFlexcomm5Rx                = 79U,  /**< LP_FLEXCOMM5 Receive request */
    kDma0RequestMuxLpFlexcomm5Tx                = 80U,  /**< LP_FLEXCOMM5 Transmit request */
    kDma0RequestMuxLpFlexcomm6Rx                = 81U,  /**< LP_FLEXCOMM6 Receive request */
    kDma0RequestMuxLpFlexcomm6Tx                = 82U,  /**< LP_FLEXCOMM6 Transmit request */
    kDma0RequestMuxLpFlexcomm7Rx                = 83U,  /**< LP_FLEXCOMM7 Receive request */
    kDma0RequestMuxLpFlexcomm7Tx                = 84U,  /**< LP_FLEXCOMM7 Transmit request */
    kDma0RequestMuxLpFlexcomm8Rx                = 85U,  /**< LP_FLEXCOMM8 Receive request */
    kDma0RequestMuxLpFlexcomm8Tx                = 86U,  /**< LP_FLEXCOMM8 Transmit request */
    kDma0RequestMuxLpFlexcomm9Rx                = 87U,  /**< LP_FLEXCOMM9 Receive request */
    kDma0RequestMuxLpFlexcomm9Tx                = 88U,  /**< LP_FLEXCOMM9 Transmit request */
    kDma0RequestMuxESpi0Ch0                     = 89U,  /**< eSPI0 channel 0 */
    kDma0RequestMuxESpi0Ch1                     = 90U,  /**< eSPI0 channel 1 */
    kDma0RequestMuxEmvSim0Rx                    = 91U,  /**< EMVSIM0 Receive request */
    kDma0RequestMuxEmvSim0Tx                    = 92U,  /**< EMVSIM0 Transmit request */
    kDma0RequestMuxEmvSim1Rx                    = 93U,  /**< EMVSIM1 Receive request */
    kDma0RequestMuxEmvSim1Tx                    = 94U,  /**< EMVSIM1 Transmit request */
    kDma0RequestMuxI3c0Rx                       = 95U,  /**< I3C0 Receive request */
    kDma0RequestMuxI3c0Tx                       = 96U,  /**< I3C0 Transmit request */
    kDma0RequestMuxI3c1Rx                       = 97U,  /**< I3C0 Receive request */
    kDma0RequestMuxI3c1Tx                       = 98U,  /**< I3C0 Transmit request */
    kDma0RequestMuxSai0Rx                       = 99U,  /**< SAI0 Receive request */
    kDma0RequestMuxSai0Tx                       = 100U, /**< SAI0 Receive request */
    kDma0RequestMuxSai1Rx                       = 101U, /**< SAI1 Receive request */
    kDma0RequestMuxSai1Tx                       = 102U, /**< SAI1 Receive request */
    kDma0RequestMuxSinc0IpdReqSincAlt0          = 103U, /**< SINC0 ipd_req_sinc[0] or ipd_req_alt [0] */
    kDma0RequestMuxSinc0IpdReqSincAlt1          = 104U, /**< SINC0 ipd_req_sinc[1] or ipd_req_alt [1] */
    kDma0RequestMuxSinc0IpdReqSincAlt2          = 105U, /**< SINC0 ipd_req_sinc[2] or ipd_req_alt [2] */
    kDma0RequestMuxSinc0IpdReqSincAlt3          = 106U, /**< SINC0 ipd_req_sinc[3] or ipd_req_alt [3] */
    kDma0RequestMuxSinc0IpdReqSincAlt4          = 107U, /**< SINC0 ipd_req_sinc[4] or ipd_req_alt [4] */
    kDma0RequestMuxGpio0PinEventRequest0        = 108U, /**< GPIO0 Pin event request 0 */
    kDma0RequestMuxGpio0PinEventRequest1        = 109U, /**< GPIO0 Pin event request 1 */
    kDma0RequestMuxGpio1PinEventRequest0        = 110U, /**< GPIO1 Pin event request 0 */
    kDma0RequestMuxGpio1PinEventRequest1        = 111U, /**< GPIO1 Pin event request 1 */
    kDma0RequestMuxGpio2PinEventRequest0        = 112U, /**< GPIO2 Pin event request 0 */
    kDma0RequestMuxGpio2PinEventRequest1        = 113U, /**< GPIO2 Pin event request 1 */
    kDma0RequestMuxGpio3PinEventRequest0        = 114U, /**< GPIO3 Pin event request 0 */
    kDma0RequestMuxGpio3PinEventRequest1        = 115U, /**< GPIO3 Pin event request 1 */
    kDma0RequestMuxGpio4PinEventRequest0        = 116U, /**< GPIO4 Pin event request 0 */
    kDma0RequestMuxGpio4PinEventRequest1        = 117U, /**< GPIO4 Pin event request 1 */
    kDma0RequestMuxGpio5PinEventRequest0        = 118U, /**< GPIO5 Pin event request 0 */
    kDma0RequestMuxGpio5PinEventRequest1        = 119U, /**< GPIO5 Pin event request 1 */
    kDma0RequestMuxTsi0EndOfScan                = 120U, /**< TSI0 End of Scan */
    kDma0RequestMuxTsi0OutOfRange               = 121U, /**< TSI0 Out of Range */

    kDma1RequestDisabled                        = 0U,   /**< Disabled*/
    kDma1RequestMuxFlexSpi0Rx                   = 1U,   /**< FlexSPI0 Receive event */
    kDma1RequestMuxFlexSpi0Tx                   = 2U,   /**< FlexSPI0 Transmit event */
    kDma1RequestMuxPinInt0                      = 3U,   /**< PinInt0 */
    kDma1RequestMuxPinInt1                      = 4U,   /**< PinInt1 */
    kDma1RequestMuxPinInt2                      = 5U,   /**< PinInt2 */
    kDma1RequestMuxPinInt3                      = 6U,   /**< PinInt3 */
    kDma1RequestMuxCtimer0M0                    = 7U,   /**< Ctimer0_M0 */
    kDma1RequestMuxCtimer0M1                    = 8U,   /**< Ctimer0_M1 */
    kDma1RequestMuxCtimer1M0                    = 9U,   /**< Ctimer1_M0 */
    kDma1RequestMuxCtimer1M1                    = 10U,  /**< Ctimer1_M1 */
    kDma1RequestMuxCtimer2M0                    = 11U,  /**< Ctimer2_M0 */
    kDma1RequestMuxCtimer2M1                    = 12U,  /**< Ctimer2_M1 */
    kDma1RequestMuxCtimer3M0                    = 13U,  /**< Ctimer3_M0 */
    kDma1RequestMuxCtimer3M1                    = 14U,  /**< Ctimer3_M1 */
    kDma1RequestMuxCtimer4M0                    = 15U,  /**< Ctimer4_M0 */
    kDma1RequestMuxCtimer5M1                    = 16U,  /**< Ctimer4_M1 */
    kDma1RequestMuxWuu0                         = 17U,  /**< Wake up event */
    kDma1RequestMuxMicfil0FifoRequest           = 18U,  /**< MICFIL0 FIFO_request */
    kDma1RequestMuxSct0Dma0                     = 19U,  /**< SCT0 DMA0 */
    kDma1RequestMuxSct0Dma1                     = 20U,  /**< SCT0 DMA1 */
    kDma1RequestMuxAdc0FifoARequest             = 21U,  /**< ADC0 FIFO A request */
    kDma1RequestMuxAdc0FifoBRequest             = 22U,  /**< ADC0 FIFO B request */
    kDma1RequestMuxAdc1FifoARequest             = 23U,  /**< ADC1 FIFO A request */
    kDma1RequestMuxAdc1FifoBRequest             = 24U,  /**< ADC1 FIFO B request */
    kDma1RequestMuxDac0FifoRequest              = 25U,  /**< DAC0 FIFO_request */
    kDma1RequestMuxDac1FifoRequest              = 26U,  /**< DAC1 FIFO_request */
    kDma1RequestMuxDac2FifoRequest              = 27U,  /**< HP DAC0 FIFO_request */
    kDma1RequestMuxHsCmp0DmaRequest             = 28U,  /**< HS CMP0 DMA_request */
    kDma1RequestMuxHsCmp1DmaRequest             = 29U,  /**< HS CMP0 DMA_request */
    kDma1RequestMuxHsCmp2DmaRequest             = 30U,  /**< HS CMP0 DMA_request */
    kDma1RequestMuxEvtg0Out0A                   = 31U,  /**< EVTG0 OUT0A */
    kDma1RequestMuxEvtg0Out0B                   = 32U,  /**< EVTG0 OUT0B */
    kDma1RequestMuxEvtg0Out1A                   = 33U,  /**< EVTG0 OUT1A */
    kDma1RequestMuxEvtg0Out1B                   = 34U,  /**< EVTG0 OUT1B */
    kDma1RequestMuxEvtg0Out2A                   = 35U,  /**< EVTG0 OUT2A */
    kDma1RequestMuxEvtg0Out2B                   = 36U,  /**< EVTG0 OUT2B */
    kDma1RequestMuxEvtg0Out3A                   = 37U,  /**< EVTG0 OUT3A */
    kDma1RequestMuxEvtg0Out3B                   = 38U,  /**< EVTG0 OUT3B */
    kDma1RequestMuxFlexPwm0ReqCapt0             = 39U,  /**< FlexPWM0 Req_capt0 */
    kDma1RequestMuxFlexPwm0ReqCapt1             = 40U,  /**< FlexPWM0 Req_capt1 */
    kDma1RequestMuxFlexPwm0ReqCapt2             = 41U,  /**< FlexPWM0 Req_capt2 */
    kDma1RequestMuxFlexPwm0ReqCapt3             = 42U,  /**< FlexPWM0 Req_capt3 */
    kDma1RequestMuxFlexPwm0ReqVal0              = 43U,  /**< FlexPWM0 Req_val0 */
    kDma1RequestMuxFlexPwm0ReqVal1              = 44U,  /**< FlexPWM0 Req_val1 */
    kDma1RequestMuxFlexPwm0ReqVal2              = 45U,  /**< FlexPWM0 Req_val2 */
    kDma1RequestMuxFlexPwm0ReqVal3              = 46U,  /**< FlexPWM0 Req_val3 */
    kDma1RequestMuxFlexPwm1ReqCapt0             = 47U,  /**< FlexPWM1 Req_capt0 */
    kDma1RequestMuxFlexPwm1ReqCapt1             = 48U,  /**< FlexPWM1 Req_capt1 */
    kDma1RequestMuxFlexPwm1ReqCapt2             = 49U,  /**< FlexPWM1 Req_capt2 */
    kDma1RequestMuxFlexPwm1ReqCapt3             = 50U,  /**< FlexPWM1 Req_capt3 */
    kDma1RequestMuxFlexPwm1ReqVal0              = 51U,  /**< FlexPWM1 Req_val0 */
    kDma1RequestMuxFlexPwm1ReqVal1              = 52U,  /**< FlexPWM1 Req_val1 */
    kDma1RequestMuxFlexPwm1ReqVal2              = 53U,  /**< FlexPWM1 Req_val2 */
    kDma1RequestMuxFlexPwm1ReqVal3              = 54U,  /**< FlexPWM1 Req_val3 */
    kDma1RequestMuxItrc0TmprOut0                = 55U,  /**< ITRC0 TMPR_OUT0 */
    kDma1RequestMuxItrc0TmprOut1                = 56U,  /**< ITRC0 TMPR_OUT1 */
    kDma1RequestMuxLptmr0                       = 57U,  /**< LPTMR0 Counter match event */
    kDma1RequestMuxLptmr1                       = 58U,  /**< LPTMR1 Counter match event */
    kDma1RequestMuxFlexCan0DmaRequest           = 59U,  /**< FlexCAN0 DMA request */
    kDma1RequestMuxFlexCan1DmaRequest           = 60U,  /**< FlexCAN1 DMA request */
    kDma1RequestMuxFlexIO0ShiftRegister0Request = 61U,  /**< FlexIO0 Shift Register 0 request */
    kDma1RequestMuxFlexIO0ShiftRegister1Request = 62U,  /**< FlexIO0 Shift Register 1 request */
    kDma1RequestMuxFlexIO0ShiftRegister2Request = 63U,  /**< FlexIO0 Shift Register 2 request */
    kDma1RequestMuxFlexIO0ShiftRegister3Request = 64U,  /**< FlexIO0 Shift Register 3 request */
    kDma1RequestMuxFlexIO0ShiftRegister4Request = 65U,  /**< FlexIO0 Shift Register 4 request */
    kDma1RequestMuxFlexIO0ShiftRegister5Request = 66U,  /**< FlexIO0 Shift Register 5 request */
    kDma1RequestMuxFlexIO0ShiftRegister6Request = 67U,  /**< FlexIO0 Shift Register 6 request */
    kDma1RequestMuxFlexIO0ShiftRegister7Request = 68U,  /**< FlexIO0 Shift Register 7 request */
    kDma1RequestMuxLpFlexcomm0Rx                = 69U,  /**< LP_FLEXCOMM0 Receive request */
    kDma1RequestMuxLpFlexcomm0Tx                = 70U,  /**< LP_FLEXCOMM0 Transmit request */
    kDma1RequestMuxLpFlexcomm1Rx                = 71U,  /**< LP_FLEXCOMM1 Receive request */
    kDma1RequestMuxLpFlexcomm1Tx                = 72U,  /**< LP_FLEXCOMM1 Transmit request */
    kDma1RequestMuxLpFlexcomm2Rx                = 73U,  /**< LP_FLEXCOMM2 Receive request */
    kDma1RequestMuxLpFlexcomm2Tx                = 74U,  /**< LP_FLEXCOMM2 Transmit request */
    kDma1RequestMuxLpFlexcomm3Rx                = 75U,  /**< LP_FLEXCOMM3 Receive request */
    kDma1RequestMuxLpFlexcomm3Tx                = 76U,  /**< LP_FLEXCOMM3 Transmit request */
    kDma1RequestMuxLpFlexcomm4Rx                = 77U,  /**< LP_FLEXCOMM4 Receive request */
    kDma1RequestMuxLpFlexcomm4Tx                = 78U,  /**< LP_FLEXCOMM4 Transmit request */
    kDma1RequestMuxLpFlexcomm5Rx                = 79U,  /**< LP_FLEXCOMM5 Receive request */
    kDma1RequestMuxLpFlexcomm5Tx                = 80U,  /**< LP_FLEXCOMM5 Transmit request */
    kDma1RequestMuxLpFlexcomm6Rx                = 81U,  /**< LP_FLEXCOMM6 Receive request */
    kDma1RequestMuxLpFlexcomm6Tx                = 82U,  /**< LP_FLEXCOMM6 Transmit request */
    kDma1RequestMuxLpFlexcomm7Rx                = 83U,  /**< LP_FLEXCOMM7 Receive request */
    kDma1RequestMuxLpFlexcomm7Tx                = 84U,  /**< LP_FLEXCOMM7 Transmit request */
    kDma1RequestMuxLpFlexcomm8Rx                = 85U,  /**< LP_FLEXCOMM8 Receive request */
    kDma1RequestMuxLpFlexcomm8Tx                = 86U,  /**< LP_FLEXCOMM8 Transmit request */
    kDma1RequestMuxLpFlexcomm9Rx                = 87U,  /**< LP_FLEXCOMM9 Receive request */
    kDma1RequestMuxLpFlexcomm9Tx                = 88U,  /**< LP_FLEXCOMM9 Transmit request */
    kDma1RequestMuxESpi0Ch0                     = 89U,  /**< eSPI0 channel 0 */
    kDma1RequestMuxESpi0Ch1                     = 90U,  /**< eSPI0 channel 1 */
    kDma1RequestMuxEmvSim0Rx                    = 91U,  /**< EMVSIM0 Receive request */
    kDma1RequestMuxEmvSim0Tx                    = 92U,  /**< EMVSIM0 Transmit request */
    kDma1RequestMuxEmvSim1Rx                    = 93U,  /**< EMVSIM1 Receive request */
    kDma1RequestMuxEmvSim1Tx                    = 94U,  /**< EMVSIM1 Transmit request */
    kDma1RequestMuxI3c0Rx                       = 95U,  /**< I3C0 Receive request */
    kDma1RequestMuxI3c0Tx                       = 96U,  /**< I3C0 Transmit request */
    kDma1RequestMuxI3c1Rx                       = 97U,  /**< I3C0 Receive request */
    kDma1RequestMuxI3c1Tx                       = 98U,  /**< I3C0 Transmit request */
    kDma1RequestMuxSai0Rx                       = 99U,  /**< SAI0 Receive request */
    kDma1RequestMuxSai0Tx                       = 100U, /**< SAI0 Receive request */
    kDma1RequestMuxSai1Rx                       = 101U, /**< SAI1 Receive request */
    kDma1RequestMuxSai1Tx                       = 102U, /**< SAI1 Receive request */
    kDma1RequestMuxSinc0IpdReqSincAlt0          = 103U, /**< SINC0 ipd_req_sinc[0] or ipd_req_alt [0] */
    kDma1RequestMuxSinc0IpdReqSincAlt1          = 104U, /**< SINC0 ipd_req_sinc[1] or ipd_req_alt [1] */
    kDma1RequestMuxSinc0IpdReqSincAlt2          = 105U, /**< SINC0 ipd_req_sinc[2] or ipd_req_alt [2] */
    kDma1RequestMuxSinc0IpdReqSincAlt3          = 106U, /**< SINC0 ipd_req_sinc[3] or ipd_req_alt [3] */
    kDma1RequestMuxSinc0IpdReqSincAlt4          = 107U, /**< SINC0 ipd_req_sinc[4] or ipd_req_alt [4] */
    kDma1RequestMuxGpio0PinEventRequest0        = 108U, /**< GPIO0 Pin event request 0 */
    kDma1RequestMuxGpio0PinEventRequest1        = 109U, /**< GPIO0 Pin event request 1 */
    kDma1RequestMuxGpio1PinEventRequest0        = 110U, /**< GPIO1 Pin event request 0 */
    kDma1RequestMuxGpio1PinEventRequest1        = 111U, /**< GPIO1 Pin event request 1 */
    kDma1RequestMuxGpio2PinEventRequest0        = 112U, /**< GPIO2 Pin event request 0 */
    kDma1RequestMuxGpio2PinEventRequest1        = 113U, /**< GPIO2 Pin event request 1 */
    kDma1RequestMuxGpio3PinEventRequest0        = 114U, /**< GPIO3 Pin event request 0 */
    kDma1RequestMuxGpio3PinEventRequest1        = 115U, /**< GPIO3 Pin event request 1 */
    kDma1RequestMuxGpio4PinEventRequest0        = 116U, /**< GPIO4 Pin event request 0 */
    kDma1RequestMuxGpio4PinEventRequest1        = 117U, /**< GPIO4 Pin event request 1 */
    kDma1RequestMuxGpio5PinEventRequest0        = 118U, /**< GPIO5 Pin event request 0 */
    kDma1RequestMuxGpio5PinEventRequest1        = 119U, /**< GPIO5 Pin event request 1 */
    kDma1RequestMuxTsi0EndOfScan                = 120U, /**< TSI0 End of Scan */
    kDma1RequestMuxTsi0OutOfRange               = 121U, /**< TSI0 Out of Range */
} dma_request_source_t;

#define FSL_FEATURE_EDMA_MODULE_CHANNEL(base)                (16U)
#define FSL_FEATURE_EDMA_MODULE_MAX_CHANNEL                  (16)
#define FSL_FEATURE_EDMA_HAS_GLOBAL_MASTER_ID_REPLICATION    (1)
#define FSL_FEATURE_EDMA_HAS_CONTINUOUS_LINK_MODE            (0)
#define FSL_FEATURE_EDMA_MODULE_COUNT                        (2)
#define FSL_FEATURE_EDMA_HAS_CHANNEL_CONFIG                  (1)
#define FSL_FEATURE_EDMA_HAS_CHANNEL_SWAP_SIZE               (0)
#define FSL_FEATURE_EDMA_HAS_CHANNEL_ACCESS_TYPE             (0)
#define FSL_FEATURE_EDMA_HAS_CHANNEL_MEMRORY_ATTRIBUTE       (0)
#define FSL_FEATURE_EDMA_HAS_CHANNEL_SIGN_EXTENSION          (0)
#define FSL_FEATURE_EDMA_MODULE_SUPPORT_MATTR(base)          (0U)
#define FSL_FEATURE_EDMA_MODULE_SUPPORT_SIGN_EXTENSION(base) (0U)
#define FSL_FEATURE_EDMA_MODULE_SUPPORT_SWAP(base)           (0U)
#define FSL_FEATURE_EDMA_MODULE_SUPPORT_INSTR(base)          (0U)

/*!@brief EDMA base address convert macro */
#define EDMA_CHANNEL_OFFSET           0x1000U
#define EDMA_CHANNEL_ARRAY_STEP(base) (0x1000U)

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*!
 * @}
 */

#endif /* _FSL_EDMA_SOC_H_ */
