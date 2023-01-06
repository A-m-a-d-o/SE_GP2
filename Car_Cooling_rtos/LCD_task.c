

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



//*****************************************************************************
//
// The stack size for the LED toggle task.
//
//*****************************************************************************
#define LCDTASKSTACKSIZE                // Stack size in words

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
    lcd_msg data_in;

    char temp[5];


    xLastWakeTime_LCD = xTaskGetTickCount();
    LCD_Command(0x0C); //Turn on Lcd
    LCD_Clear(); //Clear screen
    LCD_Cursor(5);
    LCD_Write_c(0xDF);
    LCD_Write_c('C');







    while(1)
    {

        if (xQueueReceive (LCD_write_queue, &data_in, portMAX_DELAY) != pdPASS)
        {
            while(1);
        }

        float_to_string(temp, data_in.data);

        LCD_Cursor(0);
        if (temp[0] == '0')
            LCD_print(&temp[1]);
        else
            LCD_print(temp);



       vTaskDelayUntil(&xLastWakeTime_LCD, 200/ portTICK_RATE_MS);
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
