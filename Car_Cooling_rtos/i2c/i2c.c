/*
 * i2c.c
 *
 *  Created on: 23/12/2022
 *      Author: Pedro Amado
 */


#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <math.h>



#define SLAVE_ADDR              0x48
#define REG_TEMP_ADDR           0x00
#define REG_CONFIG_ADDR         0x01
#define REG_TLOW                0x02
#define REG_TMAX         0x03



// I2C hardware functions
/*
PA6 -----> SCL
PA7 -----> SDA
*/


xSemaphoreHandle i2c_semaphore;

void i2c_handler(void)
{
    I2CMasterIntClear(I2C1_BASE);
    static portBASE_TYPE xHigherPriorityTaskWoken;
     xHigherPriorityTaskWoken = pdFALSE;
     /* 'Give' the semaphore to unblock the task. */
     xSemaphoreGiveFromISR( i2c_semaphore, &xHigherPriorityTaskWoken );
     if( xHigherPriorityTaskWoken == pdTRUE ) {
     /* Giving the semaphore unblocked a task, and the priority of the unblocked task
     is higher than the currently running task - force a context switch to ensure that
     the interrupt returns directly to the unblocked (higher priority) task.
     NOTE: The actual macro to use (context switch) from an ISR is dependent on the
     port. This is the correct macro for the Open Watcom DOS port. Other ports may
     require different syntax */
     portEND_SWITCHING_ISR (xHigherPriorityTaskWoken);
     }


}

// I2C Peripheral Initializations


void I2C_Init(void){

    // I2C module 1 initializations
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // PA Ports defined and configured for I2C
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

    I2CIntRegister(I2C1_BASE, i2c_handler);


    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);

}



//Writes 2 Bytes on I2C bus
void I2C_2byte_Write(uint16_t data, uint8_t reg, uint8_t slave_addr)
{
    I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, false); // Sets I2C for a write transaction

    I2CMasterDataPut(I2C1_BASE, reg);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    xSemaphoreTake( i2c_semaphore, portMAX_DELAY );

    I2CMasterDataPut(I2C1_BASE, (data >> 8));
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    xSemaphoreTake( i2c_semaphore, portMAX_DELAY );

    I2CMasterDataPut(I2C1_BASE, (data & 0x00FF));
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    xSemaphoreTake( i2c_semaphore, portMAX_DELAY );

}


// Reads 2 Bytes from I2C bus
uint16_t I2C_2byte_Read(uint8_t reg_addr, uint8_t slave_addr)
{
    uint16_t data = 0;

    I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, false);  // Sets I2C for a write transaction

    I2CMasterDataPut(I2C1_BASE, reg_addr);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    xSemaphoreTake( i2c_semaphore, portMAX_DELAY );

    I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, true); // Sets I2C for a read transaction

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    xSemaphoreTake( i2c_semaphore, portMAX_DELAY );
    data = I2CMasterDataGet(I2C1_BASE) << 8;


    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    xSemaphoreTake( i2c_semaphore, portMAX_DELAY );
    data |= I2CMasterDataGet(I2C1_BASE);

    return data;
}


