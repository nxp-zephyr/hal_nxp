/*
 * Copyright (c) 2023 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define MBEDTLS_PSA_CRYPTO_DRIVERS
#define PSA_CRYPTO_DRIVER_ELS_PKC

#define MBEDTLS_THREADING_C
#define MBEDTLS_THREADING_ALT

/* Entropy related configs */
#define MBEDTLS_ENTROPY_HARDWARE_ALT
#undef MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
