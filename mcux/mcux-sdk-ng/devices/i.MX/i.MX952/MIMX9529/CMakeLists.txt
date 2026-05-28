# Copyright 2024, 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

#### device spepcific drivers
mcux_add_cmakelists(${SdkRootDirPath}/${device_root}/i.MX/i.MX952/MIMX9529/drivers)
mcux_add_cmakelists(${SdkRootDirPath}/${device_root}/i.MX/i.MX952/MIMX9529/${core_id})

#### i.MX shared drivers/components/middlewares, project segments
include(${SdkRootDirPath}/devices/i.MX/shared.cmake)
