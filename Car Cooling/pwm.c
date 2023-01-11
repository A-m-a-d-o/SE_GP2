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
#include "driverlib/pwm.h"
#include "math.h"

#define TICK_COUNT 50000



void PWM_Init(void){

    // PWM module 0 initializations
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // PB Port defined and configured for PWM
    GPIOPinConfigure(GPIO_PB4_M0PWM2);

    GPIOPinConfigure(GPIO_PB5_M0PWM3);

    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);

    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);

    //
    // Enable the PWM Module 0 peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    //

    // Wait for the PWM module 0 to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0))
    {
    }
    //
    // Configure the PWM generator for count down mode with immediate updates
    // to the parameters.
    //
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1,
    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
}

void PWM_Fan_Control(float perc){

    uint32_t pulse_width;

    pulse_width = TICK_COUNT * (perc/100);

    pulse_width = roundf(pulse_width);

    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, pulse_width);



}



