if (CONFIG_MCUX_COMPONENT_component.i2c_mux)
if (CONFIG_MCUX_COMPONENT_component.i2c_mux_pca954x)
    mcux_add_source(
        SOURCES fsl_pca954x.h
        SOURCES fsl_pca954x.c
    )
    mcux_add_include(
        INCLUDES ./
    )
if (CONFIG_MCUX_COMPONENT_component.enable_max7356)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_MAX7356"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_max7357)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_MAX7357"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_max7358)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_MAX7358"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_max7367)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_MAX7367"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_max7368)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_MAX7368"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_max7369)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_MAX7369"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_pca9540)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_PCA9540"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_pca9542)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_PCA9542"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_pca9543)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_PCA9543"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_pca9544)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_PCA9544"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_pca9545)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_PCA9545"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_pca9546)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_PCA9546"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_pca9547)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_PCA9547"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_pca9548)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_PCA9548"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_pca9646)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_PCA9646"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_pca9846)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_PCA9846"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_pca9847)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_PCA9847"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_pca9848)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_PCA9848"
    )
endif()
if (CONFIG_MCUX_COMPONENT_component.enable_pca9849)
    mcux_add_configuration(
	    CC "-DMCUX_ENABLE_PCA9849"
    )
endif()

endif()
endif()
