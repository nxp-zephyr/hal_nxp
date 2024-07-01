.. _nxp_mbedtls_psa_crypto:
.. zephyr:code-sample:: nxp_mbedtls_psa_crypto
   :name: mbedtls_psa_crypto
   :relevant-api: psa-crypto-apis

   psa-crypto-driver example for psa-api usage

NXP Mbedtls Psa Crypto Test Sample
##################################

Overview
********

This is the nxp-psa-crypto-driver example, where PSA Crypto apis are being exercised.

The source code for this sample application can be found at:
:zephyr_file:`samples/crypto/nxp_mbedtls_psa_crypto`.
This sample is available for platforms that support els_pkc and psa-crypto-driver.

Requirements
************

- Micro USB cable
- Supported platform
- Personal Computer

Building, Flashing and Running
******************************

.. zephyr-app-commands::
   :zephyr-app: samples/boards/rd_rw612_bga/nxp_mbedtls_psa_crypto
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

    Mbedtls - PSA - Crypto - examples
    =================================
    cipher encrypt/decrypt AES CBC no padding: success!
    cipher encrypt/decrypt AES CBC PKCS7 multipart: success!
    cipher encrypt/decrypt AES CTR multipart: success!