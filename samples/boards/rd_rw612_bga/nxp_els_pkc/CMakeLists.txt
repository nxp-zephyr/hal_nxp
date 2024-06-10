# Copyright 2023-2024 NXP
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nxp_els_pkc)

target_include_directories(app
    PUBLIC
        ${APPLICATION_SOURCE_DIR}/src
)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
