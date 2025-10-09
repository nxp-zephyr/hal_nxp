# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_component.ele_crypto)
    mcux_add_source(
        SOURCES ele_crypto.c
                ele_crypto.h
                ele_crypto_internal.h
    )
    mcux_add_include(
        INCLUDES ./
    )

mcux_add_macro(
    CC "-DCACHE_MODE_WRITE_THROUGH=1"
)

endif()

if (CONFIG_MCUX_COMPONENT_component.ele_nvm_manager)
    mcux_add_source(
        SOURCES ele_nvm_manager.c
                ele_nvm_manager.h
    )
    mcux_add_include(
        INCLUDES ./
    )
endif()
