.. _nxp_mbedtls_psa_crypto:

NXP Mbedtls Psa Crypto Thread Test Sample
##################################

Overview
********

This is the mbedtls3x example, where thread safety of PSA Crypto apis is tested.

The source code for this sample application can be found at:
:zephyr_file:`samples/boards/rd_rw612_bga/nxp_mbedtls_psa_crypto_thread`.

Requirements
************

- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer

Building, Flashing and Running
******************************

.. zephyr-app-commands::
   :zephyr-app: samples/boards/rd_rw612_bga/nxp_mbedtls_psa_crypto_thead
   :board: rd_rw612_bga
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
