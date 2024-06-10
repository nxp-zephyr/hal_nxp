.. _nxp_mbedtls_psa_crypto:

NXP Mbedtls Psa Crypto Test Sample
##################################

Overview
********

This is the mbedtls3x example, where PSA Crypto apis are being exercised.

The source code for this sample application can be found at:
:zephyr_file:`samples/boards/rd_rw612_bga/nxp_mbedtls_psa_crypto`.

Requirements
************

- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer

Building, Flashing and Running
******************************

.. zephyr-app-commands::
   :zephyr-app: samples/boards/rd_rw612_bga/nxp_mbedtls_psa_crypto
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

    Mbedtls - PSA - Crypto - examples
    =================================
    cipher encrypt/decrypt AES CBC no padding: success!
    cipher encrypt/decrypt AES CBC PKCS7 multipart: success!
    cipher encrypt/decrypt AES CTR multipart: success!