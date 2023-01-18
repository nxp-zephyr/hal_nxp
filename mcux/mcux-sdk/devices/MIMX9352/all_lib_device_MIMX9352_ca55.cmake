list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/.
    ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/Core_AArch64/Include
    ${CMAKE_CURRENT_LIST_DIR}/../../components/i2c
    ${CMAKE_CURRENT_LIST_DIR}/../../components/lists
    ${CMAKE_CURRENT_LIST_DIR}/../../components/phy
    ${CMAKE_CURRENT_LIST_DIR}/../../components/phy/device/phyrtl8211f
    ${CMAKE_CURRENT_LIST_DIR}/../../components/phy/mdio
    ${CMAKE_CURRENT_LIST_DIR}/../../components/phy/mdio/enet
    ${CMAKE_CURRENT_LIST_DIR}/../../components/phy/mdio/enet_qos
    ${CMAKE_CURRENT_LIST_DIR}/../../components/serial_manager
    ${CMAKE_CURRENT_LIST_DIR}/../../components/uart
    ${CMAKE_CURRENT_LIST_DIR}/../../mcux/mcux-sdk/drivers/cache/armv8-a
    ${CMAKE_CURRENT_LIST_DIR}/../../mcux/mcux-sdk/drivers/common
    ${CMAKE_CURRENT_LIST_DIR}/../../mcux/mcux-sdk/drivers/enet
    ${CMAKE_CURRENT_LIST_DIR}/../../mcux/mcux-sdk/drivers/enet_qos
    ${CMAKE_CURRENT_LIST_DIR}/../../mcux/mcux-sdk/drivers/flexcan
    ${CMAKE_CURRENT_LIST_DIR}/../../mcux/mcux-sdk/drivers/lptmr
    ${CMAKE_CURRENT_LIST_DIR}/../../mcux/mcux-sdk/drivers/lpi2c
    ${CMAKE_CURRENT_LIST_DIR}/../../mcux/mcux-sdk/drivers/lpuart
    ${CMAKE_CURRENT_LIST_DIR}/../../mcux/mcux-sdk/drivers/rgpio
    ${CMAKE_CURRENT_LIST_DIR}/../../mcux/mcux-sdk/drivers/sai
    ${CMAKE_CURRENT_LIST_DIR}/../../utilities/assert
    ${CMAKE_CURRENT_LIST_DIR}/../../utilities/debug_console
    ${CMAKE_CURRENT_LIST_DIR}/../../utilities/misc_utilities
    ${CMAKE_CURRENT_LIST_DIR}/mcux/mcux-sdk/drivers
)


# Copy the cmake components into projects
#    include(driver_lpi2c)
#    include(driver_lpi2c_edma)
#    include(driver_lpi2c_freertos)
#    include(driver_lpuart_dma)
#    include(driver_lpuart_edma)
#    include(driver_lpuart_freertos)
#    include(component_lpi2c_adapter)
#    include(component_lpuart_adapter)
#    include(component_lists)
#    include(component_serial_manager)
#    include(component_serial_manager_uart)
#    include(device_startup)
#    include(device_system)
#    include(driver_cache_armv8a)
#    include(driver_common)
#    include(driver_enet)
#    include(driver_flexcan)
#    include(driver_lptmr)
#    include(driver_rgpio)
#    include(driver_lpuart)
#    include(driver_mdio-enet)
#    include(driver_phy-device-rtl8211f)
#    include(driver_sai)
#    include(middleware_freertos-kernel_aarch64)
#    include(middleware_freertos-kernel_extension)
#    include(middleware_freertos-kernel_heap_4)
#    include(utilities_misc_utilities)
#    include(utility_assert)
#    include(utility_debug_console)
#    include(utility_debug_console_lite)
