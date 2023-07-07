list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/.
    ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/Core/Include
    ${CMAKE_CURRENT_LIST_DIR}/../../components/lists
    ${CMAKE_CURRENT_LIST_DIR}/../../components/uart
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/common
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/gpio
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/inputmux
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/lpflexcomm
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/mailbox
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/mcx_spc
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/port
    ${CMAKE_CURRENT_LIST_DIR}/../../../middleware
    ${CMAKE_CURRENT_LIST_DIR}/../../../rtos/freertos/freertos-kernel
    ${CMAKE_CURRENT_LIST_DIR}/../../utilities
    ${CMAKE_CURRENT_LIST_DIR}/../../utilities/assert
    ${CMAKE_CURRENT_LIST_DIR}/../../utilities/misc_utilities
    ${CMAKE_CURRENT_LIST_DIR}/drivers
)


# Copy the cmake components into projects
#    include(component_lpuart_adapter)
#    include(driver_port)
#    include(driver_gpio)
#    include(component_lists)
#    include(driver_clock)
#    include(middleware_freertos-kernel_cm33_nonsecure_port)
#    include(driver_inputmux_connections)
#    include(driver_mailbox)
#    include(driver_lpuart)
#    include(middleware_freertos-kernel_extension)
#    include(utility_assert_lite)
#    include(device_startup_MCXN947_cm33_core1)
#    include(middleware_baremetal)
#    include(driver_mcx_spc)
#    include(device_CMSIS)
#    include(utility_debug_console_lite)
#    include(driver_common)
#    include(middleware_freertos-kernel_MCXN947_cm33_core1)
#    include(device_system_MCXN947_cm33_core1)
#    include(driver_inputmux)
#    include(driver_lpflexcomm)
#    include(CMSIS_Include_core_cm)
#    include(middleware_freertos-kernel_heap_4)
#    include(utilities_misc_utilities_MCXN947_cm33_core1)
#    include(driver_reset)
