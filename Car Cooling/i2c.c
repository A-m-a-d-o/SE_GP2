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
#define REG_TLOW           0x02
#define REG_TMAX         0x03


//initialize I2C module 1
void I2CInit(void){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);


}

void config_i2c(void){

    I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDR, false);

    I2CMasterDataPut(I2C1_BASE, 0x01);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterDataPut(I2C1_BASE, 0x6E);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C1_BASE));
}

void i2c_data_write(uint16_t data, uint8_t reg)
{
    I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDR, false);

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




uint16_t I2CRead(uint8_t reg_addr)
{
    uint16_t data = 0;

    I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDR, false);

    I2CMasterDataPut(I2C1_BASE, reg_addr);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDR, true);

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(I2CMasterBusy(I2C1_BASE));
    data = I2CMasterDataGet(I2C1_BASE) << 8;


    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C1_BASE));
    data |= I2CMasterDataGet(I2C1_BASE);

    return data;
}

float temp_get (void){
    uint16_t aux;





    aux= I2CRead(0x00);

    if((aux && 0x8000) == 0x8000)
    {
        aux &= 0x7FFF;

        aux = aux >> 6;

            return -(aux * 0.25);

    }

    aux = aux >> 6;

    return aux * 0.25;
}

uint8_t set_temp(float temp_max, uint8_t reg)
{
    const float precision = 0.0625;
    float math_aux;
    uint16_t aux;


    if (temp_max < 0 && temp_max > -128 )
    {
        temp_max = fabs(temp_max);
        math_aux = fmod(temp_max, precision);
        temp_max -= math_aux;
        aux = temp_max / precision;
        aux = (~aux) +1;
        aux = aux << 4;
        i2c_data_write(aux, reg);
        return 1;
    }
    else if (temp_max > 0 && temp_max < 128)
    {
        math_aux = fmod(temp_max, precision);
        temp_max -= math_aux;
        aux = temp_max / precision;
        aux = aux << 4;
        i2c_data_write(aux, reg);
        return 1;
    }
 return -1;
}
