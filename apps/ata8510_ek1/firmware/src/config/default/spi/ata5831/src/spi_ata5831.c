/******************************************************************************
  SPI_ATA5831

  Company:
    Microchip Technology Inc.

  File Name:
    spi_ata5831.c

  Summary:
    ATA5831 SPI Command Set Implementation

  Description:
    The ATA5831 SPI Command Set provides an interface to control and interact
    with the external UHF transceiver.
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

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "definitions.h"
#include "configuration.h"
#include "spi/ata5831/spi_ata5831.h"
#include "spi/ata5831/src/spi_ata5831_local.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global objects
// *****************************************************************************
// *****************************************************************************

events_struct_t g_events;
static uint8_t g_tx_buf[UHF_SPI_BUFFER_LENGTH];
static uint8_t g_rx_buf[UHF_SPI_BUFFER_LENGTH];

// *****************************************************************************
// *****************************************************************************
// Section: SPI_ATA5831 Local Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: SPI_ATA5831 Global Functions
// *****************************************************************************
// *****************************************************************************

void delay_ms(uint32_t ms)
{
    SYSTICK_DelayMs(ms);
}

void delay_us(uint32_t us)
{
    SYSTICK_DelayUs(us);
}

void uhf_power_on(void)
{
    /* clear UHF NPWRON1 line to wake-up device */
    UHF_NPWRON1_CLEAR;
    /* clear and set NRES pin to initialize device */
    UHF_NRESET_CLEAR;
    delay_us(1000);
    UHF_NRESET_SET;
    delay_us(5000);
}

uint8_t uhf_spi_read_fill_level_rx_fifo(void)
{
    UHF_SPI_CS_ENABLE;
    
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);
    
    g_tx_buf[0] = UHF_SPI_CMD_READ_FILL_LEVEL_RX_FIFO;
    g_tx_buf[1] = g_tx_buf[2] = 0;

    UHF_SPI_TRANSFER(g_tx_buf,0x03, g_rx_buf, 0x03);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
    
    return g_rx_buf[2];
}

uint8_t uhf_spi_read_fill_level_tx_fifo(void)
{
    UHF_SPI_CS_ENABLE;
    
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);
    
    g_tx_buf[0] = UHF_SPI_CMD_READ_FILL_LEVEL_TX_FIFO;
    g_tx_buf[1] = g_tx_buf[2] = 0;

    UHF_SPI_TRANSFER(g_tx_buf, 0x03, g_rx_buf, 0x03);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);
   
    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
    
    return g_rx_buf[2];
}

uint8_t uhf_spi_read_fill_level_rssi_fifo(void)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);
    
    g_tx_buf[0] = UHF_SPI_CMD_READ_FILL_LEVEL_RSSI_FIFO;
    g_tx_buf[1] = g_tx_buf[2] = 0;

    UHF_SPI_TRANSFER(g_tx_buf, 0x03, g_rx_buf, 0x03);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);
    
    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];

    return g_rx_buf[2];
}

void uhf_spi_get_event_bytes(uint8_t *events)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_GET_EVENT_BYTES;
    memset(&g_tx_buf[1], 0x00, 0x03);

    UHF_SPI_TRANSFER(g_tx_buf, 0x04, g_rx_buf, 0x04);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);
    
    memcpy(events, g_rx_buf, 0x04);
}

void uhf_spi_read_rssi_fifo(uint8_t *data, uint8_t length)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_READ_RSSI_FIFO;
    g_tx_buf[1] = length;
    /* Dummy bytes; extra one dummy byte in addition to length of data */
    memset(&g_tx_buf[2], 0x00, length + 1);

    UHF_SPI_TRANSFER(g_tx_buf, length+3, g_rx_buf, length+3);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
    /* g_rx_buf[2] is a dummy byte */
    memcpy(data, &g_rx_buf[3], length);
}

void uhf_spi_read_rx_fifo(uint8_t *data, uint8_t length)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_READ_RX_FIFO;
    g_tx_buf[1] = length;
    /* Dummy bytes; extra one dummy byte in addition to length of data */
    memset(&g_tx_buf[2], 0x00, length + 1);

    UHF_SPI_TRANSFER(g_tx_buf, length+3, g_rx_buf, length+3);
    
    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
    /* g_rx_buf[2] is a dummy byte */
    memcpy(data, &g_rx_buf[3], length);    
}

void uhf_spi_write_sram_reg(uint16_t addr, uint8_t *data, uint8_t length)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_WRITE_SRAM;
    g_tx_buf[1] = length;
    /* High byte of address */
    g_tx_buf[2] = (addr >> 0x08) & 0xFF;
    /* Low byte of address */
    g_tx_buf[3] = (addr & 0xFF);
    memcpy(&g_tx_buf[4], data, length);

    UHF_SPI_TRANSFER(g_tx_buf, length+4, g_rx_buf, length+4);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
    /* All other are dummy bytes */
}

void uhf_spi_read_sram_reg(uint16_t addr, uint8_t *data, uint8_t length)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_READ_SRAM;
    g_tx_buf[1] = length;
    /* High byte of address */
    g_tx_buf[2] = (addr >> 0x08) & 0xFF;
    /* Low byte of address */
    g_tx_buf[3] = (addr & 0xFF);
    memset(&g_tx_buf[4], 0x00, length + 1);

    UHF_SPI_TRANSFER(g_tx_buf, length+5, g_rx_buf, length+5);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
    /* g_rx_buf[2...4] are dummy bytes */
    memcpy(data, &g_rx_buf[5], length);
}

void uhf_spi_write_eeprom(uint16_t addr, uint8_t data)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_WRITE_EEPROM;
    g_tx_buf[1] = (addr >> 0x08) & 0xFF;
    g_tx_buf[2] = (addr & 0xFF);
    g_tx_buf[3] = data;

    UHF_SPI_TRANSFER(g_tx_buf, 0x04, g_rx_buf, 0x04);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
}

void uhf_spi_write_eeprom_block(uint16_t addr, uint8_t *data, uint8_t len)
{
    uint8_t i = 0;
    uint8_t tmpLen;

    while(i < len)
    {
        uhf_spi_write_sram_reg(0x02E9, (uint8_t *)&addr + 1, 1);/* high addr */
        uhf_spi_write_sram_reg(0x02EA, (uint8_t *)&addr, 1);    /* low addr */
        if((i + 7) <= len)
        {
            tmpLen = 7;
            uhf_spi_write_sram_reg(0x02EB, &tmpLen, 1);         /* len */
            uhf_spi_write_sram_reg(0x02EC, &data[i], 7);        /* data */
        }
        else
        {
            tmpLen = len - i;
            uhf_spi_write_sram_reg(0x02EB, &tmpLen, 1);         /* len */
            uhf_spi_write_sram_reg(0x02EC, &data[i], len - i);  /* data */
        }
        i += tmpLen;
        addr += tmpLen;
        uhf_spi_trigger_eeprom_secure_write();
        delay_us(10000 * tmpLen);
    }
}

uint8_t uhf_spi_read_eeprom(uint16_t addr)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_READ_EEPROM;
    g_tx_buf[1] = (addr >> 0x08) & 0xFF;
    g_tx_buf[2] = (addr & 0xFF);
    /*/ Two dummy bytes */
    g_tx_buf[3] = 0x00;
    g_tx_buf[4] = 0x00;

    UHF_SPI_TRANSFER(g_tx_buf, 0x05, g_rx_buf, 0x05);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];

    return g_rx_buf[4];
}

void uhf_spi_write_tx_fifo(uint8_t *data, uint8_t length)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_WRITE_TX_FIFO;
    g_tx_buf[1] = length;
    memcpy(&g_tx_buf[2], data, length);

    UHF_SPI_TRANSFER(g_tx_buf, length+2, g_rx_buf, length+2);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];

    /* All other are dummy bytes */
}

void uhf_spi_write_tx_preamble_fifo(uint8_t *data, uint8_t length)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_WRITE_TX_PREAMBLE_FIFO;
    g_tx_buf[1] = length;
    memcpy(&g_tx_buf[2], data, length);

    UHF_SPI_TRANSFER(g_tx_buf, length+2, g_rx_buf, length+2);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];

    /* All other bytes are dummy */
}

void uhf_spi_set_system_mode(uint8_t system_mode_config, uint8_t service_channel_config)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);
    
    g_tx_buf[0] = UHF_SPI_CMD_SET_SYSTEM_MODE;
    g_tx_buf[1] = system_mode_config;
    g_tx_buf[2] = service_channel_config;
    
    UHF_SPI_TRANSFER(g_tx_buf, 0x03, g_rx_buf, 0x03);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);
    
    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
}

void uhf_spi_calibrate_and_check(uint8_t tune_check_config, uint8_t service_channel_config)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);
    
    g_tx_buf[0] = UHF_SPI_CMD_CALIBRATE_AND_CHECK;
    g_tx_buf[1] = tune_check_config;
    g_tx_buf[2] = service_channel_config;

    UHF_SPI_TRANSFER(g_tx_buf, 0x03, g_rx_buf, 0x03);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);
    
    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
}

void uhf_spi_patch_spi(uint8_t parameter)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);
    
    g_tx_buf[0] = UHF_SPI_CMD_PATCH_SPI;
    g_tx_buf[1] = parameter;

    UHF_SPI_TRANSFER(g_tx_buf, 0x02, g_rx_buf, 0x02);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
}

void uhf_spi_system_reset_ROM(void)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);
    
    g_tx_buf[0] = UHF_SPI_CMD_SYSTEM_RESET_ROM;
    g_tx_buf[1] = 0x00;
    
    UHF_SPI_TRANSFER(g_tx_buf, 0x02, g_rx_buf, 0x02);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);
}

uint8_t uhf_spi_get_version_ROM(void)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_GET_VERSION_ROM;
    g_tx_buf[1] = g_tx_buf[2] = 0;

    UHF_SPI_TRANSFER(g_tx_buf, 0x03, g_rx_buf, 0x03);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];

    return g_rx_buf[2];
}

uint8_t uhf_spi_get_version_flash(uint8_t flash_version[], uint8_t *customer)
{
    uint8_t version = 0x00;
    
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_GET_VERSION_FLASH;
    memset(&g_tx_buf[1], 0x00, 0x05);

    UHF_SPI_TRANSFER(g_tx_buf, 0x06, g_rx_buf, 0x06);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
    /* 2 bytes of version */
    version = g_rx_buf[2];
    flash_version[0] = g_rx_buf[3];
    flash_version[1] = g_rx_buf[4];
    *customer = g_rx_buf[5];
    
    return version;
}

void uhf_spi_customer_conf_cmd(void)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);
    
    g_tx_buf[0] = UHF_SPI_CMD_CUST_CONF;
    g_tx_buf[1] = 0x00;
    
    UHF_SPI_TRANSFER(g_tx_buf, 0x02, g_rx_buf, 0x02);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);
    
    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
}

void uhf_spi_system_reset(void)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);
    
    g_tx_buf[0] = UHF_SPI_CMD_SYSTEM_RESET;
    g_tx_buf[1] = 0x00;
    
    UHF_SPI_TRANSFER(g_tx_buf, 0x02, g_rx_buf, 0x02);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);
}

void uhf_spi_trigger_eeprom_secure_write(void)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_TRIG_EEPROM_SECURE_WRITE;
    /* Trigger Sequence bytes: 0xAA, 0xCC, 0xF0 */
    g_tx_buf[1] = 0xAA;
    g_tx_buf[2] = 0xCC;
    g_tx_buf[3] = 0xF0;

    UHF_SPI_TRANSFER(g_tx_buf, 0x04, g_rx_buf, 0x04);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);
    
    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
}

void uhf_spi_set_voltage_monitor(uint8_t reg_vmcsr)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_SET_VOLTAGE_MONITOR;
    g_tx_buf[1] = reg_vmcsr;

    UHF_SPI_TRANSFER(g_tx_buf, 0x02, g_rx_buf, 0x02);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);
    
    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
}

void uhf_spi_off_command(void)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_OFF;
    g_tx_buf[1] = 0;

    UHF_SPI_TRANSFER(g_tx_buf, 0x02, g_rx_buf, 0x02);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);
}

uint16_t uhf_spi_read_temperature_value(void)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_READ_TEMP_VALUE;
    /* 3 dummy bytes */
    memset(&g_tx_buf[1], 0x00, 0x03);

    UHF_SPI_TRANSFER(g_tx_buf, 0x04, g_rx_buf, 0x04);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];

    return (((uint16_t)g_rx_buf[2]) << 8) | (g_rx_buf[3]);
}

void uhf_spi_init_sram_service(uint8_t sram_service_nr, uint8_t eep_service_nr)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_INIT_SRAM_SERVICE;
    g_tx_buf[1] = sram_service_nr;
    g_tx_buf[2] = eep_service_nr;

    UHF_SPI_TRANSFER(g_tx_buf, 0x03, g_rx_buf, 0x03);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
}

void uhf_spi_start_rssi_meas(uint8_t service_channel_config)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);
    
    g_tx_buf[0] = UHF_SPI_CMD_START_RSSI_MEAS;
    g_tx_buf[1] = service_channel_config;

    UHF_SPI_TRANSFER(g_tx_buf, 0x02, g_rx_buf, 0x02);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);
    
    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
}

uint16_t uhf_spi_get_rssi_value(void)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_GET_RSSI_VALUE;
    /* 3 dummy bytes */
    memset(&g_tx_buf[1], 0x00, 0x03);

    UHF_SPI_TRANSFER(g_tx_buf, 0x04, g_rx_buf, 0x04);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];

    return (((uint16_t)g_rx_buf[2]) << 8) | (g_rx_buf[3]);
}

void uhf_spi_read_rx_fifo_byte_int(uint8_t *data, uint8_t length)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_READ_RX_BUFFER_BYTE_INT;
    /* parameter step of 0x01 to continue reading depending on length */
    memset(&g_tx_buf[1], 0x01, length - 1);
    g_tx_buf[length] = 0x00;
    g_tx_buf[length + 1] = 0x00;

    UHF_SPI_TRANSFER(g_tx_buf, length+2, g_rx_buf, length+2);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
    memcpy(data, &g_rx_buf[2], length);
}

void uhf_spi_read_rssi_fifo_byte_int(uint8_t *data, uint8_t length)
{
    UHF_SPI_CS_ENABLE;
    delay_us(UHF_SPI_T0);
    delay_us(UHF_SPI_T1);

    g_tx_buf[0] = UHF_SPI_CMD_READ_RSSI_BUFFER_BYTE_INT;
    /* parameter step of 0x01 to continue reading depending on length */
    memset(&g_tx_buf[1], 0x01, length - 1);
    g_tx_buf[length] = 0x00;
    g_tx_buf[length + 1] = 0x00;

    UHF_SPI_TRANSFER(g_tx_buf, length+2, g_rx_buf, length+2);

    delay_us(UHF_SPI_T4);
    UHF_SPI_CS_DISABLE;
    delay_us(UHF_SPI_T5);

    g_events.system = g_rx_buf[0];
    g_events.trx = g_rx_buf[1];
    memcpy(data, &g_rx_buf[2], length);
}
