#Copyright 2023 - 2024 NXP
#SPDX - License - Identifier : Apache - 2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nxp_mbedtls_psa_crypto_thread)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
zephyr_include_directories(${APPLICATION_SOURCE_DIR}/src/tls_config)
