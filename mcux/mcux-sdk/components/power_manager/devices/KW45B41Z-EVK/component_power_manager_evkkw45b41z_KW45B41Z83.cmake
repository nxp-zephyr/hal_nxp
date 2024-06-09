include_guard(GLOBAL)
message("component_power_manager_evkkw45b41z component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_pm_board.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(component_power_manager_core_KW45B41Z83)

include(driver_spc)

include(driver_cmc)

include(driver_wuu)

include(driver_reset)

include(driver_crc)

