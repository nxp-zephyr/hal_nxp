/**
 *  Copyright 2023-2024 NXP
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  @file  main.c
 *  @brief main file
 */

#include <zephyr/kernel.h>
#include <string.h>
#include <stdlib.h>
#include <mbedtls/platform.h>

/* Include psa-crypto-driver interface file*/
#if defined(PSA_CRYPTO_DRIVER_ELS_PKC)
#include "els_pkc_driver.h"
#include "threading_alt.h"
#endif /* PSA_CRYPTO_DRIVER_ELS_PKC */

/************************************************************************ ***
 * MACROS                                                                 
 ************************************************************************ **/
#define CHECK_STATUS(actual, expected)                                             \
    do                                                                             \
    {                                                                              \
        if ((actual) != (expected))                                                \
        {                                                                          \
            printf(                                                                \
                "\tassertion failed at %s:%d - "                                   \
                "actual:%d expected:%d\r\n",                                       \
                __FILE__, __LINE__, (psa_status_t)actual, (psa_status_t)expected); \
            goto exit;                                                             \
        }                                                                          \
    } while (0)

/***************************************************************************
 * Global variables
 **************************************************************************/
/* size of stack area used by each thread */
#define STACKSIZE KB(3)
K_THREAD_STACK_DEFINE(thread_one_stack_area, STACKSIZE);
K_THREAD_STACK_DEFINE(thread_two_stack_area, STACKSIZE);

/* Thread priority */
#define PRIORITY     k_thread_priority_get(k_current_get())
#define SLEEP_PERIOD K_SECONDS(1)

struct k_thread thread_one;
struct k_thread thread_two;

/***************************************************************************
 * Definitions
 **************************************************************************/
static psa_status_t cipher_operation(psa_cipher_operation_t *operation,
                                     const uint8_t *input,
                                     size_t input_size,
                                     size_t part_size,
                                     uint8_t *output,
                                     size_t output_size,
                                     size_t *output_len)
{
    psa_status_t status;
    size_t bytes_to_write = 0, bytes_written = 0, len = 0;

    *output_len = 0;
    while (bytes_written != input_size)
    {
        bytes_to_write = (input_size - bytes_written > part_size ? part_size : input_size - bytes_written);

        status = psa_cipher_update(operation, input + bytes_written, bytes_to_write, output + *output_len,
                                   output_size - *output_len, &len);
        CHECK_STATUS(status, PSA_SUCCESS);

        bytes_written += bytes_to_write;
        *output_len += len;
    }

    status = psa_cipher_finish(operation, output + *output_len, output_size - *output_len, &len);
    CHECK_STATUS(status, PSA_SUCCESS);
    *output_len += len;

exit:
    return status;
}

static psa_status_t cipher_encrypt(psa_key_id_t key,
                                   psa_algorithm_t alg,
                                   uint8_t *iv,
                                   size_t iv_size,
                                   const uint8_t *input,
                                   size_t input_size,
                                   size_t part_size,
                                   uint8_t *output,
                                   size_t output_size,
                                   size_t *output_len)
{
    psa_status_t status;
    psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;
    size_t iv_len                    = 0;

    memset(&operation, 0, sizeof(operation));
    status = psa_cipher_encrypt_setup(&operation, key, alg);
    CHECK_STATUS(status, PSA_SUCCESS);

    status = psa_cipher_generate_iv(&operation, iv, iv_size, &iv_len);
    CHECK_STATUS(status, PSA_SUCCESS);

    status = cipher_operation(&operation, input, input_size, part_size, output, output_size, output_len);
    CHECK_STATUS(status, PSA_SUCCESS);

exit:
    psa_cipher_abort(&operation);
    return status;
}

static psa_status_t cipher_decrypt(psa_key_id_t key,
                                   psa_algorithm_t alg,
                                   const uint8_t *iv,
                                   size_t iv_size,
                                   const uint8_t *input,
                                   size_t input_size,
                                   size_t part_size,
                                   uint8_t *output,
                                   size_t output_size,
                                   size_t *output_len)
{
    psa_status_t status;
    psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;

    memset(&operation, 0, sizeof(operation));
    status = psa_cipher_decrypt_setup(&operation, key, alg);
    CHECK_STATUS(status, PSA_SUCCESS);

    status = psa_cipher_set_iv(&operation, iv, iv_size);
    CHECK_STATUS(status, PSA_SUCCESS);

    status = cipher_operation(&operation, input, input_size, part_size, output, output_size, output_len);
    CHECK_STATUS(status, PSA_SUCCESS);

exit:
    psa_cipher_abort(&operation);
    return status;
}

static psa_status_t cipher_example_encrypt_decrypt_aes_cbc_nopad_1_block(void)
{
    enum
    {
        block_size = PSA_BLOCK_CIPHER_BLOCK_LENGTH(PSA_KEY_TYPE_AES),
        key_bits   = 256,
        part_size  = block_size,
    };
    const psa_algorithm_t alg = PSA_ALG_CBC_NO_PADDING;

    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t key                = 0;
    size_t output_len               = 0;
    uint8_t iv[block_size];
    uint8_t input[block_size];
    uint8_t encrypt[block_size];
    uint8_t decrypt[block_size];

    status = psa_generate_random(input, sizeof(input));
    CHECK_STATUS(status, PSA_SUCCESS);

    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attributes, key_bits);

    status = psa_generate_key(&attributes, &key);
    CHECK_STATUS(status, PSA_SUCCESS);

    status = cipher_encrypt(key, alg, iv, sizeof(iv), input, sizeof(input), part_size, encrypt, sizeof(encrypt),
                            &output_len);
    CHECK_STATUS(status, PSA_SUCCESS);

    status =
        cipher_decrypt(key, alg, iv, sizeof(iv), encrypt, output_len, part_size, decrypt, sizeof(decrypt), &output_len);
    CHECK_STATUS(status, PSA_SUCCESS);

    status = memcmp(input, decrypt, sizeof(input));
    CHECK_STATUS(status, PSA_SUCCESS);

exit:
    psa_destroy_key(key);
    return status;
}

static psa_status_t cipher_example_encrypt_decrypt_aes_cbc_pkcs7_multi(void)
{
    enum
    {
        block_size = PSA_BLOCK_CIPHER_BLOCK_LENGTH(PSA_KEY_TYPE_AES),
        key_bits   = 256,
        input_size = 100,
        part_size  = 10,
    };

    const psa_algorithm_t alg = PSA_ALG_CBC_PKCS7;

    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t key                = 0;
    size_t output_len               = 0;
    uint8_t iv[block_size], input[input_size], encrypt[input_size + block_size], decrypt[input_size + block_size];

    status = psa_generate_random(input, sizeof(input));
    CHECK_STATUS(status, PSA_SUCCESS);

    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attributes, key_bits);

    status = psa_generate_key(&attributes, &key);
    CHECK_STATUS(status, PSA_SUCCESS);

    status = cipher_encrypt(key, alg, iv, sizeof(iv), input, sizeof(input), part_size, encrypt, sizeof(encrypt),
                            &output_len);
    CHECK_STATUS(status, PSA_SUCCESS);

    status =
        cipher_decrypt(key, alg, iv, sizeof(iv), encrypt, output_len, part_size, decrypt, sizeof(decrypt), &output_len);
    CHECK_STATUS(status, PSA_SUCCESS);

    status = memcmp(input, decrypt, sizeof(input));
    CHECK_STATUS(status, PSA_SUCCESS);

exit:
    psa_destroy_key(key);
    return status;
}

static psa_status_t cipher_example_encrypt_decrypt_aes_ctr_multi(void)
{
    enum
    {
        block_size = PSA_BLOCK_CIPHER_BLOCK_LENGTH(PSA_KEY_TYPE_AES),
        key_bits   = 256,
        input_size = 100,
        part_size  = 10,
    };
    const psa_algorithm_t alg = PSA_ALG_CTR;

    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t key                = 0;
    size_t output_len               = 0;
    uint8_t iv[block_size], input[input_size], encrypt[input_size], decrypt[input_size];

    status = psa_generate_random(input, sizeof(input));
    CHECK_STATUS(status, PSA_SUCCESS);

    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attributes, key_bits);

    status = psa_generate_key(&attributes, &key);
    CHECK_STATUS(status, PSA_SUCCESS);

    status = cipher_encrypt(key, alg, iv, sizeof(iv), input, sizeof(input), part_size, encrypt, sizeof(encrypt),
                            &output_len);
    CHECK_STATUS(status, PSA_SUCCESS);

    status =
        cipher_decrypt(key, alg, iv, sizeof(iv), encrypt, output_len, part_size, decrypt, sizeof(decrypt), &output_len);
    CHECK_STATUS(status, PSA_SUCCESS);

    status = memcmp(input, decrypt, sizeof(input));
    CHECK_STATUS(status, PSA_SUCCESS);

exit:
    psa_destroy_key(key);
    return status;
}

static psa_status_t cipher_examples(void)
{
    psa_status_t status;

    status = cipher_example_encrypt_decrypt_aes_cbc_nopad_1_block();
    CHECK_STATUS(status, PSA_SUCCESS);

    status = cipher_example_encrypt_decrypt_aes_cbc_pkcs7_multi();
    CHECK_STATUS(status, PSA_SUCCESS);

    status = cipher_example_encrypt_decrypt_aes_ctr_multi();
    CHECK_STATUS(status, PSA_SUCCESS);

exit:
    return status;
}

static psa_status_t thread_one_func(void)
{
    psa_status_t status;
    int count = 0;

    while (1)
    {
        status = psa_crypto_init();
        if (status != PSA_SUCCESS)
        {
            printf("Thread 1 failed init.");
            return status;
        }

        status = cipher_examples();
        if (status != PSA_SUCCESS)
        {
            printf("Thread 1 failed cipher");
            return status;
        }

        count++;
        if (count % 200 == 0)
        {
            printf("Thread 1 is running.\n");
        }
    }
}

static psa_status_t thread_two_func(void)
{
    psa_status_t status;
    int count = 0;

    while (1)
    {
        status = psa_crypto_init();
        if (status != PSA_SUCCESS)
        {
            printf("Thread 2 failed init.");
            return status;
        }
        status = cipher_examples();
        if (status != PSA_SUCCESS)
        {
            printf("Thread 2 failed cipher.");
            return status;
        }

        count++;
        if (count % 500 == 0)
        {
            printf("Thread 2 is running.\n");
        }
    }
}
/*************************************************************************
 * Prototypes
 *************************************************************************/

/*************************************************************************
 * Code
 *************************************************************************/

mbedtls_threading_mutex_t alloc_mutex;

static void *buffer_alloc_calloc_mutexed_custom(size_t n, size_t size)
{
    void *buf = NULL;
    if (mbedtls_alt_mutex_lock(&alloc_mutex) != 0)
    {
        return (NULL);
    }
    buf = calloc(n, size);
    if (mbedtls_alt_mutex_unlock(&alloc_mutex) != 0)
    {
        mbedtls_free(buf);
        return (NULL);
    }
    return (buf);
}

static void buffer_alloc_free_mutexed_custom(void *ptr)
{
    if (mbedtls_alt_mutex_lock(&alloc_mutex) != 0)
    {
        return;
    }
    free(ptr);
    if (mbedtls_alt_mutex_unlock(&alloc_mutex) != 0)
    {
        return;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    printf("\r\nMbedtls - PSA - Crypto - examples - Multihread\r\n");
    printf("============================\r\n");

    /* Initialize the mbedtls mutex and call mbedtls_threading_set_alt()
     as required by mbedTLS3x threading documentation*/
    config_mbedtls_threading_alt();

    mbedtls_alt_mutex_init(&alloc_mutex);
    mbedtls_platform_set_calloc_free(buffer_alloc_calloc_mutexed_custom, buffer_alloc_free_mutexed_custom);

    k_tid_t thread_one_id =
        k_thread_create(&thread_one, thread_one_stack_area, K_THREAD_STACK_SIZEOF(thread_one_stack_area),
                        (void *)thread_one_func, NULL, NULL, NULL, PRIORITY, 0, K_SECONDS(1));

    if (thread_one_id != NULL)
    {
        printf("Thread one was created successfully\n");
    }

    k_tid_t thread_two_id =
        k_thread_create(&thread_two, thread_two_stack_area, K_THREAD_STACK_SIZEOF(thread_two_stack_area),
                        (void *)thread_two_func, NULL, NULL, NULL, PRIORITY, 0, K_SECONDS(1));

    if (thread_two_id != NULL)
    {
        printf("Thread two was created successfully\n");
    }

    return 0;
}

