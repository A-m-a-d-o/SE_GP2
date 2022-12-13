//*****************************************************************************
//
// blinky.c - Simple example to blink the on-board LED.
//
// Copyright (c) 2012-2020 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 2.2.0.295 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
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


//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

uint8_t pin[6] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4, GPIO_PIN_5};


void temp_sens_init(void)
{
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);

    GPIOPinTypeI2C(GPIO_PORTA_BASE,GPIO_PIN_6|GPIO_PIN_7);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0);
    //
    // Enable the I2C1 peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

    //Enable the GPIO peripheral for the alert
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    // Wait for the I2C1 module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1))
    {
    }

    // Wait for the I2C1 module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }
    //
    // Initialize Master and Slave
    //
    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);
    //
    // Specify slave address
    //
    I2CMasterSlaveAddrSet(I2C1_BASE, 0x90, false);

    I2CMasterDataPut(I2C1_BASE, 0x01);

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    while(I2CMasterBusBusy(I2C1_BASE))
    {
    }

    I2CMasterDataPut(I2C1_BASE, 0x2E);

    // Initiate send of character from Master to Slave
    //
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    //
    // Delay until transmission completes
    //
    while(I2CMasterBusBusy(I2C1_BASE))
    {
    }


}

uint16_t temp_read(void)
{
    uint16_t temp;
    I2CMasterSlaveAddrSet(I2C1_BASE, 0x90, false);

    I2CMasterDataPut(I2C1_BASE, 0x00);

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    while(I2CMasterBusBusy(I2C1_BASE))
    {
    }

    I2CMasterSlaveAddrSet(I2C1_BASE, 0x90, true);

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

   temp = (uint8_t) I2CMasterDataGet(I2C1_BASE);

   while(I2CMasterBusBusy(I2C1_BASE))
       {
       }

   I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

     temp = (uint8_t) I2CMasterDataGet(I2C1_BASE);

     while(I2CMasterBusBusy(I2C1_BASE))
         {
         }







}




int main(void)
{
    temp_sens_init();
    SysCtlClockSet(SYSCTL_SYSDIV_8|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // Loop forever.
    //
    while(1)
    {

    }
    return 0;

}
