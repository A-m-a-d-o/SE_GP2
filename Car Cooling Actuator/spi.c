
#include <msp430.h>
#include <stdint.h>


#define CS_ON P1OUT &= (~BIT5) // Select Device
#define CS_OFF P1OUT |= (BIT5) // Unselect Device
#define DUMMY 0xAA
#define MASK_REG_1 0x20
#define MASK_REG_2 0x24
#define NO_INT 0
#define ERROR_INT 1
#define WAKE_INT 2
#define TXB0_INT 3
#define TXB1_INT 4
#define TXB2_INT 5
#define RXB0_INT 6
#define RXB1_INT 7
#define SPI_CONFIG_8MHZ_100KBIT
typedef struct can_frame_
{
    uint16_t msg_id;
    uint8_t msg_lenght;
    uint8_t *msg_data;
}can_frame;
//******************************************************************************
// Device Initialization *******************************************************
//******************************************************************************

void initClockTo16MHz()
{
    if (CALBC1_16MHZ==0xFF)                  // If calibration constant erased
    {
        while(1);                               // do not load, trap CPU!!
    }
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_16MHZ;                    // Set DCO
    DCOCTL = CALDCO_16MHZ;
}

void initGPIO()
{

    P1OUT |= BIT5;
    P1DIR |= BIT5;
    //SPI Pins
    P1SEL = BIT1 + BIT2 + BIT4;
    P1SEL2 = BIT1 + BIT2 + BIT4;


}

void initSPI()
{
  //Clock Polarity: The inactive state is high
  //MSB First, 8-bit, Master, 3-pin mode, Synchronous
  UCA0CTL1 = UCSWRST;
  UCA0CTL0 |=  UCMSB +  UCMST + UCSYNC + UCCKPH;
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 |= 0x20;                          // /2
  UCA0BR1 = 0;                              //
  UCA0MCTL = 0;                             // No modulation must be cleared for SPI
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**


}

//******************************************************************************
// Main ************************************************************************
// Send and receive three messages containing the example commands *************
//******************************************************************************
uint8_t com_spi(uint8_t data)
{
    uint8_t received_ch;

    while (!(IFG2 & UCA0TXIFG)); // USCI_A0 TX buffer ready?
    UCA0TXBUF = data; // Send 0xAA over SPI to Slave
    while (!(IFG2 & UCA0RXIFG)); // USCI_A0 RX Received?
    received_ch = UCA0RXBUF; // Store received data

    return received_ch;
}
void spi_reset_int()
{
    P1OUT &= (~BIT5); // Select Device

    com_spi(0xC0); // Reset Intruction

    P1OUT |= (BIT5); // Unselect Device

}

uint8_t spi_write_registers(uint8_t reg, uint8_t* data, uint8_t num_bytes)
{
    uint8_t i;
    CS_ON;
    com_spi(0x02);
    com_spi(reg);
    for( i = 0; i < num_bytes; i++)
    {
       com_spi(data[i]);
    }

    CS_OFF;
    return 1;
}

void spi_read_registers(uint8_t reg, uint8_t* data_out, uint8_t num_bytes )
{
    uint8_t i;
    CS_ON;
    com_spi(0x03);
    com_spi(reg);

    for( i = 0; i < num_bytes; i++)
       {
          data_out[i] = com_spi(DUMMY);
       }
    CS_OFF;
}
void spi_write_mask(uint16_t id_mask, uint8_t reg)
{
    uint8_t mask_bytes[4];
    mask_bytes[0]= id_mask >>3;
    mask_bytes[1]= id_mask <<5;
    mask_bytes[2]= 0;
    mask_bytes[3]= 0;
    spi_reset_int();
    spi_write_registers(reg, mask_bytes, 4);
    return;

}
uint8_t get_status ()
{
    uint8_t data_in;
    spi_read_registers(0xE, &data_in, 1 );
    data_in &= 0xF1;
    data_in >>= 1;
   return data_in;
}


void spi_set_normal_mode(void)
{
    uint8_t aux=0;
    spi_write_registers(0x0F, &aux, 1);

}
void config_8m_100kb()
{
    uint8_t data_out[3] = {0x86, 0xB4, 0x01};
    spi_write_registers( 0x28, data_out, 3);
}
void spi_send_frame(can_frame* to_send, uint8_t reg)
{
    uint8_t data[14];
    uint8_t prep_id[4] = {0},aux;
    if(to_send->msg_lenght > 8)
        {
            return;
        }
    prep_id[0]= to_send->msg_id >>3;
    prep_id[1]= to_send->msg_id <<5;
    memcpy(data, prep_id, 4);
    data[4] = to_send->msg_lenght & 0xF;
    memcpy(&data[5], to_send->msg_data, to_send->msg_lenght);

    spi_write_registers(reg, data, 5 + to_send->msg_lenght);
    aux = 0xB;
    spi_write_registers(reg-1,&aux, 1 );
    spi_read_registers(reg-1, &aux , 1);

}
int main(void)
{
   can_frame can_msg;
   uint8_t msg_data_r = 0xAA;
   can_msg.msg_data = &msg_data_r;
   can_msg.msg_id = 0x01;
   can_msg.msg_lenght = 1;

   uint8_t aux = 0x64;
   uint8_t a =0;

  WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer



  initClockTo16MHz();
  initGPIO();
  initSPI();
  spi_reset_int();
  config_8m_100kb();
  spi_write_registers(0x60,&aux, 1);
  spi_set_normal_mode();

  spi_send_frame(&can_msg, 0x31);
  while (1)
  {
      a = get_status();
  }


}
