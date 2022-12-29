
#include <msp430g2553.h>
#include <stdint.h>

void initClockTo16MHz()
{
    if (CALBC1_16MHZ==0xFF)                  // If calibration constant erased
    {
        while(1);                               // do not load, trap CPU!!
    }
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_16MHZ;                    // Set DCO
    DCOCTL = CALDCO_16MHZ;
}

change_pwm_duty(uint8_t duty)
{
    if(duty <100 & duty > 0)
    {
        CCR0 = 1000;                             // PWM Period
        CCTL1 = OUTMOD_7;                         // CCR1 reset/set
        CCR1 = duty*10;                               // CCR1 PWM duty cycle
        TACTL = TASSEL_2 + MC_1 + ID_3;                  // SMCLK, up mode
       }
    }




int main(void)
{

    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    initClockTo16MHz();

      P1DIR |= BIT6;                            // P1.2 output
      P1SEL |= BIT6;                            // P1.2 for TA0.1 output
      P1SEL2 = 0;                               // Select default function for P1.2
      change_pwm_duty(50);

      __bis_SR_register(CPUOFF);                // Enter LPM0
}
