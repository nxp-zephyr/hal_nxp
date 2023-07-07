#Description: LPFLEXCOMM LPSPI FREERTOS Driver; user_visible: True
include_guard(GLOBAL)
message("driver_lpflexcomm_lpspi_freertos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_lpspi_freertos.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)

#OR Logic component
if(${MCUX_DEVICE} STREQUAL "MCXN946_cm33_core0")
    include(middleware_freertos-kernel_MCXN947_cm33_core0)
endif()
if(${MCUX_DEVICE} STREQUAL "MCXN547_cm33_core0")
    include(middleware_freertos-kernel_MCXN547_cm33_core0)
endif()
if(${MCUX_DEVICE} STREQUAL "MCXN546_cm33_core0")
    include(middleware_freertos-kernel_MCXN547_cm33_core0)
endif()
if(${MCUX_DEVICE} STREQUAL "MCXN947_cm33_core0")
    include(middleware_freertos-kernel_MCXN947_cm33_core0)
endif()

include(driver_lpspi)
