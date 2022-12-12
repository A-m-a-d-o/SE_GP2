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



void LCD_Command(unsigned char c) {

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xf0) );
        GPIOPinWrite(LCDCONTROL, RS, 0x00);
        GPIOPinWrite(LCDCONTROL, E, E);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDCONTROL, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0x0f) << 4 );
        GPIOPinWrite(LCDCONTROL, RS, 0x00);
        GPIOPinWrite(LCDCONTROL, E, E);

        SysCtlDelay(10);

        GPIOPinWrite(LCDCONTROL, E, 0x00);

        SysCtlDelay(50000);

}

void LCD_Clear(void){

        LCD_Command(0x01);
        SysCtlDelay(10);

}

void LCD_init() {

        SysCtlPeripheralEnable(LCDPORTENABLE);
        SysCtlPeripheralEnable(LCDCONTROLEN);
        GPIOPinTypeGPIOOutput(LCDPORT, D4| D5 | D6 | D7);
        GPIOPinTypeGPIOOutput(LCDCONTROL, RS | E);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDCONTROL, RS,  0x00 );

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
        GPIOPinWrite(LCDCONTROL, E, E);
        SysCtlDelay(10);
        GPIOPinWrite(LCDCONTROL, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
        GPIOPinWrite(LCDCONTROL, E, E);
        SysCtlDelay(10);
        GPIOPinWrite(LCDCONTROL, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
        GPIOPinWrite(LCDCONTROL, E, E);
        SysCtlDelay(10);
        GPIOPinWrite(LCDCONTROL, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x20 );
        GPIOPinWrite(LCDCONTROL, E, E);
        SysCtlDelay(10);
        GPIOPinWrite(LCDCONTROL, E, 0x00);

        SysCtlDelay(50000);


        LCD_Command(0x0F); //Turn on Lcd
        LCD_Clear(); //Clear screen

}

void LCD_Pulse()
{
    GPIOPinWrite(LCDCONTROL , E, E);
    SysCtlDelay(10);
    GPIOPinWrite(LCDCONTROL, E, 0x00);
}



void LCD_Write_c(unsigned char d) {

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0xf0) );
        GPIOPinWrite(LCDCONTROL , RS, RS);
        LCD_Pulse();
        SysCtlDelay(50000);
        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0x0f) << 4 );
        GPIOPinWrite(LCDCONTROL , RS, RS);
        LCD_Pulse();
        SysCtlDelay(50000);

}

void LCD_print(char * input)
{
   int size = strlen(input), i;

   for(i = 0; i < size; i++)
   {
       LCD_Write_c(input[i]);
   }

}

void LCD_Cursor(uint8_t col){


        LCD_Command(0x80 + (col % 20));
        return;

}


int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_8|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    LCD_init();
    LCD_print("O");
    LCD_Cursor(19);
    LCD_print("O");
    // Loop forever.
    //
    while(1)
    {
        int a;
        a = 0;

    }
    return 0;

}
