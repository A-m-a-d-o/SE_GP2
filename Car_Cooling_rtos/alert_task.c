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



#define ALERTTASKSTACKSIZE               128 // Stack size in words

static void
Alert_Task(void *pvParameters)
{

    portTickType xLastWakeTime;


    uint8_t alarm_order;




    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {

        if (xQueueReceive (Alert_queue, &alarm_order, portMAX_DELAY) != pdPASS)
           {
               while(1);
           }

        if(alarm_order == MIN_THR_MODE)
        {
            while(1)
            {

            if (alarm_order == STOP_ALARM)
            {
               PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT , false);
               break;
            }

            PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT , true);
            vTaskDelayUntil(&xLastWakeTime, 300 / portTICK_RATE_MS);
            PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT , false);



           vTaskDelayUntil(&xLastWakeTime, 500 / portTICK_RATE_MS);

           xQueueReceive (Alert_queue, &alarm_order, 0);
            }

        }
        if(alarm_order == MAX_THR_MODE)
        {
            while(1)
            {


             if (alarm_order == STOP_ALARM)
             {
                PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT , false);
                break;
             }
            PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT , true);
            vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);
            PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT , false);

           vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);

           xQueueReceive (Alert_queue, &alarm_order, 0);
            }
         }




    }
}




uint32_t
Alert_TaskInit(void)
{

    if(xTaskCreate(Alert_Task, (const portCHAR *)"ALERT", ALERTTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_ALERT_TASK, NULL) != pdTRUE)
    {
        return(1);
    }


    //
    // Success.
    //
    return(0);
}



