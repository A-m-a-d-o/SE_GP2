/*
 * LCD.c
 *
 *  Created on: 12/12/2022
 *      Author: mac
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



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

#define LCD_DELAY_C     6
#define LCD_DELAY_C_ON 0.002
portTickType xLastWakeTime_LCD;
void LCD_Command(unsigned char c) {

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xf0) );
        GPIOPinWrite(LCDCONTROL, RS, 0x00);
        GPIOPinWrite(LCDCONTROL, E, E);

        vTaskDelayUntil(&xLastWakeTime_LCD, LCD_DELAY_C/ portTICK_RATE_MS);

        GPIOPinWrite(LCDCONTROL, E, 0x00);

        vTaskDelayUntil(&xLastWakeTime_LCD, LCD_DELAY_C/ portTICK_RATE_MS);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0x0f) << 4 );
        GPIOPinWrite(LCDCONTROL, RS, 0x00);
        GPIOPinWrite(LCDCONTROL, E, E);

        vTaskDelayUntil(&xLastWakeTime_LCD, LCD_DELAY_C_ON/ portTICK_RATE_MS);

        GPIOPinWrite(LCDCONTROL, E, 0x00);

        vTaskDelayUntil(&xLastWakeTime_LCD, LCD_DELAY_C/ portTICK_RATE_MS);

}

void LCD_Clear(void){

        LCD_Command(0x01);
        vTaskDelayUntil(&xLastWakeTime_LCD, LCD_DELAY_C_ON/ portTICK_RATE_MS);

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




}

void LCD_Pulse()
{
    GPIOPinWrite(LCDCONTROL , E, E);
    vTaskDelayUntil(&xLastWakeTime_LCD, LCD_DELAY_C_ON/ portTICK_RATE_MS);
    GPIOPinWrite(LCDCONTROL, E, 0x00);
}



void LCD_Write_c(unsigned char d) {

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0xf0) );
        GPIOPinWrite(LCDCONTROL , RS, RS);
        LCD_Pulse();
        vTaskDelayUntil(&xLastWakeTime_LCD, LCD_DELAY_C/ portTICK_RATE_MS);
        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0x0f) << 4 );
        GPIOPinWrite(LCDCONTROL , RS, RS);
        LCD_Pulse();
        vTaskDelayUntil(&xLastWakeTime_LCD, LCD_DELAY_C/ portTICK_RATE_MS);

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

