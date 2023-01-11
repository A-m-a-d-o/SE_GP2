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

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/hibernate.h"
#include "i2c.h"
#include "pwm.h"
#include "keypad.h"
#include "math.h"

void seconds_to_string( uint32_t * in, char * time)
{

    uint32_t hours, minutes, seconds;

    hours =(uint32_t) in/3600;

    minutes = (uint32_t) (in/600)%60;

    seconds = (uint32_t) in%60;

    time[0] = (hours / 10) + '0';

    time[1] = (hours % 10) + '0';

    time[2] = (minutes / 10) + '0';

    time[3] = (minutes % 10) + '0';

    time[4] = (seconds / 10) + '0';

    time[5] = (seconds % 10);

}





int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    uint32_t hours, minutes, seconds;
    //uint32_t  in = 58851; //16:20:51

    char  in[7] =  {'1','6','2','0','5','1'};

    uint32_t time;

       time = ((in[0] - '0')*10 + (in[1] - '0'))*3600;

       time += ((in[2] - '0')*10 + (in[3] - '0'))*60;

       time += ((in[4] - '0')*10 + (in[5] - '0'));




 return 0;

}
