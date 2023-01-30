/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project. The responsibility of 
    the function is as mentioned below:
    -> Receiving temperature data packets and displaying it over OLED.
    -> Addressing the button press.

  Author:
    Somesh Singh (I20952)
 
  Last date Modified:
    23-Nov-2022
 
 
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <string.h>
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include <oled/oled.h>
#include "definitions.h"                // SYS function prototypes

#define RF_SENSORCHANNEL    0x40
#define RF_NODATA           0x14
#define RF_LOWBATT          0x19
#define RF_TEMPDATA         0x64
#define RF_RSSIDATA         0x60

#define RF_TXMODE           0x31
#define RF_RXMODE           0x32
#define RF_POLLINGMODE      0x23
#define RF_TXSERVICE        0x40

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
struct rfstruct
{
    unsigned int version[2];
    unsigned char event[4];
    unsigned char rx_buffer[32];
    unsigned char tx_buffer[32];
    unsigned char rssi_buffer[32];
    unsigned char rx_len;
    unsigned char tx_len;
    unsigned char rssi_len;
};

unsigned int msg_count = 0;
unsigned int err_count = 0;
unsigned int tot_count = 0;

unsigned long dtim = 0L;
char string[150];
uint8_t rf_packets_received = 0;

// to perform easy number conversion for received sensor data
union {
    unsigned char   b[4];
    int             i[2];
    long            l;
} data;

// to keep track of the timer counter hit.
volatile long unsigned int _timer_counter;
struct rfstruct rf;

/***********************************************************************************************************************
* Function Name: cleaner()
* Description : clears the OLED screen and initialize string to NULL
* Arguments : none
* Return Value : none
***********************************************************************************************************************/
void cleaner(void){
    oled_clear();
    memset(string, '\0', sizeof(string));    
}


/***********************************************************************************************************************
* Function Name: TC0_cb_InterruptHandler()
* Description : Timer Counter 0 callback function
* Arguments : status: timer status and context: pointer
* Return Value : none
***********************************************************************************************************************/
static void TC0_cb_InterruptHandler(TC_TIMER_STATUS status, uintptr_t context)
{
    _timer_counter++;
}

/***********************************************************************************************************************
* Function Name: TC0_cb_InterruptHandler()
* Description : Timer Counter 2 callback function
* Arguments : status: timer status and context: pointer
* Return Value : none
***********************************************************************************************************************/
static void TC2_cb_InterruptHandler(TC_TIMER_STATUS status, uintptr_t context)
{
    static uint8_t cntr = 0;
    /*If RF Packet not received*/
    if((ATA5831_IRQ_Get() == true) && (rf_packets_received == 0))
    {
        if(cntr == 0)
        {
            sprintf(string,"ATA8510-EK1 Demo Kit \r\n(c)2022 Microchip V4.0\r\nwaiting for RF signal \r\n.....                     \r\n");
            oled_string(string, 0, 0);
        }
        else if(cntr == 1)
        {
            sprintf(string,"ATA8510-EK1 Demo Kit \r\n(c)2022 Microchip V4.0\r\nwaiting for RF signal \r\n                            \r\n");
            oled_string(string, 0, 0);
        }
        else
        {
            //Do Nothing
        }
        cntr++;
        if(cntr == 2)
        {
            cntr = 0;
        }
    }
}

/***********************************************************************************************************************
* Function Name: at_read_timer()
* Description : read SW timer counter value
* Arguments : none
* Return Value : counter value (10ms resolution)
***********************************************************************************************************************/
long unsigned int at_read_timer(void)
{
    long unsigned int tim = _timer_counter;

    _timer_counter = 0L;

    return tim;
}

/***********************************************************************************************************************
* Function Name: at_test_btn()
* Description : test if button is pressed
* Arguments : btn: select OLED_BTN1_PIN, OLED_BTN2_PIN or OLED_BTN3_PIN
* Return Value : TRUE=button pressed, FALSE=button not pressed
***********************************************************************************************************************/
uint8_t at_test_btn(uint8_t btn)
{
    uint8_t initial_state = 0, actual_state = 0;

    switch(btn)
    {
        case OLED_BTN1_PIN:initial_state = OLED_BTN1_Get();break;
        case OLED_BTN2_PIN:initial_state = OLED_BTN2_Get();break;
        case OLED_BTN3_PIN:initial_state = OLED_BTN3_Get();break;
        default:break;
    }
    if(initial_state != 0) return(false);

    // debounce switch
    do
    {
        delay_ms(2);
        switch(btn)
        {
            case OLED_BTN1_PIN:actual_state = OLED_BTN1_Get();break;
            case OLED_BTN2_PIN:actual_state = OLED_BTN2_Get();break;
            case OLED_BTN3_PIN:actual_state = OLED_BTN3_Get();break;
            default: break;
        }
    } while(actual_state != initial_state);

    return(true);
}

/***********************************************************************************************************************
* Function Name:    rf_ata5831_init()
* Description :     initialization of ATA5831 device and rf structure.
* Arguments :       none
* Return Value :    none
***********************************************************************************************************************/
void rf_ata5831_init(void)
{
    uint16_t i;
    uint8_t ver = 0;

    // initialise rf structure
    rf.version[0] = 0;
    rf.version[1] = 0;
    for(i = 0; i < 4; i++)
    {
        rf.event[i] = 0;
    }
    for(i = 0; i < 32; i++)
    {
        rf.rx_buffer[i] = 0;
        rf.tx_buffer[i] = 0;
        rf.rssi_buffer[i] = 0;
    }
    rf.rx_len = 0;
    rf.tx_len = 0;
    rf.rssi_len = 0;

    uhf_power_on();

    // read status to clear event
    uhf_spi_get_event_bytes(&rf.event[0]);

    // read ROM version
    ver = uhf_spi_get_version_ROM();
    rf.version[0] = ((ver & 0xF0) >> 4);
    rf.version[1] = (ver & 0x0F);

    return;
}

/***********************************************************************************************************************
* Function Name:    checksum()
* Description :     calculate simple checksum for data array.
* Arguments :       data[] data array
*                   len: no. of data array
* Return Value :    checksum
***********************************************************************************************************************/
uint8_t checksum(uint8_t data[], uint8_t len)
{
    uint8_t i, sum;

    sum = 0;
    for(i = 0; i < len; i++)
    {
        sum += data[i];
    }

    return (0xFF - sum + 1);
}

/***********************************************************************************************************************
* Function Name: main()
* Description : main function
* Arguments : none
* Return Value : exit code
***********************************************************************************************************************/
int main ( void )
{
    uint8_t timeout = 0;
    uint16_t rssi = 0;
    uint8_t dt = 0;
    uint8_t index = 0;
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    oled_init();
    /* Initialize ATA5831 transceiver */
    rf_ata5831_init();
    sprintf(string,"\rATA8510-EK1 Demo Kit \r\n(c)2022 Microchip V4.0\r\nwaiting for RF signal \r\n.....       \r\n");
    oled_string(string, 0, 0);
    SERCOM4_USART_Write(&string[0], sizeof(string));
    delay_ms(250);
    OLED_LED1_Set();
    delay_ms(250);
    OLED_LED2_Set();
    delay_ms(250);
    OLED_LED3_Set();

    // switch transceiver into polling mode
    uhf_spi_set_system_mode(RF_POLLINGMODE, 0x00);
    delay_us(200);

    TC0_TimerCallbackRegister(TC0_cb_InterruptHandler, (uintptr_t)NULL);
    TC2_TimerCallbackRegister(TC2_cb_InterruptHandler, (uintptr_t)NULL);
    /* Start the timer */
    TC0_TimerStart();
    TC2_TimerStart();

    //set current timer value = 0
    at_read_timer();

    while ( true )
    {
        if(ATA5831_IRQ_Get() == false)
        {
            /*To stop blink the RF wait dots */
            rf_packets_received = 1;
            // read and set current timer value = 0
            dtim = at_read_timer();

            // read status to clear event
            uhf_spi_get_event_bytes(&rf.event[0]);

            // if WCO, SOT and EOT is set for path A, channel 0 and service 0 evaluate ...
            if(((rf.event[1]&0x70) == 0x70) && (rf.event[3] == 0x40))
            {
                // read RX buffer
                rf.rx_len = uhf_spi_read_fill_level_rx_fifo();
                uhf_spi_read_rx_fifo(&rf.rx_buffer[0], rf.rx_len);
                // read RSSI buffer
                rf.rssi_len= uhf_spi_read_fill_level_rssi_fifo();
                uhf_spi_read_rssi_fifo(&rf.rssi_buffer[0], rf.rssi_len);
                // set idle mode to clear status
                uhf_spi_set_system_mode(0x00, 0x00);
                // evaluate data
                rssi = 0;
                if(rf.rssi_len > 5) rf.rssi_len = 5;
                for(uint8_t index = 0; index < rf.rssi_len; index++) rssi += rf.rssi_buffer[index];
                rssi /= rf.rssi_len;
                tot_count++;
                // check received bytes with checksum and set err receive flag if wrong data
                if(rf.rx_buffer[rf.rx_len - 1] != checksum(rf.rx_buffer, rf.rx_len - 1))
                    rf.rx_buffer[0] = RF_NODATA;

                // if valid temperature data ...
                if(rf.rx_buffer[0] == RF_TEMPDATA)
                {
                    //convert received sensor data
                    data.b[0] = rf.rx_buffer[1];
                    data.b[1] = rf.rx_buffer[2];
                    msg_count++;
                    
                    // prepare acknowledge packet
                    // set TX buffer and start tx for acknowledge
                    for (index=0; index < 6; index++)rf.tx_buffer[index] = 0xFF;
                    rf.tx_buffer[6] =0xFE;          // end of tx preamble
                    rf.tx_buffer[7] = RF_RSSIDATA;
                    rf.tx_buffer[8] = RF_RSSIDATA;  // for checksum
                    uhf_spi_write_tx_fifo(&rf.tx_buffer[0], 9);
                    rf.rssi_buffer[0] = 0;
                    uhf_spi_write_tx_preamble_fifo(&rf.rssi_buffer[0], 1);
                    uhf_spi_set_system_mode(RF_TXMODE, RF_TXSERVICE);
                    timeout=0;
                    do
                    {
                        timeout++;
                        delay_us(100);
                    } while((ATA5831_IRQ_Get() != 0) && (timeout < 300));
                    uhf_spi_get_event_bytes(&rf.event[0]);
                    // set idle mode to clear status 
                    uhf_spi_set_system_mode(0x00, 0x00);

                    if(timeout < 300)
                    {
                        //start receive mode 
                        uhf_spi_set_system_mode(RF_RXMODE, RF_TXSERVICE);
                        timeout=0;
                        do
                        {
                            timeout++;
                            delay_us(100);
                        } while((ATA5831_IRQ_Get() != 0) && (timeout < 400));
                        uhf_spi_get_event_bytes(&rf.event[0]);

                        if(timeout < 400)
                        {
                            // RF answer received 
                            // read RX buffer 
                            rf.rx_len = uhf_spi_read_fill_level_rx_fifo();
                            uhf_spi_read_rx_fifo(&rf.rx_buffer[0], rf.rx_len);
                            // read RSSI buffer 
                            rf.rssi_len= uhf_spi_read_fill_level_rssi_fifo();
                            uhf_spi_read_rssi_fifo(&rf.rssi_buffer[0], rf.rssi_len);

                            if (rf.rx_buffer[0] == RF_RSSIDATA)
                            {
                                if(dtim > 99900L) dtim = 99900L;
                                dt = (unsigned int) dtim / 100L;
                                // show receive string 
                                cleaner();
                                sprintf(string,"\r  dt=%3ds     rssi=%3d     \r\n                                \r\n          T=%3d'C            \r\n          RSSI=%3d         \r\n",
                                dt, rssi, data.i[0] / 10, rf.rx_buffer[2]);								
                                oled_string(string, 0, 0);
                                SERCOM4_USART_Write(&string[0], sizeof(string));
                            }
                            // if no sensor data available ... 
                            else if(rf.rx_buffer[0] == RF_NODATA)
                            {
                                cleaner();
                                sprintf(string, "!!!!!!!!!!!!!!!!!!!!!\r\n Sensor error:              \r\nInvalid sensor data!   \r\n!!!!!!!!!!!!!!!!!!!!!\r\n");
                                oled_string(string, 0, 0);
                                SERCOM4_USART_Write(&string[0], sizeof(string));
                                // increase error message counter 
                                err_count++;
                            }
                            // sensor has low battery voltage 
                            else if(rf.rx_buffer[0] == RF_LOWBATT)
                            {
                                cleaner();
                                sprintf(string,"!!!!!!!!!!!!!!!!!!!!!\r\n Sensor error:              \r\n Low battery voltage!   \r\n!!!!!!!!!!!!!!!!!!!!!\r\n");
                                oled_string(string, 0, 0);
                                SERCOM4_USART_Write(&string[0], sizeof(string));
                                // increase error message counter 
                                err_count++;
                            }
                            else
                            {
                                cleaner();
                                sprintf(string,":::::::::::::::::::::\r\n RF telegram error:   \r\n Wrong ACK telegram!  \r\n:::::::::::::::::::::\r\n");
                                oled_string(string, 0, 0);
                                SERCOM4_USART_Write(&string[0], sizeof(string));
                                // increase error message counter 
                                err_count++;
                            }
                        }
                        else
                        {
                            cleaner();
                            sprintf(string,"::::::::::::::::::::::\r\n  RF telegram error:  \r\n No RF ACK telegram!   \r\n:::::::::::::::::::::\r\n");
                            oled_string(string, 0, 0);
                            SERCOM4_USART_Write(&string[0], sizeof(string));
                            // increase error message counter 
                            err_count++;
                        }
                    }
                    else
                    {
                        cleaner();
                        sprintf(string,":::::::::::::::::::::\r\n RF channel error:     \r\n RF TX telegram err!   \r\n:::::::::::::::::::::\r\n");
                        oled_string(string, 0, 0);
                        SERCOM4_USART_Write(&string[0], sizeof(string));
                        // increase error message counter 
                        err_count++;
                    }
                }
                else
                {
                    cleaner();
                    sprintf(string,":::::::::::::::::::::\r\n RF channel error:     \r\n Wrong ACK telegram!     \r\n::::::::::::::::::::: \r\n");
                    oled_string(string, 0, 0);
                    SERCOM4_USART_Write(&string[0], sizeof(string));
                    // increase error message counter 
                    err_count++;
                }
            }
            // switch transceiver into idle mode 
            uhf_spi_set_system_mode(0x00, 0x00);
            // wait for 1ms after idle mode 
            delay_ms(1);
            // switch transceiver into polling mode 
            uhf_spi_set_system_mode(RF_POLLINGMODE, 0x00);
        }
        // check for button1 event 
        else if(at_test_btn(OLED_BTN1_PIN))
        {
            /*To stop blink the RF wait dots*/
            rf_packets_received = 1;
            // switch IO led1 on 
            OLED_LED1_Clear();
            OLED_LED2_Set();
            OLED_LED3_Set();
            cleaner();
            sprintf(string,"\rRF-Channel 433.92MHz \r\nData Rate 8kBit/s       \r\nFSK deviation +/-8kHz \r\nManchester Coding     \r\n");
            oled_string(string, 0, 0);
            SERCOM4_USART_Write(&string[0], sizeof(string));
            // check if button is released 
            while(at_test_btn(OLED_BTN1_PIN))
            {
                delay_ms(100);
            }
        }
        // check for button2 event
        else if(at_test_btn(OLED_BTN2_PIN))
        {
            /*To stop blink the RF wait dots */
            rf_packets_received = 1;
            // switch IO led2 on
            OLED_LED1_Set();
            OLED_LED2_Clear();
            OLED_LED3_Set();
            cleaner();
            sprintf(string,"\rCOM Port Settings:     \r\nbaudrate 38.4 kBaud    \r\n8 data + 1 stop bit     \r\nno parity, no handsh. \r\n");
            oled_string(string, 0, 0);
            SERCOM4_USART_Write(&string[0], sizeof(string));
            // check if button is released
            while(at_test_btn( OLED_BTN2_PIN ))
            {
                delay_ms(100);
            }
        }
        // check for button3 event
        else if(at_test_btn(OLED_BTN3_PIN))
        {
            /*To stop blink the RF wait dots */
            rf_packets_received = 1;
            // switch IO led3 on
            OLED_LED1_Set();
            OLED_LED2_Set();
            OLED_LED3_Clear();
            cleaner();
            sprintf(string,"\rReceiver statistics:  \r\nvalid# %10d    \r\nerror# %10d    \r\ntotal# %10d    \r\n",msg_count,err_count,tot_count);
            oled_string(string, 0, 0);
            SERCOM4_USART_Write(&string[0], sizeof(string));
            // check if button is released
            while(at_test_btn(OLED_BTN3_PIN))
            {
                delay_ms(100);
            }
        }

        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks();
    }

    /* Execution should not come here during normal operation */

    return(EXIT_FAILURE);
}


/*******************************************************************************
 End of File
*/

