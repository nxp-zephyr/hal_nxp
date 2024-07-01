.. zephyr:code-sample:: nxp_mbedtls_psa_crypto_thread
   :name: mbedtls_psa_crypto_thread
   :relevant-api: psa-apis

   mbedtls_psa_crypto_thread example for psa-api usage in threaded environment.

NXP Mbedtls Psa Crypto Thread Test Sample
##################################

Overview
********

This is the mbedtls3x example, where thread safety of PSA Crypto apis is tested.

The source code for this sample application can be found at:
:zephyr_file:`samples/crypto/nxp_mbedtls_psa_crypto_thread`.
This sample is available for platforms that support els_pkc and psa-crypto-driver.

Requirements
************

- Micro USB cable
- Supported platform
- Personal Computer

Building, Flashing and Running
******************************

.. zephyr-app-commands::
   :zephyr-app: samples/boards/rd_rw612_bga/nxp_mbedtls_psa_crypto_thead
   :board: rd_rw612_bga, frdm_rw612
   :goals: build flash
   :compact:


Flashing
********

.. code-block:: console

    # You can find "zephyr.bin" and "zephyr.elf" under the "build/zephyr/" path.
    $ cd build/zephyr
    $ loadbin C:xxx\build\zephyr.bin, 0x08000000

Sample Output
=============

.. code-block:: console


Mbedtls - PSA - Crypto - examples - Multihread
============================

Thread one was created successfully

Thread two was created successfully
Thread 1 is running.
Thread 2 is running.
Thread 1 is running.
Thread 2 is running.
Thread 1 is running.
Thread 1 is running.
Thread 2 is running.
