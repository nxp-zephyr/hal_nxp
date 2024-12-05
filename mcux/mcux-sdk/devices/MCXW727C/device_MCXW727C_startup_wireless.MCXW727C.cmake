# Add set(CONFIG_USE_device_MCXW727C_startup_wireless true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND CONFIG_CORE_ID STREQUAL cm33_core0)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/iar/startup_MCXW727C_cm33_core0.s
      ${CMAKE_CURRENT_LIST_DIR}/iar/startup_MCXW727C_cm33_core1.s
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33 AND CONFIG_CORE_ID STREQUAL cm33_core0)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MCXW727C_cm33_core0.S
      ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MCXW727C_cm33_core1.S
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33 AND CONFIG_CORE_ID STREQUAL cm33_core0)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MCXW727C_cm33_core0.S
      ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MCXW727C_cm33_core1.S
  )
endif()

