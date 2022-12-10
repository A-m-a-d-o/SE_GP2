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
#include "driverlib/systick.h"


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

int keyboard[4][4] = {
    {'1', '2', '3', 'F'},
    {'4', '5', '6', 'E'},
    {'7', '8', '9', 'D'},
    {'A', '0', 'B', 'C'}
};

volatile uint8_t key_flag = 0;
uint32_t time = 0;

char key_pressed(uint8_t col, uint8_t row)
{

    row /= 2;
    if( row == 4)
        row = 3;

    return keyboard[row][col];
}

char detect_key(void)
{
    uint8_t cols;
    char key = 0;

    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3, 0);

    for(cols = 0; cols < 4; cols++)    //Verifies every column for a key press
    {
        uint8_t reading = 0;

        GPIOPinWrite(GPIO_PORTD_BASE, pin[cols], pin[cols]);    //Writes 1 in the pin selected
        SysCtlDelay(2 * (SysCtlClockGet() / 3 / 1000000));      //Waits 2 us

        reading = GPIOPinRead(GPIO_PORTE_BASE,
        GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);       //Reads PORTE

        if (reading != 0)                                      // if a key is detected
        {
            key = key_pressed(cols, reading);
            return key;
        }


       GPIOPinWrite(GPIO_PORTD_BASE, pin[cols], 0);

    }
    return 0;
}

void PortEIntHandler(void)
{

   GPIOIntDisable(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);
   key_flag = 1;
}

void int_tick_ms_handler(void)
{
    time++;
}
int main(void)
{
 // Initialize GPIO's
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

  //Associate interrupt to function
    GPIOIntRegister(GPIO_PORTE_BASE, PortEIntHandler);

    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,
    GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);

    GPIOIntTypeSet(GPIO_PORTE_BASE,
    GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3, GPIO_HIGH_LEVEL);

    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,
    GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);
  // Enable GPIO interrupt
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);

  //Write 1 all keypad input's
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);

    //Config systick for ms
    SysTickIntRegister(int_tick_ms_handler);
    SysTickPeriodSet(SysCtlClockGet()/1000);

    SysTickIntEnable();

    // Loop forever.
    //
    while(1)
    {


      if (key_flag == 1)
      {
          char key;
          key_flag = 0;
          key = detect_key();
          // does lcd processing
          SysTickEnable();


      }

      if(time >= 5)
      {
          SysTickDisable();
          time = 0;
          GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);
          GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);
      }

    }
    return 0;

}
