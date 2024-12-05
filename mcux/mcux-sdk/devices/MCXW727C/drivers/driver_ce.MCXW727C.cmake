# Add set(CONFIG_USE_driver_ce true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MCXW727CxxxA) AND CONFIG_USE_driver_common)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_ce_basic.c
  ${CMAKE_CURRENT_LIST_DIR}/fsl_ce_cmd.c
  ${CMAKE_CURRENT_LIST_DIR}/fsl_ce_cmsis.c
  ${CMAKE_CURRENT_LIST_DIR}/fsl_ce_matrix.c
  ${CMAKE_CURRENT_LIST_DIR}/fsl_ce_transform.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "driver_ce.MCXW727C dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
