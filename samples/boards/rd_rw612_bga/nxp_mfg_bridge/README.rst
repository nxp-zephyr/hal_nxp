.. _rd_rw612_bga_nxp-mfg-bridge-sample:

RD-RW612-BGA MFG Bridge Sample Application
###############################

Overview
********

This is the UART Wi-Fi bridge example to demonstrate the Lab Tool support.

The source code for this sample application can be found at:
:zephyr_file:`samples/boards/rd_rw612_bga/nxp_mfg_bridge`.

Requirements
************

- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer

Building, Flashing and Running
******************************

.. zephyr-app-commands::
   :zephyr-app: samples/boards/rd_rw612_bga/nxp_mfg_bridge
   :board: rd_rw612_bga
   :goals: build flash
   :compact:

Flashing:

    For CPU1 and CPU2 image, please refer to modules/hal/nxp/mcux/components/fw_bin/readme.rst

.. code-block:: console

    # You can find "zephyr.bin" and "zephyr.elf" under the "build/zephyr/" path.
    $ cd build/zephyr
    # load cpu3 image, you can download build/zephyr/zephyr.bin to 0x08000000 with J-LINK Commander.
    $ loadbin C:xxx\build\zephyr.bin, 0x08000000

Running:

1. Connect the board with Windows PC.
2. Configure Labtool Setup.ini file with correct Baudrate and COM port of device.
3. Run command on Labtool.
   Example Labtool output:
       Name:           Dut labtool
       Version:        2.1.0.14
       Date:           Sep 18 2017 (12:16:01)

       Note:

       Name:           DutApiClass
       Interface:      EtherNet
       Version:        2.1.0.14
       Date:           Sep 18 2017 (12:15:40)

       Note:

       \\.\COM6
        DutIf_InitConnection: 0
       --------------------------------------------------------
                       RW610 (802.11a/g/b/n) TEST MENU
       --------------------------------------------------------
       Enter option: 88
       DLL Version : 2.1.0.14
       LabTool Version: 2.1.0.14
       FW Version:  16.80.207.01       Mfg Version: 2.0.0.63
       SOC:    0000    09
       BBP:    A4      00
       RF:     58      31
       OR Version:     2.3      Customer ID:   0
       Enter option:

Customization options
*********************
For RW610/612 board, before compiling the CPU3 image, there are three macros need to be configured to determine which fw to download.
  - CONFIG_SUPPORT_WIFI
  - CONFIG_SUPPORT_BLE
  - CONFIG_SUPPORT_BLE_15D4

By default: Enable wifi and ble (15d4 is not supported yet)
wifi+ble
  - CONFIG_SUPPORT_WIFI=y
  - CONFIG_SUPPORT_BLE=y
  - CONFIG_SUPPORT_BLE_15D4=n

Here are some example about fw be used:
wifi only
  - CONFIG_SUPPORT_WIFI=y
  - CONFIG_SUPPORT_BLE=n
  - CONFIG_SUPPORT_BLE_15D4=n
wifi+ble+15d4
  - CONFIG_SUPPORT_WIFI=y
  - CONFIG_SUPPORT_BLE=n
  - CONFIG_SUPPORT_BLE_15D4=y
wifi+15d4
  - CONFIG_SUPPORT_WIFI=y
  - CONFIG_SUPPORT_BLE=n
  - CONFIG_SUPPORT_BLE_15D4=y
