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
#include "i2c.h"



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





int main(void)
{
    float temp;
    int a;
    SysCtlClockSet(SYSCTL_SYSDIV_1|SYSCTL_USE_OSC|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    temp_sens_init();


    while(1)
    {

        temp_get();

        a++;
        return 0;
    }

}
