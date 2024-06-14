zephyr_include_directories(Common)
zephyr_include_directories(OSAbstraction/Interface)
zephyr_include_directories(XCVR/${MCUX_DEVICE})
zephyr_include_directories(FunctionLib) # temp. mayank mahajan.
	# this is being included in the hal_nxp.cmake as well.

add_subdirectory(XCVR/${MCUX_DEVICE})

zephyr_library_sources(OSAbstraction/Source/fsl_os_abstraction_zephyr.c)
