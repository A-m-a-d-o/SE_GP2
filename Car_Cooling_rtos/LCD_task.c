

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
#define LCDTASKSTACKSIZE               128 // Stack size in words

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
    int i=0;

    char temp[5];


    xLastWakeTime_LCD = xTaskGetTickCount();
    LCD_Command(0x0C); //Turn on Lcd
    LCD_Clear(); //Clear screen
    LCD_Cursor(0);
    LCD_print("Press A-Wait for fan");










    while(1)
    {



        if (xQueueReceive (LCD_write_queue, &data_in, portMAX_DELAY) != pdPASS)
        {
            while(1);
        }

        if(data_in.order_id == SPEED_READING)
        {
            xQueueReset( LCD_write_queue );

            if (xQueueReceive (Menu_lcd_queue, &data_in, portMAX_DELAY) != pdPASS)
                   {
                       while(1);
                   }

            LCD_Command(0x0C); //Turn on Lcd
            LCD_Clear();
            LCD_Cursor(0);
            LCD_print(data_in.pt_pwm);
            LCD_Write_c('%');
            vTaskDelay(500/ portTICK_RATE_MS);
            LCD_Clear();
            LCD_Cursor(5);
            LCD_Write_c(0xDF);
            LCD_Write_c('C');


        }

        if(data_in.order_id == DATE_MODE)
        {

            LCD_Command(0x0F); //Turn on Lcd
            LCD_Clear();
            LCD_print("DD/MM/YYYY");
            LCD_Cursor(0);


            for(i=0;i<10;i++)
              {
                if(i==2 || i==5)
                    {
                          LCD_Cursor(i+1);
                          i++;
                    }



                if (xQueueReceive (Menu_lcd_queue, &data_in, portMAX_DELAY) != pdPASS)
                        {
                            while(1);
                        }

                LCD_Write_c(data_in.ch);
              }
            LCD_Command(0x0C); //Turn on Lcd
            vTaskDelay(500/ portTICK_RATE_MS);
            LCD_Clear();
            LCD_Cursor(5);
            LCD_Write_c(0xDF);
            LCD_Write_c('C');
            xQueueReset( LCD_write_queue );
        }

        if(data_in.order_id == TIME_MODE)
        {
            LCD_Command(0x0F); //Turn on Lcd
            LCD_Clear();
            LCD_print("hh:mm:ss");
            LCD_Cursor(0);



            for(i=0;i<8;i++)
              {
                if(i==2 || i==5)
                    {
                          LCD_Cursor(i+1);
                          i++;
                    }

                if (xQueueReceive (Menu_lcd_queue, &data_in, portMAX_DELAY) != pdPASS)
                        {
                            while(1);
                        }

                LCD_Write_c(data_in.ch);
              }
            LCD_Command(0x0C); //Turn on Lcd
            vTaskDelay(500/ portTICK_RATE_MS);
            LCD_Clear();
            LCD_Cursor(5);
            LCD_Write_c(0xDF);
            LCD_Write_c('C');
            xQueueReset( LCD_write_queue );


        }

       if(data_in.order_id == TEMP_READING)
       {


        float_to_string(temp, data_in.data);



        LCD_Cursor(0);
        if (temp[0] == '0')
            LCD_print(&temp[1]);
        else
            LCD_print(temp);

        LCD_Write_c(0xDF);
        LCD_Write_c('C');



        }
       if(data_in.order_id == MENU_MODE)
       {

            LCD_Clear();
            LCD_print("Menu Mode");

            if (xQueueReceive (Menu_lcd_queue, &data_in, portMAX_DELAY) != pdPASS)
                    {
                        while(1);
                    }


                  if(data_in.order_id == MIN_THR_MODE)
                  {
                   LCD_Command(0x0F); //Turn on Lcd
                   LCD_Clear();
                   LCD_print("Tmin=XXX.XX");
                   LCD_Write_c(0xDF);
                   LCD_Write_c('C');
                   LCD_Cursor(5);

                       for(i=5;i<11;i++)
                         {
                           if(i == 8)
                               {
                                     LCD_Cursor(i+1);
                                     i++;
                               }


                           if (xQueueReceive (Menu_lcd_queue, &data_in, portMAX_DELAY) != pdPASS)
                                   {
                                       while(1);
                                   }

                           LCD_Write_c(data_in.ch);
                         }
                       LCD_Command(0x0C); //Turn on Lcd
                       vTaskDelay(500/ portTICK_RATE_MS);
                       LCD_Clear();
                       LCD_Cursor(5);
                       LCD_Write_c(0xDF);
                       LCD_Write_c('C');
                       xQueueReset( LCD_write_queue );

                  }
                  if(data_in.order_id == MAX_THR_MODE)
                 {
                  LCD_Command(0x0F); //Turn on Lcd
                  LCD_Clear();
                  LCD_print("Tmax=XXX.XX");
                  LCD_Write_c(0xDF);
                  LCD_Write_c('C');
                  LCD_Cursor(5);



                              for(i=5;i<11;i++)
                                {
                                  if(i == 8)
                                      {
                                            LCD_Cursor(i+1);
                                            i++;
                                      }


                                  if (xQueueReceive (Menu_lcd_queue, &data_in, portMAX_DELAY) != pdPASS)
                                          {
                                              while(1);
                                          }

                                  LCD_Write_c(data_in.ch);
                                }
                              LCD_Command(0x0C); //Turn on Lcd
                              vTaskDelay(500/ portTICK_RATE_MS);
                              LCD_Clear();
                              LCD_Cursor(5);
                              LCD_Write_c(0xDF);
                              LCD_Write_c('C');
                              xQueueReset( LCD_write_queue );
                 }
                 if(data_in.order_id == DATE_TIME_SCREEN)
                 {

                    LCD_Command(0x0C); //Turn on Lcd
                    LCD_Clear();
                    LCD_Cursor(0);
                    LCD_print(data_in.pt_date);
                    vTaskDelay(1500/ portTICK_RATE_MS);
                    LCD_Clear();
                    LCD_Cursor(0);
                    LCD_print(data_in.pt_time);
                    vTaskDelay(1500/ portTICK_RATE_MS);
                    LCD_Clear();
                    LCD_Cursor(5);
                    LCD_Write_c(0xDF);
                    LCD_Write_c('C');

                 }




       }




       vTaskDelay( 200/ portTICK_RATE_MS);
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
