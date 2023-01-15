

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
#include "i2c/i2c.h"
#include "TMP_task.h"
#include "global_types.h"

//*****************************************************************************
//
// The stack size for the TMP toggle task.
//
//*****************************************************************************
#define TMPTASKSTACKSIZE        128         // Stack size in words

//*****************************************************************************
//
// The item size and queue size for the TMP message queue.
//
//*****************************************************************************
#define TMP_ITEM_SIZE           sizeof(uint8_t)
#define TMP_QUEUE_SIZE          5
#define TMP_Delay               100



extern xSemaphoreHandle i2c_semaphore;

// Config I2C for TMP 101
void config_i2c(void){

    I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDR, false);

    I2CMasterDataPut(I2C1_BASE, 0x01);                             //TMP Config Reg
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterDataPut(I2C1_BASE, 0x6E);                             //TMP 9 bits Res., 2 Faults, continuous mode
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterIntEnable(I2C1_BASE);

    I2CMasterIntEnableEx(I2C1_BASE, I2C_MASTER_INT_STOP);

}

//Reads temperature from TMP 101 Sensor
float temp_get (void){
    uint16_t aux;

    aux= I2C_2byte_Read(REG_TEMP_ADDR, SLAVE_ADDR);                // Gets 2 raw bytes from temperature register.

    //Determine if reading is negative
    if((aux && 0x8000) == 0x8000)
    {
        aux &= 0x7FFF;

        aux = aux >> 6;

            return -(aux * 0.25);

    }
    // Working with 10 bits
    aux = aux >> 6;


    return aux * 0.25; // Converts number to degrees celsius
}
// Sets alarm temperature threshold
uint8_t set_temp(float temp_max, uint8_t reg)
{
    const float precision = 0.0625;
    float math_aux;
    uint16_t aux;

// Determines if alarm number is negative
    if (temp_max < 0 && temp_max > -128 )
    {
        temp_max = fabs(temp_max);              //if its negative writes value in 2's complement
        math_aux = fmod(temp_max, precision);
        temp_max -= math_aux;
        aux = temp_max / precision;
        aux = (~aux) +1;
        aux = aux << 4;
        I2C_2byte_Write(aux, reg, SLAVE_ADDR);
        return 1;
    }
    else if (temp_max > 0 && temp_max < 128)
    {
        math_aux = fmod(temp_max, precision);  //if the value is positive writes the value in 2's complement
        temp_max -= math_aux;
        aux = temp_max / precision;
        aux = aux << 4;
        I2C_2byte_Write(aux, reg, SLAVE_ADDR);
        return 1;
    }
 return -1;
}


//*****************************************************************************
//
// This task toggles the user selected LED at a user selected frequency. User
// can make the selections by pressing the left and right buttons.
//
//*****************************************************************************
static void
TMP_Task(void *pvParameters)
{

    portTickType xLastWakeTime;
    float temp=0.0;
    pwm_msg motor_control;
    lcd_msg data_out;
    data_out.order_id = TEMP_READING;
    motor_control.order_id = TEMP_READING;

    set_temp(24, REG_TMAX);
    set_temp(22, REG_TLOW);



    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {

        xSemaphoreTake (temp_mutex, portMAX_DELAY);
        xSemaphoreGive (temp_mutex);

        temp=temp_get();

        data_out.data = temp;
        motor_control.temp = temp;

        xQueueSendToBack (LCD_write_queue, &data_out , 50/ portTICK_RATE_MS);


       xQueueSendToBack (PWM_queue, &motor_control, 50/ portTICK_RATE_MS);


        vTaskDelayUntil(&xLastWakeTime, TMP_Delay/ portTICK_RATE_MS);



    }
}

//*****************************************************************************
//
// Initializes the LED task.
//
//*****************************************************************************
uint32_t
TMP_TaskInit(void)
{

    vSemaphoreCreateBinary( i2c_semaphore );
    //
    // Create the LED task.
    //
    if(xTaskCreate(TMP_Task, (const portCHAR *)"TMP", TMPTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_TMP_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    I2C_Init();
    config_i2c();

    //
    // Success.
    //
    return(0);
}
