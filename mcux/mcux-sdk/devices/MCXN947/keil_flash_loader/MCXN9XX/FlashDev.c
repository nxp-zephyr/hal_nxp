/* -----------------------------------------------------------------------------
 * Copyright (c) 2004 - 2016 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * $Date:        26. August 2016
 * $Revision:    V1.00
 *
 * Project:      Flash Device Description for NXP LPC55XX Flash using IAP
 * --------------------------------------------------------------------------- */

#include "FlashOS.h" // FlashOS Structures

#ifdef MCXN9XX

#ifdef FLASH_2048
struct FlashDevice const FlashDevice = {FLASH_DRV_VERS,             // Driver Version, do not modify!
                                        "MCXN9XX IAP 2048kB Flash", // Device Name
                                        ONCHIP,                     // Device Type
                                        0x00000000,                 // Device Start Address
                                        0x200000,                   // Device Size (2028kB)
                                        512,                        // Programming Page Size
                                        0,                          // Reserved, must be 0
                                        0xFF,                       // Initial Content of Erased Memory
                                        300,                        // Program Page Timeout 300 mSec
                                        3000,                       // Erase Sector Timeout 3000 mSec

                                        // Specify Size and Address of Sectors
                                        0x2000, 0x000000, // erase Sector Size 8192B
                                        SECTOR_END};
#endif

#ifdef FLASH_2048_S
struct FlashDevice const FlashDevice = {FLASH_DRV_VERS,               // Driver Version, do not modify!
                                        "MCXN9XX S IAP 2048kB Flash", // Device Name
                                        ONCHIP,                       // Device Type
                                        0x10000000,                   // Device Start Address
                                        0x200000,                     // Device Size (2028kB)
                                        512,                          // Programming Page Size
                                        0,                            // Reserved, must be 0
                                        0xFF,                         // Initial Content of Erased Memory
                                        300,                          // Program Page Timeout 300 mSec
                                        3000,                         // Erase Sector Timeout 3000 mSec

                                        // Specify Size and Address of Sectors
                                        0x2000, 0x000000, // erase Sector Size 8192B
                                        SECTOR_END};
#endif

#ifdef FLASH_1024
struct FlashDevice const FlashDevice = {FLASH_DRV_VERS,             // Driver Version, do not modify!
                                        "MCXN9XX IAP 1024kB Flash", // Device Name
                                        ONCHIP,                     // Device Type
                                        0x00000000,                 // Device Start Address
                                        0x100000,                   // Device Size (1024kB)
                                        512,                        // Programming Page Size
                                        0,                          // Reserved, must be 0
                                        0xFF,                       // Initial Content of Erased Memory
                                        300,                        // Program Page Timeout 300 mSec
                                        3000,                       // Erase Sector Timeout 3000 mSec

                                        // Specify Size and Address of Sectors
                                        0x2000, 0x000000, // erase Sector Size 8192B
                                        SECTOR_END};
#endif

#ifdef FLASH_1024_S
struct FlashDevice const FlashDevice = {FLASH_DRV_VERS,               // Driver Version, do not modify!
                                        "MCXN9XX S IAP 1024kB Flash", // Device Name
                                        ONCHIP,                       // Device Type
                                        0x10000000,                   // Device Start Address
                                        0x100000,                     // Device Size (1024kB)
                                        512,                          // Programming Page Size
                                        0,                            // Reserved, must be 0
                                        0xFF,                         // Initial Content of Erased Memory
                                        300,                          // Program Page Timeout 300 mSec
                                        3000,                         // Erase Sector Timeout 3000 mSec

                                        // Specify Size and Address of Sectors
                                        0x2000, 0x000000, // erase Sector Size 8192B
                                        SECTOR_END};
#endif

#ifdef FLASH_768
struct FlashDevice const FlashDevice = {FLASH_DRV_VERS,            // Driver Version, do not modify!
                                        "MCXN9XX IAP 768kB Flash", // Device Name
                                        ONCHIP,                    // Device Type
                                        0x00000000,                // Device Start Address
                                        0xC0000,                   // Device Size (768kB)
                                        512,                       // Programming Page Size
                                        0,                         // Reserved, must be 0
                                        0xFF,                      // Initial Content of Erased Memory
                                        300,                       // Program Page Timeout 300 mSec
                                        3000,                      // Erase Sector Timeout 3000 mSec

                                        // Specify Size and Address of Sectors
                                        0x2000, 0x000000, // erase Sector Size 8192B
                                        SECTOR_END};
#endif

#ifdef FLASH_768_S
struct FlashDevice const FlashDevice = {FLASH_DRV_VERS,              // Driver Version, do not modify!
                                        "MCXN9XX S IAP 768kB Flash", // Device Name
                                        ONCHIP,                      // Device Type
                                        0x10000000,                  // Device Start Address
                                        0xC0000,                     // Device Size (768kB)
                                        512,                         // Programming Page Size
                                        0,                           // Reserved, must be 0
                                        0xFF,                        // Initial Content of Erased Memory
                                        300,                         // Program Page Timeout 300 mSec
                                        3000,                        // Erase Sector Timeout 3000 mSec

                                        // Specify Size and Address of Sectors
                                        0x2000, 0x000000, // erase Sector Size 8192B
                                        SECTOR_END};
#endif

#ifdef FLASH_512
struct FlashDevice const FlashDevice = {FLASH_DRV_VERS,            // Driver Version, do not modify!
                                        "MCXN9XX IAP 512kB Flash", // Device Name
                                        ONCHIP,                    // Device Type
                                        0x00000000,                // Device Start Address
                                        0x80000,                   // Device Size (512kB)
                                        512,                       // Programming Page Size
                                        0,                         // Reserved, must be 0
                                        0xFF,                      // Initial Content of Erased Memory
                                        300,                       // Program Page Timeout 300 mSec
                                        3000,                      // Erase Sector Timeout 3000 mSec

                                        // Specify Size and Address of Sectors
                                        0x2000, 0x000000, // erase Sector Size 8192B
                                        SECTOR_END};
#endif

#ifdef FLASH_512_S
struct FlashDevice const FlashDevice = {FLASH_DRV_VERS,              // Driver Version, do not modify!
                                        "MCXN9XX S IAP 512kB Flash", // Device Name
                                        ONCHIP,                      // Device Type
                                        0x10000000,                  // Device Start Address
                                        0x80000,                     // Device Size (512kB)
                                        512,                         // Programming Page Size
                                        0,                           // Reserved, must be 0
                                        0xFF,                        // Initial Content of Erased Memory
                                        300,                         // Program Page Timeout 300 mSec
                                        3000,                        // Erase Sector Timeout 3000 mSec

                                        // Specify Size and Address of Sectors
                                        0x2000, 0x000000, // erase Sector Size 8192B
                                        SECTOR_END};
#endif

#endif // LPC55XX
