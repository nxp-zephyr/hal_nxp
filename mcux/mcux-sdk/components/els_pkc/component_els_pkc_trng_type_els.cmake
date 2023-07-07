#Description: Component els_pkc.trng.type_els; user_visible: False
include_guard(GLOBAL)
message("component_els_pkc_trng_type_els component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClTrng/src/mcuxClTrng_ELS.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(component_els_pkc_trng)
