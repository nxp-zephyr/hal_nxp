if(CONFIG_BT OR CONFIG_NET_L2_OPENTHREAD)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mcux-sdk-middleware-connectivity-framework)
    include(connectivity_framework)
endif()
