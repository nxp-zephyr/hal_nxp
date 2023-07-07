#Description: FLEXIO MCULCD EDMA Driver; user_visible: True
include_guard(GLOBAL)
message("driver_flexio_mculcd_edma component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_flexio_mculcd_edma.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)

#OR Logic component
if(${MCUX_DEVICE} STREQUAL "MCXN946_cm33_core0")
    include(driver_edma4)
endif()
if(${MCUX_DEVICE} STREQUAL "MCXN547_cm33_core0")
    include(driver_edma4)
endif()
if(${MCUX_DEVICE} STREQUAL "MCXN546_cm33_core0")
    include(driver_edma4)
endif()
if(${MCUX_DEVICE} STREQUAL "MCXN947_cm33_core0")
    include(driver_edma4)
endif()
if(${MCUX_DEVICE} STREQUAL "MK28FA15")
    include(driver_edma_MK28FA15)
endif()

include(driver_flexio_mculcd)
