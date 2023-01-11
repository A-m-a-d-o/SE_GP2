#include <stdbool.h>
#include <stdint.h>
#include <string.h>
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
#include "pwm/pwm.h"
#include "PWM_task.h"
#include "global_types.h"
#include "string_handler/string_handler.h"
#include "driverlib/pwm.h"
#include "math.h"



//*****************************************************************************
//
// The stack size for the LED toggle task.
//
//*****************************************************************************
#define PWMTASKSTACKSIZE               128 // Stack size in words

//*****************************************************************************
//
// The item size and queue size for the LED message queue.
//
//*****************************************************************************
#define TEMP_MAX_THR           26



//*****************************************************************************
//
// The queue that holds messages sent to the LED task.
//
//*****************************************************************************
xQueueHandle g_pLEDQueue;

portTickType xLastWakeTime_LCD;




static void
PWM_Task(void *pvParameters)
{

    long Start = xTaskGetTickCount(),prev_buzzer_on,prev_buzzer_off;
    float  temp_min = 22, temp_max = 27, perc = 0;

    pwm_msg temp;

    int alarm = 20;

    while(1)
    {

        if (xQueueReceive (PWM_queue, &temp, portMAX_DELAY) != pdPASS)
                {
                    while(1);
                }
        if(temp.order_id == TEMP_READING)
        {

            if(temp.temp <= 10)
            {
                perc = 0;
            }
            else
            {
                perc = 3.1567e-4 * pow(temp.temp,3) - 0.0281 * pow(temp.temp,2) + 1.487 * temp.temp + 0.1;

                if(perc > 100)
                    perc = 100;
            }

            if(temp.temp < temp_min)
            {
                alarm = MIN_THR_MODE;

                xQueueSendToBack (Alert_queue, &alarm,portMAX_DELAY);

            }
            else if(alarm == MIN_THR_MODE || alarm == MIN_THR_MODE)
            {

                alarm = STOP_ALARM;

                xQueueSendToBack (Alert_queue, &alarm,portMAX_DELAY);

                alarm = 20;

            }

            if(temp.temp > temp_max)
            {

                alarm = MAX_THR_MODE;

                xQueueSendToBack (Alert_queue, &alarm,portMAX_DELAY);

            }
            else
            {

                alarm = STOP_ALARM;

                xQueueSendToBack (Alert_queue, &alarm,portMAX_DELAY);

                alarm = 20;


            }

            PWM_Fan_Control(perc);

        }
        /*
        if(temp.order_id == MIN_THR_MODE)
        {

           temp_min = temp.temp;

        }

        if(temp.order_id == MAX_THR_MODE)
        {

           temp_max = temp.temp;

        }
        */

    }

}


uint32_t
PWM_TaskInit(void)
{
    //
    // Initialize the GPIOs and Timers that drive the three LEDs.
    //


    PWM_Init();


    if(xTaskCreate(PWM_Task, (const portCHAR *)"PWM", PWMTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_PWM_TASK, NULL) != pdTRUE)
    {
        return(1);
    }


    //
    // Success.
    //
    return(0);
}

