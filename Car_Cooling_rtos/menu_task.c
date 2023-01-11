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


portTickType xLastWakeTime_Keypad;

extern xSemaphoreHandle keypad_semaphore;


//*****************************************************************************
//
// The stack size for the LED toggle task.
//
//*****************************************************************************
#define MENUTASKSTACKSIZE              128  // Stack size in words




static void
Menu_Task(void *pvParameters)
{
    int i;
    char key, date[8] , time[6], max_temp[5], min_temp[5];
    int date_num [3] = {0};
    lcd_msg data_out;
    pwm_msg data_pwm;
    float temp = 0.0;
    uint32_t ticks=0, t_sec=0;




    xSemaphoreTake (temp_mutex, portMAX_DELAY);  // desligar temp

   while(1)
   {

    if (xQueueReceive (Menu_queue, &key, portMAX_DELAY) != pdPASS)
            {
                while(1);
            }


    switch(key)
    {

    case 'A':

        xSemaphoreGive (temp_mutex);    //ligar temp


        break;

    case 'C':



       memset(date,0,8);

        data_out.order_id = DATE_MODE;

        xQueueReset( LCD_write_queue );

        if (xQueueSendToFront (LCD_write_queue, &data_out , portMAX_DELAY) != pdPASS )
                {
                    while(1);
                }



        for(i=0;i<8;i++)
        {
            while(date[i]<'0' || date[i]>'9' )
            {
        if (xQueueReceive (Menu_queue, &date[i], portMAX_DELAY) != pdPASS)
                   {
                       while(1);
                   }
            }
        data_out.ch=date[i];

        if (xQueueSendToBack (Menu_lcd_queue, &data_out , portMAX_DELAY) != pdPASS )
                    {
                        while(1);
                    }


        }

       char_to_int(date_num, date);




        break;

    case 'D':



        memset(time,0,6);

                data_out.order_id = TIME_MODE;

                xQueueReset( LCD_write_queue );

                if (xQueueSendToFront (LCD_write_queue, &data_out , portMAX_DELAY) != pdPASS )
                               {
                                   while(1);
                               }
                for(i=0;i<6;i++)
                {
                    while(time[i]<'0' || time[i]>'9' )
                    {
                if (xQueueReceive (Menu_queue, &time[i], portMAX_DELAY) != pdPASS)
                           {
                               while(1);
                           }
                    }
                data_out.ch=time[i];

                if (xQueueSendToBack (Menu_lcd_queue, &data_out , portMAX_DELAY) != pdPASS )
                            {
                                while(1);
                            }
                }

                ticks = (uint32_t) xTaskGetTickCount();
                t_sec = string_to_seconds( time);


        break;


    case 'F':

        data_out.order_id = MENU_MODE;

        if (xQueueSendToFront (LCD_write_queue, &data_out , portMAX_DELAY) != pdPASS )
           {
               while(1);
           }

        while(key<'A' || key>'D' )
        {
            if (xQueueReceive (Menu_queue, &key, portMAX_DELAY) != pdPASS)
                {
                    while(1);
                }
        }


        switch(key)
        {

        case 'A':

            memset(min_temp,0,5);

            data_out.order_id = MIN_THR_MODE;

            data_pwm.order_id = MIN_THR_MODE;



            if (xQueueSendToBack (Menu_lcd_queue, &data_out , portMAX_DELAY) != pdPASS )
            {
                while(1);
            }



                    for(i=0;i<5;i++)
                    {
                        while(min_temp[i]<'0' || min_temp[i]>'9' )
                        {
                    if (xQueueReceive (Menu_queue, &min_temp[i], portMAX_DELAY) != pdPASS)
                               {
                                   while(1);
                               }
                        }
                    data_out.ch=min_temp[i];

                    if (xQueueSendToBack (Menu_lcd_queue, &data_out , portMAX_DELAY) != pdPASS )
                        {
                            while(1);
                        }
                    }

                    char_to_float( &temp, min_temp);

                    data_pwm.temp = temp;

                    if (xQueueSendToBack (PWM_queue, &data_pwm , portMAX_DELAY) != pdPASS )
                        {
                            while(1);
                        }
            break;

        case 'B':

            memset(max_temp,0,5);

            data_out.order_id = MAX_THR_MODE;
            data_pwm.order_id = MAX_THR_MODE;



            if (xQueueSendToBack (Menu_lcd_queue, &data_out , portMAX_DELAY) != pdPASS )
            {
                while(1);
            }



                    for(i=0;i<5;i++)
                    {
                        while(max_temp[i]<'0' || max_temp[i]>'9' )
                        {
                    if (xQueueReceive (Menu_queue, &max_temp[i], portMAX_DELAY) != pdPASS)
                               {
                                   while(1);
                               }
                        }
                    data_out.ch=max_temp[i];

                    if (xQueueSendToBack (Menu_lcd_queue, &data_out , portMAX_DELAY) != pdPASS )
                                {
                                    while(1);
                                }
                    }


                    char_to_float( &temp, max_temp);

                    data_pwm.temp = temp;

                    if (xQueueSendToBack (PWM_queue, &data_pwm, portMAX_DELAY) != pdPASS )
                       {
                           while(1);
                       }

            break;

        case 'C':

            data_out.order_id = DATE_TIME_SCREEN;

            uint32_t now_ticks = (uint32_t) xTaskGetTickCount(), sec_passed, sec_now;

            sec_passed = ((now_ticks - ticks) * portTICK_RATE_MS)/1000;

            if (sec_passed > 86400)
            {
               uint8_t days;

               days = sec_passed / 86400;

               sec_passed =- 86400 * days;

               date_num [0] += days;

            }

           sec_now = sec_passed + t_sec;

            seconds_to_string(sec_now, data_out.pt_time);
            int_to_char(date_num, data_out.pt_date);


            if (xQueueSendToBack (Menu_lcd_queue, &data_out , portMAX_DELAY) != pdPASS )
            {
               while(1);
            }



            break;


        }

        break;


    }
   }



}


uint32_t
Menu_TaskInit(void)
{
    //
    // Initialize the GPIOs and Timers that drive the three LEDs.
    //

    if(xTaskCreate(Menu_Task, (const portCHAR *)"MENU", MENUTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_MENU_TASK, NULL) != pdTRUE)
    {
        return(1);
    }
    //
    // Success.
    //
    return(0);
}




