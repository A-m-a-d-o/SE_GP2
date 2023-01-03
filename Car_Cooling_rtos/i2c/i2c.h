/*
 * i2c.h
 *
 *  Created on: 15/12/2022
 *      Author: mac
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
#include "inc/hw_i2c.h"
#include "string.h"

#define TMP_101_ADD 0x48
#define SLAVE_ADDR              0x48
#define REG_TEMP_ADDR           0x00
#define REG_CONFIG_ADDR         0x01
#define REG_TLOW                0x02
#define REG_TMAX         0x03

void I2C_Init(void);
void I2C_2byte_Write(uint16_t data, uint8_t reg, uint8_t slave_addr);
uint16_t I2C_2byte_Read(uint8_t reg_addr, uint8_t slave_addr);




#endif /* I2C_H_ */
