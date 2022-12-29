#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

//----------------------------Funcao main-----------------------------//
volatile bool errFlag = 0; // transmission error flag


int main(void)
{
    tCANMsgObject msg; // the CAN message object
     unsigned int msgData = 200; // the message data is four bytes long which we can allocate as an int32
    unsigned char *msgDataPtr = (unsigned char *)&msgData;


    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PE4_CAN0RX);
    GPIOPinConfigure(GPIO_PE5_CAN0TX);


    GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);


    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);


    CANInit(CAN0_BASE);


    #if defined(TARGET_IS_TM4C129_RA0) ||                                         \
        defined(TARGET_IS_TM4C129_RA1) ||                                         \
        defined(TARGET_IS_TM4C129_RA2)
        CANBitRateSet(CAN0_BASE, ui32SysClock, 100000);
    #else
        CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 100000);
    #endif

    CANEnable(CAN0_BASE);

        msgData = 0;
        msg.ui32MsgID =0x01;
        msg.ui32MsgIDMask = 0;
        msg.ui32Flags =0;
        msg.ui32MsgLen = sizeof(msgDataPtr);
        msg.pui8MsgData = msgDataPtr;

    while(1)
    {
        CANMessageSet(CAN0_BASE, 1, &msg, MSG_OBJ_TYPE_TX);
    }

	return 0;
}
