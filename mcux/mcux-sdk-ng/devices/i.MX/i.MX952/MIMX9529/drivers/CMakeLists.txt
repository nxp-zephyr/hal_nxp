# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_include( INCLUDES . )

if (CONFIG_MCUX_COMPONENT_driver.clock)
    mcux_component_version(1.0.0)
    mcux_add_source( SOURCES fsl_clock.c fsl_clock.h )
endif()

if (CONFIG_MCUX_COMPONENT_driver.edma_soc)
    mcux_component_version(2.0.0)
    mcux_add_source( SOURCES fsl_edma_soc.c fsl_edma_soc.h )
endif()

if (CONFIG_MCUX_COMPONENT_driver.iomuxc)
    mcux_component_version(1.0.0)
    mcux_add_source( SOURCES fsl_iomuxc.h )
endif()

if (CONFIG_MCUX_COMPONENT_driver.power)
    mcux_component_version(1.0.0)
    mcux_add_source( SOURCES fsl_power.c fsl_power.h )
endif()

if (CONFIG_MCUX_COMPONENT_driver.memory)
    mcux_component_version(1.0.0)
    mcux_add_source( SOURCES fsl_memory.h )
endif()

if (CONFIG_MCUX_COMPONENT_driver.trdc_soc)
    mcux_component_version(2.0.0)
    mcux_add_source( SOURCES fsl_trdc_soc.h )
endif()

if (CONFIG_MCUX_COMPONENT_driver.irqsteer_1_soc)
    mcux_component_version(1.0.0)
    mcux_add_source(
            SOURCES fsl_irqsteer_soc.h fsl_irqsteer_soc.c )
endif()
