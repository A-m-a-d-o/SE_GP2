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

#define SLAVE_ADDR              0x48
#define REG_TEMP_ADDR           0x00
#define REG_CONFIG_ADDR         0x01
#define REG_TLOW_ADDR           0x02
#define REG_THIGH_ADDR          0x03

//-----------------------Prototipos das funcoes-----------------------//

void I2CInit(void);
void I2CWrite(uint8_t reg_addr, uint16_t data);
uint16_t I2CRead(uint8_t reg_addr);
void I2CWriteConf(uint8_t data);
float ReadTemp (void);

//-----------------------Definicao das funcoes-----------------------//

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

void set_temp_max(float temp_max)
{
    uint16_t aux;

    aux = temp_max / 0.0625;



    I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDR, false);

    I2CMasterDataPut(I2C1_BASE, 0x03);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterDataPut(I2C1_BASE, 0x6E);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C1_BASE));

}


//----------------------------Funcao main-----------------------------//

int main(void)
    {

        float temp;

        I2CInit();
        config_i2c();
        while(1){

        temp = temp_get();

    }

	return 0;
}
