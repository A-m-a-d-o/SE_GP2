/*
 * i2c.c
 *
 *  Created on: 15/12/2022
 *      Author: mac
 */

#include "i2c.h"

void i2c_init(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);     //Enables I2C1 peripheral
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);    //Enables GPIO PORT A

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));  //Waits for the peripherals to initialize
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1));

    //Configures pins for SCL and SDA

    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);



    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);    //Setups the I2C1 clock frequency



}
void sensor_init(void)
{
    I2CMasterSlaveAddrSet(I2C1_BASE, TMP_101_ADD, false);       //Master sends the Slave Address
    I2CMasterDataPut(I2C1_BASE, 0x01);                          //Sets the Pointer to the Configuration Register
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C1_BASE));
    I2CMasterDataPut(I2C1_BASE, 0x2E);                          //Setting the Resolution to 10 bits and continuous readings
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C1_BASE));
}
void temp_get(void)
{

    uint16_t temp;


    I2CMasterSlaveAddrSet(I2C1_BASE, TMP_101_ADD, false);

    I2CMasterDataPut(I2C1_BASE, 0x00);
    SysCtlDelay(5000000);
    I2CMasterDataPut(I2C1_BASE, 0x00);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusBusy(I2C1_BASE));


    I2CMasterSlaveAddrSet(I2C1_BASE, TMP_101_ADD, true);
    temp = I2CMasterDataGet(I2C1_BASE) << 8;
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(I2CMasterBusBusy(I2C1_BASE));

    temp |=  I2CMasterDataGet(I2C1_BASE);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusBusy(I2C1_BASE));



    temp = temp >> 4;
    return;


}


