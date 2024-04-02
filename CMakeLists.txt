#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: Apache-2.0
#
# This CMake file is picked by the Zephyr build system because it is defined
# as the module CMake entry point (see zephyr/module.yml).

add_subdirectory(drivers)

zephyr_include_directories(include)
