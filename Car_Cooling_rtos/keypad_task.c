

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <TMP_task.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "drivers/rgb.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "LCD/LCD.h"
#include "LCD_task.h"
#include "global_types.h"
#include "string_handler/string_handler.h"
#include "keypad/keypad.h"



//*****************************************************************************
//
// The stack size for the LED toggle task.
//
//*****************************************************************************
#define KEYPADTASKSTACKSIZE              128  // Stack size in words

//*****************************************************************************
//
// The item size and queue size for the LED message queue.
//
//*****************************************************************************

#define DEBOUNCE_DELAY      20


//*****************************************************************************
//
// The queue that holds messages sent to the LED task.
//
//*****************************************************************************


portTickType xLastWakeTime_Keypad;

extern xSemaphoreHandle keypad_semaphore;





//*****************************************************************************
//
// This task toggles the user selected LED at a user selected frequency. User
// can make the selections by pressing the left and right buttons.
//
//*****************************************************************************

static void
Keypad_Task(void *pvParameters)
{
    lcd_msg data_in;
    char key;

    xSemaphoreTake( keypad_semaphore, portMAX_DELAY );
    while(1)
    {


        xSemaphoreTake( keypad_semaphore, portMAX_DELAY ); //Keypad Interrupt detected (rising edge)


        vTaskDelay(DEBOUNCE_DELAY/ portTICK_RATE_MS); //Delay for Debounce


        if (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3) !=0 )
        {



           key = detect_key(); //Reads key pressed

            if(key != 'e')
            {



                if (xQueueSendToBack (Menu_queue, &key,  portMAX_DELAY ) != pdPASS )
                       {
                           while(1);
                       }


                GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);

                GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3); //Enables keypad interrupt

                while(1)
                {
                    xSemaphoreTake( keypad_semaphore,portMAX_DELAY  ); //Keypad Interrupt detected (falling edge)

                    vTaskDelay(DEBOUNCE_DELAY/ portTICK_RATE_MS); //Delay for Debounce

                    if (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3) == 0 ) //Verifies if key is no longer pressed
                       {
                           GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);   //Enables keypad interrupt
                           break;
                       }

                    else
                    {
                        GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);
                        GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);   //Enables keypad interrupt
                    }
                }
            }
            else
            {
                GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);
                GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);   //Enables keypad interrupt
            }
        }
        else
        {
            GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);
            GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3);   //Enables keypad interrupt
        }

    }




}

//*****************************************************************************
//
// Initializes the LED task.
//
//*****************************************************************************
uint32_t Keypad_TaskInit(void)
{
    //
    // Initialize the GPIOs and Timers that drive the three LEDs.
    //

    vSemaphoreCreateBinary( keypad_semaphore );

    keypad_init();



    if(xTaskCreate(Keypad_Task, (const portCHAR *)"KEYPAD", KEYPADTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_KEYPAD_TASK, NULL) != pdTRUE)
    {
        return(1);
    }


    //
    // Success.
    //
    return(0);
}
