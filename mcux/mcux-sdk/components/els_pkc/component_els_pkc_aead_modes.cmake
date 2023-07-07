#Description: Component els_pkc.aead_modes; user_visible: False
include_guard(GLOBAL)
message("component_els_pkc_aead_modes component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_ELS_AesCcm.c
    ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_ELS_AesGcm.c
    ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_ELS_CcmEngineAes.c
    ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_ELS_Constants.c
    ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_ELS_GcmEngineAes.c
    ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_ELS_MultiPart.c
    ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/src/mcuxClAeadModes_ELS_OneShot.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
    ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/inc
    ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClAeadModes/inc/internal
)


include(component_els_pkc_aead)
