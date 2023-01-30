/*******************************************************************************
  SPI_ATA5831

  Company:
    Microchip Technology Inc.

  File Name:
    spi_ata5831.h

  Summary:
    ATA5831 SPI Command Set header

  Description:
    The ATA5831 SPI Command Set provides an interface to control and interact
    with the external UHF transceiver.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
//DOM-IGNORE-END

#ifndef _SPI_ATA5831_H
#define _SPI_ATA5831_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stdbool.h>
#include "driver/driver.h"
#include "system/system.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif

// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/* Event Bytes structure */
typedef struct events_struct_t {
    uint8_t system;
    uint8_t trx;
    uint8_t power;
    uint8_t config;
} events_struct_t;

// *****************************************************************************
// *****************************************************************************
// Section: SPI_ATA5831 Module Interface Routines
// *****************************************************************************
// *****************************************************************************

/* Function:
    void uhf_poweron(void)

  Summary:
    Power on UHF transceiver.

  Description:
    This function powers on / resets UHF transceiver using NRESET pin and
    NPWRON1 pin.

  Remarks:
    NRESET pin and NPWRON1 pin have to be configured.
*/
void uhf_power_on(void);

/* Function:
    uint8_t uhf_spi_read_fill_level_rx_fifo(void)

  Summary:
    Read Fill Level RX FIFO.

  Description:
    The SPI command "Read Fill Level RX FIFO" returns the fill level of the RX
    data FIFO.

  Remarks:
    Refer user guide for more information.
*/
uint8_t uhf_spi_read_fill_level_rx_fifo(void);

/* Function:
    uint8_t uhf_spi_read_fill_level_tx_fifo(void)

  Summary:
    Read Fill Level TX FIFO.

  Description:
    The SPI command "Read Fill Level TX FIFO" returns the fill level of the TX
    data FIFO.

  Remarks:
    Refer user guide for more information.
*/
uint8_t uhf_spi_read_fill_level_tx_fifo(void);

/* Function:
    uint8_t uhf_spi_read_fill_level_rssi_fifo(void)

  Summary:
    Read Fill Level RSSI FIFO.

  Description:
    The SPI command "Read Fill Level RSSI FIFO" returns the fill level of the
    support FIFO in RX direction.

  Remarks:
    Refer user guide for more information.
*/
uint8_t uhf_spi_read_fill_level_rssi_fifo(void);

/* Function:
    void uhf_spi_get_event_bytes(uint8_t *events)

  Summary:
    Get Event Bytes.

  Description:
    The SPI command "Get Event Bytes" returns the actual events.system,
    events.events, events.power and events.config bytes to the host. After the
    SPI command "Get Event Bytes", the event bytes events.system, event.events
    and events.power are cleared. The event byte events.config is not cleared as
    it contains the configuration of the currently loaded service/channel
    configuration.

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_get_event_bytes(uint8_t *events);

/* Function:
    void uhf_spi_read_rssi_fifo(uint8_t *data, uint8_t length)

  Summary:
    Read RSSI FIFO.

  Description:
    The SPI command "Read RSSI FIFO" reads n data bytes from the SFIFO in the RX
    direction. The desired number of data bytes to be returned is given in the
    length parameter. The access direction switch of the FIFO is set
    automatically and restored to the origin at the end of the command
    (direction temporarily stored in the T-bit).

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_read_rssi_fifo(uint8_t *data, uint8_t length);

/* Function:
    void uhf_spi_read_rx_fifo(uint8_t *data, uint8_t length)

  Summary:
    Read RX FIFO.

  Description:
    The SPI command "Read RX FIFO" reads n data bytes from the DFIFO in RX
    direction. The desired number of data bytes to be returned is given in the
    length parameter. The access direction switch of the FIFO is set
    automatically and restored to the origin at the end of the command
    (direction temporarily stored in the T-bit).

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_read_rx_fifo(uint8_t *data, uint8_t length);

/* Function:
    void uhf_spi_write_sram_reg(uint16_t addr, uint8_t *data, uint8_t length)

  Summary:
    Write SRAM/Register.

  Description:
    The SPI command "Write SRAM / Register" writes n (n = length) consecutive
    data bytes to the SRAM or hardware registers.

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_write_sram_reg(uint16_t addr, uint8_t *data, uint8_t length);

/* Function:
    void uhf_spi_read_sram_reg(uint16_t addr, uint8_t *data, uint8_t length)

  Summary:
    Read SRAM/Register.

  Description:
    The SPI command "Read SRAM / Register" reads n (n = length) consecutive data
    bytes from the SRAM or hardware registers, and sends them back to the host.

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_read_sram_reg(uint16_t addr, uint8_t *data, uint8_t length);

/* Function:
    void uhf_spi_write_eeprom(uint16_t addr, uint8_t data)

  Summary:
    Write EEPROM.

  Description:
    The SPI command "Write EEPROM" writes one data byte to the given EEPROM
    address.

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_write_eeprom(uint16_t addr, uint8_t data);

/* Function:
    void uhf_spi_write_eeprom_block(uint16_t addr, uint8_t *data, uint8_t len)

  Summary:
    Write EEPROM block.

  Description:
    The function writes n (n = len) consecutive data bytes to the given EEPROM
    address.

  Remarks:
    Use SPI command "Write EEPROM" and "Trigger EEPROM Secure Write".
*/
void uhf_spi_write_eeprom_block(uint16_t addr, uint8_t *data, uint8_t len);

/* Function:
    uint8_t uhf_spi_read_eeprom(uint16_t addr)

  Summary:
    Read EEPROM.

  Description:
    The SPI command "Read EEPROM" reads one data byte from the given EEPROM
    address.

  Remarks:
    Refer user guide for more information.
*/
uint8_t uhf_spi_read_eeprom(uint16_t addr);

/* Function:
    void uhf_spi_write_tx_fifo(uint8_t *data, uint8_t length)

  Summary:
    Write TX FIFO.

  Description:
    The SPI command "Write TX FIFO" writes n (n = length) data bytes into the TX
    data FIFO. The number of data bytes is given in the length parameter. The
    access direction of the DFIFO is switched automatically and restored to the
    origin value at the end of the command (direction temporarily stored in the
    T-bit).

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_write_tx_fifo(uint8_t *data, uint8_t length);

/* Function:
    void uhf_spi_write_tx_preamble_fifo(uint8_t *data, uint8_t length)

  Summary:
    Write TX Preamble FIFO.

  Description:
    The SPI command "Write TX Preamble FIFO" writes n (n = length) data bytes
    into the TX support FIFO. The number of data bytes is given in the length
    parameter. The access direction of the SFIFO is switched automatically and
    restored to the origin value at the end of the command (direction
    temporarily stored in the T-bit).

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_write_tx_preamble_fifo(uint8_t *data, uint8_t length);

/* Function:
    void uhf_spi_set_system_mode(uint8_t system_mode_config, uint8_t service_channel_config)

  Summary:
    Set System Mode.

  Description:
    The SPI command "Set System Mode" is used to switch the following modes:
    IDLEMode, TXMode, RXMode, PollingMode.

  Remarks:
    The IDLEMode is the only mode that allows reading/writing from/to the SRAM
    and registers.
    Refer user guide for more information.
*/
void uhf_spi_set_system_mode(uint8_t system_mode_config, uint8_t service_channel_config);

/* Function:
    void uhf_spi_calibrate_and_check(uint8_t tune_check_config, uint8_t service_channel_config)

  Summary:
    Calibrate and Check.

  Description:
    The SPI command "Calibrate and Check" is used to trigger one of the
    following features:
    Antenna tuning, Temperature measurement, SRC calibration, FRC calibration,
    VCO tuning, Self check.

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_calibrate_and_check(uint8_t tune_check_config, uint8_t service_channel_config);

/* Function:
    void uhf_spi_patch_spi(uint8_t parameter)

  Summary:
    Patch SPI.

  Description:
    The SPI command "Patch SPI" is used to extend the SPI command set by the
    customer as follows:
    Patch_Spi(uint8_t parameter) jumps to the function which is stored in
    extif.flashPtr. The parameter of Patch_Spi is located in the SPDR register.
    The Patch SPI extension has no return and the customer has to finish with a
    jump to PATCH_SPI_READY (which is the same as SPIBUFRDY_DISABLERXBUFINT).

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_patch_spi(uint8_t parameter);

/* Function:
    void uhf_spi_system_reset_ROM(void)

  Summary:
    System Reset ROM.

  Description:
    The SPI command "System Reset ROM" jumps directly to address 0x0000.

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_system_reset_ROM(void);

/* Function:
    uint8_t uhf_spi_get_version_ROM(void)

  Summary:
    Get Version ROM.

  Description:
    The SPI command "Get Version ROM" returns the ROM version to the host.

  Remarks:
    Refer user guide for more information.
*/
uint8_t uhf_spi_get_version_ROM(void);

/* Function:
    uint8_t uhf_spi_get_version_flash(uint8_t flash_version[], uint8_t *customer)

  Summary:
    Get Version Flash.

  Description:
    The SPI command "Get Version Flash" returns the following data bytes to the
    host:
    ROM version, Flash version high byte, Flash version low byte,
    Customer specific version Command Frame.

  Remarks:
    Refer user guide for more information.
*/
uint8_t uhf_spi_get_version_flash(uint8_t flash_version[], uint8_t *customer);

/* Function:
    void uhf_spi_customer_conf_cmd(void)

  Summary:
    Customer Configurable Command.

  Description:
    The SPI command "Customer Configurable Command" is used to extend the SPI
    command set by the customer as follows:
    "Customer Configurable Command" calls the function that is stored in
    customCmd.customPtr which points to an address in the Flash or user ROM.
    This command is not available in the ATA5833.

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_customer_conf_cmd(void);

/* Function:
    void uhf_spi_system_reset(void)

  Summary:
    System Reset.

  Description:
    The SPI command "System Reset" configures the watchdog with the minimum
    timeout period and waits for a watchdog reset in an endless loop.

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_system_reset(void);

/* Function:
    void uhf_spi_trigger_eeprom_secure_write(void)

  Summary:
    Trigger EEPROM Secure Write.

  Description:
    The SPI command "Trigger EEPROM Secure Write" starts a write process from
    some SRAM data to EEPROM. The command checks for the following pattern:
    0xAA 0xCC 0xF0
    If this pattern is sent correctly, the variable triggerEEPwr is set to 0xEE.
    Within the "ATA_check4extRequests_C" function, this variable is checked and,
    in case of matching the 0xEE, the function "ATA_eepromSecureWrite_C" is
    called.
    The data to write to the EEPROM is stored in the tmpAryApp. The first and
    second bytes of this array are the EEPROM address. The third byte is the
    length and from the fourth byte the data buffer is starting. The total size
    of the tmpAryApp is 10 bytes. This means that a maximum of seven data bytes
    are available for the secure EEPROM write.

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_trigger_eeprom_secure_write(void);

/* Function:
    void uhf_spi_set_voltage_monitor(uint8_t reg_vmcsr)

  Summary:
    Set Voltage Monitor.

  Description:
    This SPI command "Set Voltage Monitor" configures the voltage monitor in
    UHF transceiver. The voltage monitor level in the data byte is written to
    the voltage monitor control and status register (VMCSR).

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_set_voltage_monitor(uint8_t reg_vmcsr);

/* Function:
    void uhf_spi_off_command(void)

  Summary:
    OFF Command.

  Description:
    This SPI command "OFF Command" sets the UHF transceiver to OFFMode.

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_off_command(void);

/* Function:
    uint16_t uhf_spi_read_temperature_value(void)

  Summary:
    Read Temperature Value.

  Description:
    The SPI command "Read Temperature Value" returns the temperature value of
    the latest measurement which is stored in calib.sramTEMPH and
    calib.sramTEMPL.

  Remarks:
    Refer user guide for more information.
*/
uint16_t uhf_spi_read_temperature_value(void);

/* Function:
    void uhf_spi_init_sram_service(uint8_t sram_service_nr, uint8_t eep_service_nr)

  Summary:
    Init SRAM Service.

  Description:
    The SPI command "Init SRAM Service" initializes the service
    sramSerivce[sramServiceNr] with the service eepService[eepServiceNr].
    The init process is done within the main loop and takes approximately 200us.
    With this command the parameters sramServiceNr and eepServiceNr are stored
    and a trigger bit is set.
    These settings are stored in extReq.serviceInitConfig.

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_init_sram_service(uint8_t sram_service_nr, uint8_t eep_service_nr);

/* Function:
    void uhf_spi_start_rssi_meas(uint8_t service_channel_config)

  Summary:
    Start RSSI Measurement.

  Description:
    The SPI command "Start RSSI Measurement" starts the RSSI measurement of the
    service/channel configuration parameter of the transferred
    service_channel_config. The result is stored in the uint8_t rfRssiValue and
    can be read out with the SPI command "Get RSSI Value".

  Remarks:
    The RSSI measurement must be activated in the eepServices.rssiSysConf EEPROM
    variable of the corresponding service. Otherwise, incorrect values will be
    determined.
    Refer user guide for more information.
*/
void uhf_spi_start_rssi_meas(uint8_t service_channel_config);

/* Function:
    uint16_t uhf_spi_get_rssi_value(void)

  Summary:
    Get RSSI Value.

  Description:
    The SPI command "Get RSSI Value" returns the current RSSI average and peak
    value. To trigger an RSSI measurement, the SPI command "Start RSSI
    Measurement" can be used. The signal power of the watched 50 Ohm antenna
    input can be calculated by formula in user guide.

  Remarks:
    Refer user guide for more information.
*/
uint16_t uhf_spi_get_rssi_value(void);

/* Function:
    void uhf_spi_read_rx_fifo_byte_int(uint8_t *data, uint8_t length)

  Summary:
    Read Rx FIFO Byte Interrupt.

  Description:
    The SPI command "Read RX FIFO Byte Interrupt" returns n bytes of the content
    of the DFIFO in the RX direction. The desired number of data bytes to be
    returned is given by the step parameter. The access direction switch of the
    FIFO is set automatically and restored to the origin at the end of the
    command (direction temporarily stored in the T-bit).
    The SPI command "Read RX FIFO Byte Interrupt" can be used instead of the
    command "Read RX FIFO" to read the DFIFO content without using the SPI FIFO.
    The data bytes are read directly from the DFIFO. This can be helpful in
    timing critical situations where every single byte is fetched in a certain
    time slot. Due to the bypassing mechanism of the SPI FIFOs, this SPI command
    only works with data rates up to 250 Kbit/s.

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_read_rx_fifo_byte_int(uint8_t *data, uint8_t length);

/* Function:
    void uhf_spi_read_rssi_fifo_byte_int(uint8_t *data, uint8_t length)

  Summary:
    Read RSSI FIFO Byte Interrupt.

  Description:
    The SPI command "Read RSSI FIFO Byte Interrupt" returns n data bytes of the
    content of the SFIFO in the RX direction. The desired number of returned
    data bytes is given by the step parameter. The access direction switch of
    the FIFO is set automatically and restored to the origin at the end of the
    command (direction temporarily stored in the T-bit).
    The SPI command "Read RSSI FIFO Byte Interrupt" can be used instead of the
    command "Read RSSI FIFO" to read the SFIFO content without using the SPI
    FIFO. The data bytes are read directly from the SFIFO. This can be helpful
    in timing critical situations where every single byte is fetched in a
    certain time slot. Due to the bypassing mechanism of the SPI FIFOs, this SPI
    command only works with data rates up to 250 Kbit/s.

  Remarks:
    Refer user guide for more information.
*/
void uhf_spi_read_rssi_fifo_byte_int(uint8_t *data, uint8_t length);

extern events_struct_t g_events;

#ifdef __cplusplus
}
#endif

#endif // #ifndef _SPI_ATA5831_H
/*******************************************************************************
 End of File
*/
