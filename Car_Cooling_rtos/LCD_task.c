

#include <stdbool.h>
#include <stdint.h>
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



//*****************************************************************************
//
// The stack size for the LED toggle task.
//
//*****************************************************************************
#define LCDTASKSTACKSIZE        128         // Stack size in words

//*****************************************************************************
//
// The item size and queue size for the LED message queue.
//
//*****************************************************************************
#define LCD_ITEM_SIZE           sizeof(uint8_t)
#define LCD_QUEUE_SIZE          5
#define LCD_Delay               100


//*****************************************************************************
//
// The queue that holds messages sent to the LED task.
//
//*****************************************************************************
xQueueHandle g_pLEDQueue;

portTickType xLastWakeTime_LCD;





//*****************************************************************************
//
// This task toggles the user selected LED at a user selected frequency. User
// can make the selections by pressing the left and right buttons.
//
//*****************************************************************************
static void
LCD_Task(void *pvParameters)
{


    xLastWakeTime_LCD = xTaskGetTickCount();
    LCD_Command(0x0F); //Turn on Lcd
    LCD_Clear(); //Clear screen


    LCD_Cursor(2);
    LCD_print("Ola tudo ben");

    while(1)
    {





    }
}

//*****************************************************************************
//
// Initializes the LED task.
//
//*****************************************************************************
uint32_t
LCD_TaskInit(void)
{
    //
    // Initialize the GPIOs and Timers that drive the three LEDs.
    //


    LCD_init();

    if(xTaskCreate(LCD_Task, (const portCHAR *)"LCD", LCDTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_LCD_TASK, NULL) != pdTRUE)
    {
        return(1);
    }


    //
    // Success.
    //
    return(0);
}
