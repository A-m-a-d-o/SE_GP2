/*
 * test.c
 *
 *  Created on: 14/12/2022
 *      Author: mac
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
#include "inc/hw_i2c.h"
#include "string.h"

#define FALSE 0
#define TRUE 1

#define LCDPORT         GPIO_PORTC_BASE
#define LCDPORTENABLE   SYSCTL_PERIPH_GPIOC

#define LCDCONTROLEN  SYSCTL_PERIPH_GPIOB
#define LCDCONTROL        GPIO_PORTB_BASE

#define RS              GPIO_PIN_3
#define E               GPIO_PIN_2

#define D4              GPIO_PIN_4
#define D5              GPIO_PIN_5
#define D6              GPIO_PIN_6
#define D7              GPIO_PIN_7



void I2CInit(void)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

  GPIOPinConfigure(GPIO_PA6_I2C1SCL);
  GPIOPinConfigure(GPIO_PA7_I2C1SDA);

  GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
  GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

  I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);
}

void test(void)
{
  I2CMasterSlaveAddrSet(I2C1_BASE, 0xAA, false);
  I2CMasterDataPut(I2C1_BASE, 0xAC);
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
  while(I2CMasterBusy(I2C1_BASE));

}

int main()
{

    I2CInit();
    test();
    return 0;

}
