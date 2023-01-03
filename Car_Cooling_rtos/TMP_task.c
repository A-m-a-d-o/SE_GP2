

#include <stdbool.h>
#include <stdint.h>
#include <TMP_task.h>
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
#include "i2c/i2c.h"

//*****************************************************************************
//
// The stack size for the LED toggle task.
//
//*****************************************************************************
#define TMPTASKSTACKSIZE        128         // Stack size in words

//*****************************************************************************
//
// The item size and queue size for the LED message queue.
//
//*****************************************************************************
#define TMP_ITEM_SIZE           sizeof(uint8_t)
#define TMP_QUEUE_SIZE          5


//*****************************************************************************
//
// The queue that holds messages sent to the LED task.
//
//*****************************************************************************
xQueueHandle g_pLEDQueue;


extern xSemaphoreHandle g_pUARTSemaphore;

//*****************************************************************************
//
// This task toggles the user selected LED at a user selected frequency. User
// can make the selections by pressing the left and right buttons.
//
//*****************************************************************************
static void
TMP_Task(void *pvParameters)
{

        //vTaskDelayUntil(&ui32WakeTime, ui32LEDToggleDelay / portTICK_RATE_MS);

}

//*****************************************************************************
//
// Initializes the LED task.
//
//*****************************************************************************
uint32_t
TMP_TaskInit(void)
{
    //
    // Initialize the GPIOs and Timers that drive the three LEDs.
    //

    g_pLEDQueue = xQueueCreate(TMP_QUEUE_SIZE, TMP_ITEM_SIZE);

    //
    // Create the LED task.
    //
    if(xTaskCreate(TMP_Task, (const portCHAR *)"TMP", TMPTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_TMP_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}
