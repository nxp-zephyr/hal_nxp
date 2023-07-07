list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/.
    ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/Core/Include
    ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/DSP
    ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/Driver/Include
    ${CMAKE_CURRENT_LIST_DIR}/../../cmsis_drivers/lpi2c
    ${CMAKE_CURRENT_LIST_DIR}/../../cmsis_drivers/lpspi
    ${CMAKE_CURRENT_LIST_DIR}/../../cmsis_drivers/lpuart
    ${CMAKE_CURRENT_LIST_DIR}/../../components/button
    ${CMAKE_CURRENT_LIST_DIR}/../../components/common_task
    ${CMAKE_CURRENT_LIST_DIR}/../../components/els_pkc
    ${CMAKE_CURRENT_LIST_DIR}/../../components/flash/mflash
    ${CMAKE_CURRENT_LIST_DIR}/../../components/flash/mflash/mcxnx4x
    ${CMAKE_CURRENT_LIST_DIR}/../../components/flash/mflash/mcxnx4x_flexspi
    ${CMAKE_CURRENT_LIST_DIR}/../../components/ft6x06
    ${CMAKE_CURRENT_LIST_DIR}/../../components/gpio
    ${CMAKE_CURRENT_LIST_DIR}/../../components/i2c
    ${CMAKE_CURRENT_LIST_DIR}/../../components/ili9341
    ${CMAKE_CURRENT_LIST_DIR}/../../components/lists
    ${CMAKE_CURRENT_LIST_DIR}/../../components/log
    ${CMAKE_CURRENT_LIST_DIR}/../../components/osa
    ${CMAKE_CURRENT_LIST_DIR}/../../components/p3t1755
    ${CMAKE_CURRENT_LIST_DIR}/../../components/phy
    ${CMAKE_CURRENT_LIST_DIR}/../../components/phy/device/phylan8741
    ${CMAKE_CURRENT_LIST_DIR}/../../components/serial_manager
    ${CMAKE_CURRENT_LIST_DIR}/../../components/silicon_id
    ${CMAKE_CURRENT_LIST_DIR}/../../components/ssd1963
    ${CMAKE_CURRENT_LIST_DIR}/../../components/timer
    ${CMAKE_CURRENT_LIST_DIR}/../../components/timer_manager
    ${CMAKE_CURRENT_LIST_DIR}/../../components/uart
    ${CMAKE_CURRENT_LIST_DIR}/../../components/video/display/dbi
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/cache/cache64
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/cdog
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/common
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/crc
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/ctimer
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/dac14
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/dac_1
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/edma4
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/eim
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/enc
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/erm
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/evtg
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/ewm
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/flexcan
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/flexio
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/flexspi
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/flexspi/flexspi_dma3
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/gdet
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/gpio
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/i3c
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/inputmux
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/intm
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/irtc
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/itrc
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/lpadc
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/lpc_freqme
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/lpcmp
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/lpflexcomm
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/lptmr
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/mailbox
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/mcx_cmc
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/mcx_enet
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/mcx_romapi
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/mcx_spc
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/mcx_vbat
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/mrt
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/opamp
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/ostimer
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/pdm
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/pint
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/plu
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/port
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/powerquad
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/puf_v3
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/pwm
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/sctimer
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/syspm
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/tdet
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/trdc_1
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/trng
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/tsi/tsi_v6
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/usdhc
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/utick
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/vref_1
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/wuu
    ${CMAKE_CURRENT_LIST_DIR}/../../drivers/wwdt
    ${CMAKE_CURRENT_LIST_DIR}/../../../middleware
    ${CMAKE_CURRENT_LIST_DIR}/../../../rtos/freertos/freertos-kernel
    ${CMAKE_CURRENT_LIST_DIR}/../../utilities
    ${CMAKE_CURRENT_LIST_DIR}/../../utilities/assert
    ${CMAKE_CURRENT_LIST_DIR}/../../utilities/misc_utilities
    ${CMAKE_CURRENT_LIST_DIR}/../../utilities/unity
    ${CMAKE_CURRENT_LIST_DIR}/drivers
    ${CMAKE_CURRENT_LIST_DIR}/utilities
)


# Copy the cmake components into projects
#    include(driver_lpflexcomm_lpuart_freertos)
#    include(utility_incbin)
#    include(driver_wuu)
#    include(driver_cdog)
#    include(driver_ctimer)
#    include(driver_mcx_cmc)
#    include(driver_lpuart)
#    include(driver_cmsis_lpuart)
#    include(component_els_pkc_random)
#    include(driver_lpadc)
#    include(driver_sctimer)
#    include(middleware_freertos-kernel_cm33_nonsecure_port)
#    include(driver_flexio_uart)
#    include(driver_ewm)
#    include(driver_cmsis_lpspi)
#    include(driver_opamp)
#    include(middleware_freertos-kernel_extension)
#    include(driver_flexio)
#    include(driver_edma4)
#    include(driver_enc)
#    include(driver_flexio_i2c_master)
#    include(middleware_baremetal)
#    include(driver_lpc_freqme)
#    include(driver_lpuart_edma)
#    include(driver_lpspi_edma)
#    include(component_els_pkc_padding)
#    include(driver_lpcmp)
#    include(driver_flexio_uart_edma)
#    include(component_els_pkc_ecc)
#    include(component_els_pkc_core)
#    include(driver_ostimer)
#    include(driver_syspm)
#    include(component_els_pkc_cipher_modes)
#    include(driver_wwdt)
#    include(component_gpio_adapter)
#    include(driver_inputmux)
#    include(component_serial_manager)
#    include(component_serial_manager_uart)
#    include(driver_eim)
#    include(component_log_backend_debugconsole)
#    include(component_els_pkc_trng_type_els)
#    include(driver_usdhc)
#    include(driver_ili9341)
#    include(driver_pint)
#    include(component_lpuart_adapter)
#    include(component_els_pkc_session)
#    include(driver_lpi2c_edma)
#    include(utilities_misc_utilities_MCXN947_cm33_core0)
#    include(driver_flashiap)
#    include(driver_lpflexcomm_lpspi_freertos)
#    include(driver_phy-device-lan8741)
#    include(utility_assert)
#    include(component_els_pkc_mac_modes)
#    include(component_els_pkc_param_integrity)
#    include(component_mflash_mcxnx4x_onchip)
#    include(CMSIS_Driver_Include_I2C)
#    include(driver_utick)
#    include(driver_cmsis_lpi2c)
#    include(component_mflash_mcxnx4x_flexspi)
#    include(driver_clock)
#    include(device_CMSIS)
#    include(component_els_pkc_flow_protection)
#    include(CMSIS_Driver_Include_Common)
#    include(component_osa_bm)
#    include(middleware_freertos-kernel_MCXN947_cm33_core0)
#    include(driver_common)
#    include(driver_cache_cache64)
#    include(component_els_pkc_pre_processor)
#    include(driver_vref_1)
#    include(component_mrt_adapter)
#    include(component_osa_free_rtos)
#    include(component_els_pkc_aead)
#    include(component_lpi2c_adapter)
#    include(middleware_freertos-kernel_heap_3)
#    include(CMSIS_Include_core_cm)
#    include(middleware_freertos-kernel_heap_4)
#    include(driver_flexio_mculcd)
#    include(driver_powerquad_cmsis)
#    include(driver_trdc_soc)
#    include(driver_evtg)
#    include(component_els_pkc_hash)
#    include(component_els_pkc_pkc)
#    include(CMSIS_DSP_Source)
#    include(driver_tdet)
#    include(driver_port)
#    include(driver_pdm)
#    include(driver_lpspi)
#    include(driver_gpio)
#    include(component_log)
#    include(utilities_unity)
#    include(driver_plu)
#    include(CMSIS_Driver_Include_USART)
#    include(CMSIS_Driver_Include_SPI)
#    include(utility_debug_console_lite)
#    include(driver_dbi)
#    include(driver_mailbox)
#    include(driver_p3t1755)
#    include(component_els_pkc_math)
#    include(utility_shell)
#    include(driver_edma_soc)
#    include(utility_assert_lite)
#    include(driver_tsi_v6)
#    include(component_els_pkc_key)
#    include(component_els_pkc_mac)
#    include(driver_gdet)
#    include(driver_flexcan)
#    include(utility_debug_console)
#    include(component_els_pkc_cipher)
#    include(component_osa_thread)
#    include(component_els_pkc_rsa)
#    include(component_mflash_common)
#    include(component_els_pkc_trng)
#    include(driver_crc)
#    include(driver_trdc_1)
#    include(device_system_MCXN947_cm33_core0)
#    include(driver_lpflexcomm_lpi2c_freertos)
#    include(driver_erm)
#    include(driver_puf_v3)
#    include(driver_lptmr)
#    include(driver_power)
#    include(driver_lpflexcomm)
#    include(CMSIS_DSP_Include)
#    include(driver_flexio_mculcd_edma)
#    include(component_silicon_id_MCXN947_cm33_core0)
#    include(driver_dac_1)
#    include(driver_mem_interface)
#    include(component_button_MCXN947_cm33_core0)
#    include(driver_flexspi_dma3)
#    include(driver_flexspi)
#    include(driver_flexio_spi_edma)
#    include(component_timer_manager)
#    include(driver_trng)
#    include(component_common_task)
#    include(component_lists)
#    include(driver_mrt)
#    include(driver_irtc)
#    include(driver_mcx_spc)
#    include(component_els_pkc_aead_modes)
#    include(component_osa)
#    include(component_els_pkc_memory)
#    include(component_mflash_file_MCXN947_cm33_core0)
#    include(driver_itrc)
#    include(driver_flexio_spi)
#    include(component_els_pkc_els)
#    include(driver_dac14)
#    include(device_startup_MCXN947_cm33_core0)
#    include(component_els_pkc_secure_counter)
#    include(driver_runbootloader)
#    include(driver_inputmux_connections)
#    include(driver_i3c)
#    include(component_els_pkc_random_modes)
#    include(driver_lpi2c)
#    include(driver_powerquad)
#    include(driver_mcx_enet)
#    include(driver_phy-common_MCXN947_cm33_core0)
#    include(component_els_pkc_prng)
#    include(driver_mcx_vbat)
#    include(driver_pwm)
#    include(driver_ssd1963)
#    include(driver_pdm_edma)
#    include(component_els_pkc_aes)
#    include(component_els_pkc_toolchain)
#    include(driver_reset)
#    include(driver_intm)
#    include(driver_ft6x06)
