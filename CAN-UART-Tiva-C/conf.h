#ifndef CONF_H_
#define CONF_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// CAN
volatile uint32_t RxMsjContador = 0;
volatile uint32_t TxMsjContador = 0;

volatile bool RxFlag = 0;

volatile uint32_t errorFlag = 0;

tCANMsgObject RxMsj;
tCANMsgObject TxMsj;

#define RxID                    0
#define RxObj                   1
#define TxID                    2
#define TxObj                   2

unsigned int i = 0;
unsigned int j = 0;
volatile bool uart = 0;
//Prueba con float
float RxMsjData;
float TxMsjData;

// Flags that contain the current value of the interrupt indicator as displayed
// on the UART.
//
//*****************************************************************************
uint32_t g_ui32Flags;

void configuracion(void)    {    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);}
void configuracionUART()    {
    // Enable  UART1
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    // Enable PortC
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    // Configure the pin multiplexing
    ROM_GPIOPinConfigure(GPIO_PC4_U1RX);
    ROM_GPIOPinConfigure(GPIO_PC5_U1TX);
    // Configure the type of the pins for UART Tx/Rx
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5);
    ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
    // Init the console
    //UARTStdioInit(0);
    UARTFIFODisable(UART1_BASE);
    //UARTFIFOEnable(UART1_BASE);
}
void InitCAN0(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinConfigure(GPIO_PE4_CAN0RX);
    GPIOPinConfigure(GPIO_PE5_CAN0TX);
    GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    CANInit(CAN0_BASE);
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
    IntEnable(INT_CAN0);
    CANEnable(CAN0_BASE);
    //RX
    RxMsj.ui32MsgID = RxID;
    RxMsj.ui32MsgIDMask = 0;
    RxMsj.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER | MSG_OBJ_EXTENDED_ID;
    RxMsj.ui32MsgLen = sizeof(RxMsjData);
    CANMessageSet(CAN0_BASE, RxObj, &RxMsj, MSG_OBJ_TYPE_RX);
    //TX
    TxMsjData = 0;
    TxMsj.ui32MsgID = TxID;
    TxMsj.ui32MsgIDMask = 0;
    TxMsj.ui32Flags = MSG_OBJ_TX_INT_ENABLE | MSG_OBJ_EXTENDED_ID;
    TxMsj.ui32MsgLen = sizeof(TxMsjData);
    TxMsj.pui8MsgData = (uint8_t *)&TxMsjData;
} // void InitCAN0(void)

void CANErrorHandler(void){
    if(errorFlag & CAN_STATUS_BUS_OFF)
        errorFlag &= ~(CAN_STATUS_BUS_OFF);

    if(errorFlag & CAN_STATUS_EWARN)
        errorFlag &= ~(CAN_STATUS_EWARN);

    if(errorFlag & CAN_STATUS_EPASS)
        errorFlag &= ~(CAN_STATUS_EPASS);

    if(errorFlag & CAN_STATUS_RXOK)
        errorFlag &= ~(CAN_STATUS_RXOK);

    if(errorFlag & CAN_STATUS_TXOK)
        errorFlag &= ~(CAN_STATUS_TXOK);

    if(errorFlag & CAN_STATUS_LEC_MSK)
        errorFlag &= ~(CAN_STATUS_LEC_MSK);

    if(errorFlag & CAN_STATUS_LEC_STUFF)
        errorFlag &= ~(CAN_STATUS_LEC_STUFF);

    if(errorFlag & CAN_STATUS_LEC_FORM)
        errorFlag &= ~(CAN_STATUS_LEC_FORM);

    if(errorFlag & CAN_STATUS_LEC_ACK)
        errorFlag &= ~(CAN_STATUS_LEC_ACK);

    if(errorFlag & CAN_STATUS_LEC_BIT1)
        errorFlag &= ~(CAN_STATUS_LEC_BIT1);

    if(errorFlag & CAN_STATUS_LEC_BIT0)
        errorFlag &= ~(CAN_STATUS_LEC_BIT0);

    if(errorFlag & CAN_STATUS_LEC_CRC)
        errorFlag &= ~(CAN_STATUS_LEC_CRC);

    if(errorFlag & CAN_STATUS_LEC_MASK)
        errorFlag &= ~(CAN_STATUS_LEC_MASK);

    if(errorFlag !=0)
        UARTprintf("    Unhandled ERROR: %x \n",errorFlag);
}

void ftoa(float f,char *buf)    {
    int pos=0,ix,dp,num;
    if (f<0)     {
        buf[pos++]='-';
        f = -f;
    }
    dp=0;
    while (f>=10.0)    {
        f=f/10.0;
        dp++;
    }
    for (ix=1;ix<8;ix++)    {
            num = (int)f;
            f=f-num;
            if (num>9)
                buf[pos++]='#';
            else
                buf[pos++]='0'+num;
            if (dp==0) buf[pos++]='.';
            f=f*10.0;
            dp--;
    }
}
void configurarTimer(void){
    // Enable the peripherals used by this example.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    // Enable processor interrupts.
    ROM_IntMasterEnable();
    // Configure the two 32-bit periodic timers.
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet() * 10);  // 16MHz * segundos   %en este caso 10 segundos
    // Setup the interrupts for the timer timeouts.
    ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Enable the timers.
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);
}



void configurarConsola(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

        GPIOPinConfigure(GPIO_PA0_U0RX);
        GPIOPinConfigure(GPIO_PA1_U0TX);

        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

        UARTEnable(UART0_BASE);
}

#endif /* CONF_H_ */
