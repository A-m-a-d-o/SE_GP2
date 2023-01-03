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

    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);

}
// Config I2C for TMP 101
void config_i2c(void){

    I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDR, false);

    I2CMasterDataPut(I2C1_BASE, 0x01);                             //TMP Config Reg
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterDataPut(I2C1_BASE, 0x6E);                             //TMP 9 bits Res., 2 Faults, continuous mode
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C1_BASE));
}


//Writes 2 Bytes on I2C bus
void I2C_2byte_Write(uint16_t data, uint8_t reg, uint8_t slave_addr)
{
    I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, false); // Sets I2C for a write transaction

    I2CMasterDataPut(I2C1_BASE, reg);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterDataPut(I2C1_BASE, (data >> 8));
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterDataPut(I2C1_BASE, (data & 0x00FF));
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C1_BASE));

}

// Reads 2 Bytes from I2C bus
uint16_t I2C_2byte_Read(uint8_t reg_addr, uint8_t slave_addr)
{
    uint16_t data = 0;

    I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, false);  // Sets I2C for a write transaction

    I2CMasterDataPut(I2C1_BASE, reg_addr);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, true); // Sets I2C for a read transaction

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(I2CMasterBusy(I2C1_BASE));
    data = I2CMasterDataGet(I2C1_BASE) << 8;


    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C1_BASE));
    data |= I2CMasterDataGet(I2C1_BASE);

    return data;
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

