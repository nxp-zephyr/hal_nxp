.. zephyr:code-sample:: nxp_els_pkc
   :name: els_pkc
   :relevant-api: els_pkc

   els-pkc example for els_pkc high level api usage.

nxp els_pkc Sample Application
#######################################

Overview
********

This is the els-pkc example, where els_pkc apis are being exercised.

The source code for this sample application can be found at:
:zephyr_file:`samples/crypto/nxp_els_pkc`.

This sample is available for platforms that support els_pkc.

Requirements
************

- Micro USB cable
- Supported platform
- Personal Computer

Building, Flashing and Running
******************************

.. zephyr-app-commands::
   :zephyr-app: samples/crypto/nxp_els_pkc
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

    ================
    ELS hash example
    ================
    SHA224 one shot:pass
    SHA256 one shot:pass
    SHA256 streaming example:pass
    SHA256 long message example:pass
    SHA384 one shot:pass
    SHA512 one shot:pass

    =================================
    ELS PKC asymmetric cipher example
    =================================
    PKC ECC keygen sign verify:pass
    PKC RSA no-verify:pass
    PKC RSA sign no-encode:pass
    PKC RSA-PSS sign SHA256:pass
    PKC RSA-PSS verify SHA256:pass
    PKC ECC Curve25519:pass
    TLS Master session keys:pass

    ======================
    ELS PKC common example
    ======================
    ELS get info:pass
    RNG PRNG random:pass
    Flow protection:pass
    Memory data invariant compare:pass
    Memory data invariant copy:pass
    Key component operations:pass
    ELS power down wake-up init:pass
    PKC power down wake-up init:pass

    ============================
    ELS symmetric cipher example
    ============================
    AES128-CBC encryption:pass
    AES128-ECB encryption:pass
    CMAC AES128:pass

    RESULT: All 24 test PASS!!
    ELS-PKC stand-alone examples end
