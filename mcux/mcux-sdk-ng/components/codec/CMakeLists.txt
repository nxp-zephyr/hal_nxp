# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_driver.codec)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
    )

    mcux_add_source(
        SOURCES ./fsl_codec_common.h
                ./fsl_codec_common.c
    )

    mcux_add_include(
        INCLUDES ./
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.codec_adapters)
    mcux_add_configuration(
        CC "-DCODEC_MULTI_ADAPTERS=1"
    )


    mcux_add_source(
        SOURCES ./port/fsl_codec_adapter.c
    )

    mcux_add_include(
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.codec_i2c)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
    )

    mcux_add_source(
        SOURCES ./i2c/fsl_codec_i2c.h
                ./i2c/fsl_codec_i2c.c
    )

    mcux_add_include(
        INCLUDES ./i2c
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.wm8904_adapter)

    mcux_add_source(
        SOURCES ./port/wm8904/fsl_codec_wm8904_adapter.h
                ./port/wm8904/fsl_codec_wm8904_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/wm8904
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.wm8960_adapter)

    mcux_add_source(
        SOURCES ./port/wm8960/fsl_codec_wm8960_adapter.h
                ./port/wm8960/fsl_codec_wm8960_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/wm8960
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.wm8962_adapter)

    mcux_add_source(
        SOURCES ./port/wm8962/fsl_codec_wm8962_adapter.h
                ./port/wm8962/fsl_codec_wm8962_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/wm8962
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.wm8524_adapter)

    mcux_add_source(
        SOURCES ./port/wm8524/fsl_codec_wm8524_adapter.h
                ./port/wm8524/fsl_codec_wm8524_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/wm8524
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.sgtl_adapter)

    mcux_add_source(
        SOURCES ./port/sgtl5000/fsl_codec_sgtl_adapter.h
                ./port/sgtl5000/fsl_codec_sgtl_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/sgtl5000
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.da7212_adapter)

    mcux_add_source(
        SOURCES ./port/da7212/fsl_codec_da7212_adapter.h
                ./port/da7212/fsl_codec_da7212_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/da7212
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.ak4497_adapter)

    mcux_add_source(
        SOURCES ./port/ak4497/fsl_codec_ak4497_adapter.h
                ./port/ak4497/fsl_codec_ak4497_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/ak4497
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.tfa9xxx_adapter)

    mcux_add_source(
        SOURCES ./port/tfa9xxx/fsl_codec_tfa9xxx_adapter.h
                ./port/tfa9xxx/fsl_codec_tfa9xxx_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/tfa9xxx
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.tfa9xxx_adapter)

    mcux_add_source(
        SOURCES ./port/tfa9xxx/fsl_codec_tfa9xxx_adapter.h
                ./port/tfa9xxx/fsl_codec_tfa9xxx_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/tfa9xxx
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.cs42888_adapter)

    mcux_add_source(
        SOURCES ./port/cs42888/fsl_codec_cs42888_adapter.h
                ./port/cs42888/fsl_codec_cs42888_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/cs42888
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.cs42448_adapter)

    mcux_add_source(
        SOURCES ./port/cs42448/fsl_codec_cs42448_adapter.h
                ./port/cs42448/fsl_codec_cs42448_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/cs42448
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.tfa9896_adapter)

    mcux_add_source(
        SOURCES ./port/tfa9896/fsl_codec_tfa9896_adapter.h
                ./port/tfa9896/fsl_codec_tfa9896_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/tfa9896
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.pcm186x_adapter)

    mcux_add_source(
        SOURCES ./port/pcm186x/fsl_codec_pcm186x_adapter.h
                ./port/pcm186x/fsl_codec_pcm186x_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/pcm186x
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_component.pcm512x_adapter)

    mcux_add_source(
        SOURCES ./port/pcm512x/fsl_codec_pcm512x_adapter.h
                ./port/pcm512x/fsl_codec_pcm512x_adapter.c
                ./port/fsl_codec_adapter.h
    )

    mcux_add_include(
        INCLUDES ./port/pcm512x
        INCLUDES ./port
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.ak4497)
    mcux_add_configuration(
        CC "-DCODEC_AK4497_ENABLE"
    )

    mcux_add_source(
        SOURCES ./ak4497/fsl_ak4497.h
                ./ak4497/fsl_ak4497.c
    )

    mcux_add_include(
        INCLUDES ./ak4497
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.cs42448)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
        CC "-DCODEC_CS42448_ENABLE"
    )

    mcux_add_source(
        SOURCES ./cs42448/fsl_cs42448.h
                ./cs42448/fsl_cs42448.c
    )

    mcux_add_include(
        INCLUDES ./cs42448
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.cs42888)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
        CC "-DCODEC_CS42888_ENABLE"
    )

    mcux_add_source(
        SOURCES ./cs42888/fsl_cs42888.h
                ./cs42888/fsl_cs42888.c
    )

    mcux_add_include(
        INCLUDES ./cs42888
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.dialog7212)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
        CC "-DBOARD_USE_CODEC=1"
        CC "-DCODEC_DA7212_ENABLE"
    )

    mcux_add_source(
        SOURCES ./da7212/fsl_dialog7212.h
                ./da7212/fsl_dialog7212.c
    )

    mcux_add_include(
        INCLUDES ./da7212
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.pcm186x)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
        CC "-DBOARD_USE_CODEC=1"
        CC "-DCODEC_PCM186X_ENABLE"
    )

    mcux_add_source(
        SOURCES ./pcm186x/fsl_pcm186x.h
                ./pcm186x/fsl_pcm186x.c
    )

    mcux_add_include(
        INCLUDES ./pcm186x
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.pcm512x)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
        CC "-DBOARD_USE_CODEC=1"
        CC "-DCODEC_PCM512X_ENABLE"
    )

    mcux_add_source(
        SOURCES ./pcm512x/fsl_pcm512x.h
                ./pcm512x/fsl_pcm512x.c
    )

    mcux_add_include(
        INCLUDES ./pcm512x
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.sgtl5000)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
        CC "-DBOARD_USE_CODEC=1"
        CC "-DCODEC_SGTL5000_ENABLE"
    )

    mcux_add_source(
        SOURCES ./sgtl5000/fsl_sgtl5000.h
                ./sgtl5000/fsl_sgtl5000.c
    )

    mcux_add_include(
        INCLUDES ./sgtl5000
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.tfa9xxx)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
        CC "-DBOARD_USE_CODEC=1"
        CC "-DCODEC_TFA9XXX_ENABLE"
    )

    mcux_add_source(
        SOURCES ./tfa9xxx/tfa_config_TFA9892N1.h
                ./tfa9xxx/tfa_config_TFA9894N1.h
                ./tfa9xxx/tfa_config_TFA9894N2.h
                ./tfa9xxx/fsl_tfa9xxx.c
                ./tfa9xxx/fsl_tfa9xxx.h
                ./tfa9xxx/fsl_tfa9xxx_IMX.c
                ./tfa9xxx/README.md
                ./tfa9xxx/vas_tfa_drv/config.h
                ./tfa9xxx/vas_tfa_drv/tfa2_container.c
                ./tfa9xxx/vas_tfa_drv/tfa2_container.h
                ./tfa9xxx/vas_tfa_drv/tfa2_dev.c
                ./tfa9xxx/vas_tfa_drv/tfa2_dev.h
                ./tfa9xxx/vas_tfa_drv/tfa2_dsp_fw.h
                ./tfa9xxx/vas_tfa_drv/tfa2_haptic.c
                ./tfa9xxx/vas_tfa_drv/tfa2_haptic.h
                ./tfa9xxx/vas_tfa_drv/tfa2_init.c
                ./tfa9xxx/vas_tfa_drv/tfa9xxx_parameters.h
                ./tfa9xxx/vas_tfa_drv/tfa_container_crc32.c
                ./tfa9xxx/vas_tfa_drv/tfa_haptic_fw_defs.h
    )

    mcux_add_include(
        INCLUDES ./tfa9xxx
        INCLUDES ./tfa9xxx/vas_tfa_drv
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.tfa9xxx_hal)

    mcux_add_source(
        SOURCES ./tfa9xxx/tfa_device_hal.h
                ./tfa9xxx/tfa_device_hal.c
    )

    mcux_add_include(
        INCLUDES ./tfa9xxx
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.tfa9896)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
        CC "-DBOARD_USE_CODEC=1"
        CC "-DCODEC_TFA9896_ENABLE"
    )

    mcux_add_source(
        SOURCES ./tfa9896/fsl_tfa9896_buffer.h
                ./tfa9896/fsl_hal_registers.c
                ./tfa9896/fsl_tfa9896.c
                ./tfa9896/fsl_tfa9896.h
                ./tfa9896/MIMXRT595595-EVK_TFA9896_SW.pdf
    )

    mcux_add_include(
        INCLUDES ./tfa9896
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.wm8524)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
        CC "-DBOARD_USE_CODEC=1"
        CC "-DCODEC_WM8524_ENABLE"
    )

    mcux_add_source(
        SOURCES ./wm8524/fsl_wm8524.h
                ./wm8524/fsl_wm8524.c
    )

    mcux_add_include(
        INCLUDES ./wm8524
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.wm8904)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
        CC "-DBOARD_USE_CODEC=1"
        CC "-DCODEC_WM8904_ENABLE"
    )

    mcux_add_source(
        SOURCES ./wm8904/fsl_wm8904.h
                ./wm8904/fsl_wm8904.c
    )

    mcux_add_include(
        INCLUDES ./wm8904
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.wm8960)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
        CC "-DBOARD_USE_CODEC=1"
        CC "-DCODEC_WM8960_ENABLE"
    )

    mcux_add_source(
        SOURCES ./wm8960/fsl_wm8960.h
                ./wm8960/fsl_wm8960.c
    )

    mcux_add_include(
        INCLUDES ./wm8960
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.wm8962)
    mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
        CC "-DBOARD_USE_CODEC=1"
        CC "-DCODEC_WM8962_ENABLE"
    )

    mcux_add_source(
        SOURCES ./wm8962/fsl_wm8962.h
                ./wm8962/fsl_wm8962.c
    )

    mcux_add_include(
        INCLUDES ./wm8962
    )

endif()
