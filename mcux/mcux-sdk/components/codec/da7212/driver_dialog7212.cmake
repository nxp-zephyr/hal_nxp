#Description: Driver dialog7212; user_visible: True
include_guard(GLOBAL)
message("driver_dialog7212 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_dialog7212.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)

#OR Logic component
if(${MCUX_DEVICE} STREQUAL "MCXN547_cm33_core0")
    include(component_codec_i2c_MCXN547_cm33_core0)
endif()
if(${MCUX_DEVICE} STREQUAL "MK66F18")
    include(component_codec_i2c_MK66F18)
endif()

