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
#include "driverlib/sysctl.h"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Blinky (blinky)</h1>
//!
//! A very simple example that blinks the on-board LED using direct register
//! access.
//
//*****************************************************************************

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

uint8_t pin[6] = {GPIO_PIN_0,GPIO_PIN_1, GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4, GPIO_PIN_5};





int key_pressed(void)
{
    return 0;
}
//*****************************************************************************
//
// Blink the on-board LED.
//
//*****************************************************************************
int main(void)
{
    volatile uint8_t rows, cols;

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));



    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,
    GPIO_PIN_0);

    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,
    GPIO_PIN_0);

    //
    // Loop forever.
    //
    while(1)
    {
        for(cols = 0; cols < 4; cols++)
        {
            GPIOPinWrite(GPIO_PORTD_BASE, pin[cols], pin[cols]);
            SysCtlDelay(2 * (SysCtlClockGet() / 3 / 1000000));
            for(rows = 0; rows < 4; rows++)  //Scan rows
                  {
                    if(GPIOPinRead(GPIO_PORTE_BASE, pin[rows]) == 1)
                    {
                      key_pressed();
                    }
                  }
            GPIOPinWrite(GPIO_PORTD_BASE, pin[cols], 0);
        }



    }
}
