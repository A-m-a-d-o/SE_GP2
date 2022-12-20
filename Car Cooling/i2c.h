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

void i2c_init(void);

void sensor_init(void);

void temp_get(void);


#endif /* I2C_H_ */
