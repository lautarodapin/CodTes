#define TARGET_IS_BLIZZARD_RA1
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "utils/uartstdio.h"
#include "driverlib/pin_map.h"
#include <inttypes.h>	//se utiliza para imprimir los valores uint32_t
#include "stdlib.h"
#include "string.h"

//Includes del CAN
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "driverlib/fpu.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"

//Includes del Timer
#include "driverlib/timer.h"

//Configuraciones del sistema
#include "conf.h"


#include "ctype.h" //Funcion para el manejo de datos uart char (isDigit)


// --> esta inicializacion debe estar igual que en el arduino para que haya una sincronizacion correcta
#define cantidadVariables             8

#define IDvar1  1
#define IDvar2  2
#define IDvar3  3
#define IDvar4  4
#define IDbtnStop 5
#define IDbtnStart 6
#define IDCon 7
#define IDFinish 8

float var1, var2, var3, var4, flagStart, flagStop, flagCon, flagFinish; //Variables utilizadas en la com UART
uint16_t identificador[cantidadVariables] = {  //Esta matriz tiene las ID y los punteros a las variables
  IDvar1,
  IDvar2,
  IDvar3,
  IDvar4,
  IDbtnStop,
  IDbtnStart,
  IDCon,
  IDFinish
};
float *variables [cantidadVariables] = {&var1, &var2, &var3, &var4, &flagStop, &flagStart, &flagCon, &flagFinish}; //Puntero a las variables utilizadas en la com UART

#define cantidadVariablesCAN          9       //Cantidad de variables que entran por CAN
// --> Matriz que contiene los identificadores para los paquetes CAN
#define IDCiPanel       1
#define IDCvPanel       2
#define IDCiBat         3
#define IDCvBat         4
#define IDCvCC          5
#define IDCiCarga       6
#define IDCvCarga       7
#define IDCANbtnStart   8
#define IDCANconectado  9

int identificadorCan[cantidadVariablesCAN] = {  //Esta matriz tiene las ID y los punteros a las variables utilizadas en CAN
                                                 IDCiPanel,
                                                 IDCvPanel,
                                                 IDCiBat,
                                                 IDCvBat,
                                                 IDCvCC,
                                                 IDCiCarga,
                                                 IDCvCarga,
                                                 IDCANbtnStart,
                                                 IDCANconectado
};
float CiPanel, CvPanel, CiBat, CvBat, CvCC, CiCarga, CvCarga, flagBtnStart,flagCANconectado;    //Variables utilizadas en el bus CAN
float *CAN [cantidadVariablesCAN]={ &CiPanel, &CvPanel, &CiBat, &CvBat, &CvCC, &CiCarga, &CvCarga, &flagBtnStart, &flagCANconectado}; //Puntero a las variables CAN


#define true    1
#define false   0

uint16_t flag_id;
bool flag_id_rx;
bool flag_request= true; //Usado como disparador para pedir datos al DSP
bool flag_send_arduino = false; //Disparador para enviar datos al arduino
bool flag_request_ended = false;    //Indicador para cuando se recibio el ultimo dato del DSP

char inString[20];
bool flagID = false;
int inID;
float inDato;
uint8_t iChar = 0;
int id_request = 0;

// Declaracion de funciones
extern void configuracion ();
extern void configuracionUART();
extern void InitCAN0(void);
extern void CANErrorHandler(void);
extern void ftoa(float f,char *buf);
extern void configurarTimer();

void read_can_data(void);
void request_can_data(void);
void send_uart_data(void);
void read_uart_data(void);

int main(void) {
    configuracion();
    configuracionUART();
    configurarTimer();
    InitCAN0();
    configurarConsola();


    while (1){
        //Pedir datos CAN
        if (flag_request == true )   {
            request_can_data();
        } //if (flag_request == true && !RxFlag)

        //Recibir datos CAN
        if (RxFlag)     {
            read_can_data();
        }//if(RxFlag)

        //Enviar datos UART
        if(flag_request_ended)  {           //Si ya se recibieron todos los request, procedo a enviar a arduino
                    flag_send_arduino = true;
                    flag_request_ended = false;
        }
        if (flag_send_arduino){
            send_uart_data();
        } // if(flag_send_arduino)

        //Recibir datos UART
        read_uart_data();
    }//while 1
}//main

void request_can_data(void) {
    if (id_request < cantidadVariables ){
         TxMsj.ui32MsgID = identificadorCan[id_request]; //cambio la id mask
         flag_id = TxMsj.ui32MsgID;
         TxMsjData = 0;
         CANMessageSet(CAN0_BASE, TxObj, &TxMsj, MSG_OBJ_TYPE_TX);
         id_request++;
         flag_request = false;
    } else {  //Si se terminaron de pedir todos los datos, dejo de pedir, reinicio el contador, e indico que pedi el ultimo dato
       flag_request = false;
       flag_request_ended = true;
       id_request = 0;
    }
}
void read_can_data(void){
    RxMsj.pui8MsgData = (uint8_t *) &RxMsjData;
    CANMessageGet(CAN0_BASE, RxObj, &RxMsj, 0);
    if(RxMsj.ui32MsgID == IDCANconectado){
        flagCon = true; //Indica que esta conectado, y se enviara al arduino luego
        flagCANconectado = true;
    } else {
        int n_fila;
        for (n_fila = 0; n_fila < cantidadVariables; n_fila++){
            if(RxMsj.ui32MsgID == identificadorCan[n_fila]){
                if (flag_id == RxMsj.ui32MsgID){    flag_request = true; } //Si se recibio lo que se pidio, pongo la request en true nuevamente
                *(CAN[n_fila]) = RxMsjData;
                break;
            }
        }
    }
    RxFlag = 0;
    if(RxMsj.ui32Flags & MSG_OBJ_DATA_LOST) {
            //UARTprintf("\nCAN message loss detected\n");
        }
    flag_request = true; //Se recibio el dato esperado, entonces pido el siguiente
}
void read_uart_data (void) {
    char inChar;
    while (UARTCharsAvail(UART1_BASE)){
        inChar = UARTCharGetNonBlocking(UART1_BASE);
        //UARTCharPut(UART0_BASE,inChar);
        if (!flagID) {
              if (inChar == '|') {
                  inID = atoi(inString);
                  memset(inString,0,sizeof(inString));
                  iChar = 0;
                  flagID = true;
              } else {
                inString[iChar++] = (char)inChar;
              }
            } else {  //Leo datos
              if (inChar != '\n') {
                  inString[iChar++] = (char)inChar;
              } else {
                inDato = atof(inString);
                memset(inString,0,sizeof(inString));
                iChar = 0;
                flagID = false;
                if(inID == IDbtnStop)   {
                    flagCANconectado = false; //Elimino la flag que dice que esta conectado
                    TxMsj.ui32MsgID = IDCANconectado;
                    TxMsjData = flagCANconectado;
                    CANMessageSet(CAN0_BASE, TxObj, &TxMsj, MSG_OBJ_TYPE_TX);
                    //stop_charge();
                } else if (inID == IDbtnStart)  {
                    TxMsj.ui32MsgID = IDCANbtnStart;
                    flagBtnStart = true;
                    TxMsjData = flagBtnStart;
                    CANMessageSet(CAN0_BASE, TxObj, &TxMsj, MSG_OBJ_TYPE_TX);
                }
              }
            }
    }//while uartcharavail
}
void send_uart_data(void){
    //REALIZO UN CALCULO CON LAS VARIABLES OBTENIDAS POR BUS CAN
    var1=CiPanel;
    var2=CvPanel;
    var3=CiBat;
    var4=CvBat;
    //ENVIO AL ARDUINO
    int j, i;
    for (j = 0; j < cantidadVariables; j++) {
        char uID [8], udato[8];
        sprintf(uID,"%d",identificador[j]); //esta funcion convierte el entero en una cadena de caracteres
        ftoa(*variables[j],udato);
        for(i=0; i < 8; i++) {
            UARTCharPut(UART1_BASE, uID[i]);
        }
        UARTCharPut(UART1_BASE,'|'); //Indicacion que termino el identificador
        for(i = 0; i < 8; i++)    {
            UARTCharPut(UART1_BASE, udato[i]);
        }
        UARTCharPut(UART1_BASE, '\n'); //Indicacion del fin de dato
    }
    flag_send_arduino = false;
}


void Timer0IntHandler(void) {        //Esta interrupcion salta de acuerdo con el timer seteado
    // Clear the timer interrupt.
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Toggle the flag for the first timer.
    HWREGBITW(&g_ui32Flags, 0) ^= 1;
    //Aca se coloca lo que se quiere hacer durante la interrupcion
    flag_request = true;        //Procedo a pedir los datos del DSP
    // Update the interrupt status on the display.
    ROM_IntMasterEnable();
}


void CAN0IntHandler(void){
    uint32_t estatus;
    estatus = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);
    if(estatus == CAN_INT_INTID_STATUS)    {
        estatus = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
        errorFlag |= estatus;
    }
    else if(estatus == RxObj)    {
        //estadoLED(LED_VERDE,1);
        CANIntClear(CAN0_BASE, RxObj);
        RxMsjContador++;
        RxFlag = true;
        errorFlag = 0;
    }
    else if(estatus == TxObj)    {
        CANIntClear(CAN0_BASE, TxObj);
        TxMsjContador++;
        errorFlag = 0;
    }
    else    {
        //
        // Spurious interrupt handling can go here.
        //
    }
}
