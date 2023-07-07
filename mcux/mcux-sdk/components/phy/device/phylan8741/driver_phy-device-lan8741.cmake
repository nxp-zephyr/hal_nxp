#Description: Driver phy-device-lan8741; user_visible: True
include_guard(GLOBAL)
message("driver_phy-device-lan8741 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_phylan8741.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)

#OR Logic component
if(${MCUX_DEVICE} STREQUAL "MCXN547_cm33_core0")
    include(driver_phy-common_MCXN547_cm33_core0)
endif()
if(${MCUX_DEVICE} STREQUAL "MCXN947_cm33_core0")
    include(driver_phy-common_MCXN947_cm33_core0)
endif()

