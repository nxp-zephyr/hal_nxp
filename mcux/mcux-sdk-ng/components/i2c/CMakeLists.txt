# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_component.flexcomm_i2c_adapter)
    mcux_add_source(
        SOURCES fsl_adapter_flexcomm_i2c.c
    )
endif()

if (CONFIG_MCUX_COMPONENT_component.i2c_adapter)
    mcux_add_source(
        SOURCES fsl_adapter_i2c.c
    )
endif()

if (CONFIG_MCUX_COMPONENT_component.ii2c_adapter)
    mcux_add_source(
        SOURCES fsl_adapter_ii2c.c
    )
endif()

if (CONFIG_MCUX_COMPONENT_component.lpc_i2c_adapter)
    mcux_add_source(
        SOURCES fsl_adapter_lpc_i2c.c
    )
endif()

if (CONFIG_MCUX_COMPONENT_component.lpi2c_adapter)
    mcux_add_source(
        SOURCES fsl_adapter_lpi2c.c
    )
endif()

if (CONFIG_MCUX_COMPONENT_component.i3c_adapter)
    mcux_add_source(
        SOURCES fsl_adapter_i3c.c
    )

    mcux_add_macro(
        CC "-DSDK_I3C_BASED_COMPONENT_USED=1"
    )
endif()

if (CONFIG_MCUX_COMPONENT_component.dsc_i2c_adapter)
    mcux_add_source(
        SOURCES fsl_adapter_dsc_i2c.c
    )
endif()

if (CONFIG_MCUX_COMPONENT_component.i2c_adapter_interface)
    mcux_add_source(
        SOURCES fsl_adapter_i2c.h
    )
    mcux_add_include(
        INCLUDES ./
    )
endif()
mcux_add_cmakelists(${SdkRootDirPath}/components/i2c/muxes)
