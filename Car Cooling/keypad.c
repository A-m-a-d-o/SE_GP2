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
#include "driverlib/systick.h"
#include "keypad.h"

const uint8_t pin[6] = {GPIO_PIN_0,GPIO_PIN_1, GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4, GPIO_PIN_5};
volatile char key = 0;
const char keyboard[4][4] = {
    {'1', '2', '3', 'F'},
    {'4', '5', '6', 'E'},
    {'7', '8', '9', 'D'},
    {'A', '0', 'B', 'C'}
};

volatile uint8_t key_flag = 0;
volatile uint8_t up=0;
uint32_t time = 0;

char key_pressed(uint8_t col, uint8_t row)
{

    row /= 2;
    if( row == 4)
        row = 3;

    return keyboard[row][col];
}


void keypad_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

  //Associate interrupt to function
    GPIOIntRegister(GPIO_PORTE_BASE, PortEIntHandler);

    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,
    GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);

    GPIOIntTypeSet(GPIO_PORTE_BASE,
    GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3, GPIO_BOTH_EDGES);

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

}

char detect_key(void)
{
    uint8_t cols;
    char key_ = 0;

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
            key_ = key_pressed(cols, reading);
            return key;
        }


       GPIOPinWrite(GPIO_PORTD_BASE, pin[cols], 0);

    }
    return 0;
}

void PortEIntHandler(void)
{
   GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);
   GPIOIntDisable(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);
   key_flag = 1;
}

void int_tick_ms_handler(void)
{
    time++;
}

void keypad_processing(void)
{

    if (key_flag == 1 && up == 0)
        {
            key_flag = 0;
            SysTickEnable();
            return;
        }

    else if (key_flag == 1 && up == 1)
        {
            key_flag = 0;
            SysTickEnable();
            return;

        }

    if(time >= 10 && up == 0)
        {
            SysTickDisable();
            time = 0;
            key = detect_key();
            up = 1;
            GPIOPinWrite(GPIO_PORTD_BASE,
                         GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);

            GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);
            return;
        }

    else if(time >= 10 && up == 1)
        {
            SysTickDisable();
            time = 0;

            if (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3) != 0 )
            {
                GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);
                return;
            }

            GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);
            up = 0;
            return;
        }


}
