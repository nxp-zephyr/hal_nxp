include_guard()
message("driver_runbootloader component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/runbootloader/src/fsl_runbootloader.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/mem_interface
    ${CMAKE_CURRENT_LIST_DIR}/flash
    ${CMAKE_CURRENT_LIST_DIR}/nboot
    ${CMAKE_CURRENT_LIST_DIR}/runbootloader
)


include(driver_common_MCXN947_cm33_core0)

