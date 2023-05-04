/*******************************************************************************
  SPI_ATA8510

  Company:
    Microchip Technology Inc.

  File Name:
    spi_ata8510_local.h

  Summary:
    ATA8510 Command Set Local Data Structures

  Description:
    Driver Local Data Structures for the external UHF transceiver.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2023, Microchip Technology Inc., and its subsidiaries. All rights reserved.
* 
* The software and documentation is provided by microchip and its contributors
* "as is" and any express, implied or statutory warranties, including, but not
* limited to, the implied warranties of merchantability, fitness for a particular
* purpose and non-infringement of third party intellectual property rights are
* disclaimed to the fullest extent permitted by law. In no event shall microchip
* or its contributors be liable for any direct, indirect, incidental, special,
* exemplary, or consequential damages (including, but not limited to, procurement
* of substitute goods or services; loss of use, data, or profits; or business
* interruption) however caused and on any theory of liability, whether in contract,
* strict liability, or tort (including negligence or otherwise) arising in any way
* out of the use of the software and documentation, even if advised of the
* possibility of such damage.
* 
* Except as expressly permitted hereunder and subject to the applicable license terms
* for any third-party software incorporated in the software and any applicable open
* source software license terms, no license or other rights, whether express or
* implied, are granted under any patent or other intellectual property rights of
* Microchip or any third party.
*******************************************************************************/
//DOM-IGNORE-END

#ifndef _SPI_ATA8510_LOCAL_H
#define _SPI_ATA8510_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

/* SPI timing constants */
#define UHF_SPI_T0 25   // us - Time from NSS LOW to AVR active (in case AVR sleep mode is enabled)
#define UHF_SPI_T1 18   // us - Time from AVR active to beginning of first telegram byte
#define UHF_SPI_T2 0    // us - Time to shift in one SPI-byte with SCK
#define UHF_SPI_T3 0    // us - Pause between two bytes. Can be any length including 0 us
#define UHF_SPI_T4 35   // us - Time for ATA8510 to handle last byte
#define UHF_SPI_T5 16   // us - SPI idle time telegram

/* SPI buffer length */
#define UHF_SPI_BUFFER_LENGTH 32

/* SPI command IDs */
#define UHF_SPI_CMD_READ_FILL_LEVEL_RX_FIFO     (0x01)
#define UHF_SPI_CMD_READ_FILL_LEVEL_TX_FIFO     (0x02)
#define UHF_SPI_CMD_READ_FILL_LEVEL_RSSI_FIFO   (0x03)
#define UHF_SPI_CMD_GET_EVENT_BYTES             (0x04)
#define UHF_SPI_CMD_READ_RSSI_FIFO              (0x05)
#define UHF_SPI_CMD_READ_RX_FIFO                (0x06)
#define UHF_SPI_CMD_WRITE_SRAM                  (0x07)
#define UHF_SPI_CMD_READ_SRAM                   (0x08)
#define UHF_SPI_CMD_WRITE_EEPROM                (0x09)
#define UHF_SPI_CMD_READ_EEPROM                 (0x0A)
#define UHF_SPI_CMD_WRITE_TX_FIFO               (0x0B)
#define UHF_SPI_CMD_WRITE_TX_PREAMBLE_FIFO      (0x0C)
#define UHF_SPI_CMD_SET_SYSTEM_MODE             (0x0D)
#define UHF_SPI_CMD_CALIBRATE_AND_CHECK         (0x0E)
#define UHF_SPI_CMD_PATCH_SPI                   (0x0F)
#define UHF_SPI_CMD_SYSTEM_RESET_ROM            (0x10)
#define UHF_SPI_CMD_GET_VERSION_ROM             (0x12)
#define UHF_SPI_CMD_GET_VERSION_FLASH           (0x13)
#define UHF_SPI_CMD_CUST_CONF                   (0x14)
#define UHF_SPI_CMD_SYSTEM_RESET                (0x15)
#define UHF_SPI_CMD_TRIG_EEPROM_SECURE_WRITE    (0x16)
#define UHF_SPI_CMD_SET_VOLTAGE_MONITOR         (0x17)
#define UHF_SPI_CMD_OFF                         (0x18)
#define UHF_SPI_CMD_READ_TEMP_VALUE             (0x19)
#define UHF_SPI_CMD_INIT_SRAM_SERVICE           (0x1A)
#define UHF_SPI_CMD_START_RSSI_MEAS             (0x1B)
#define UHF_SPI_CMD_GET_RSSI_VALUE              (0x1C)
#define UHF_SPI_CMD_READ_RX_BUFFER_BYTE_INT     (0x1D)
#define UHF_SPI_CMD_READ_RSSI_BUFFER_BYTE_INT   (0x1E)

#endif //#ifndef _SPI_ATA8510_LOCAL_H
