#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
//#include <math.h>
#include "com_can.h"

#define true 1
#define false 0

int ID;
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
//El orden de los valores de la matriz va segun la matriz "comparador"
int identificadorCan[cantidadVariablesCAN] = {  //Esta matriz tiene las ID y los punteros a las variables
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
float CiPanel, CvPanel, CiBat, CvBat, CvCC, CiCarga, CvCarga, flagBtnStart,flagCANconectado;
float *CAN [cantidadVariablesCAN]={ &CiPanel, &CvPanel, &CiBat, &CvBat, &CvCC, &CiCarga, &CvCarga, &flagBtnStart, &flagCANconectado};



//##################################################################################//
//						     Declaracion de funciones       					    //
//##################################################################################//

// Funciones Internas
void Gpio_select(void);

// Funciones Externas
extern void InitSysCtrl(void);
extern void InitPieCtrl(void);
extern void InitPieVectTable(void);
extern void InitCpuTimers(void);
extern void ConfigCpuTimer(struct CPUTIMER_VARS *, float, float);

interrupt void cpu_timer0_isr(void);

void main(void) {
    struct ECAN_REGS ECanaShadow;
	InitSysCtrl();			// Inicializacion basica
	DINT;					// Desabilita todas las interrupciones
    InitPieCtrl();			// Inicializa el PIE
	InitPieVectTable();		// Inicializa la tabla PIE a un estado inicial
	Gpio_select();			// Configuracion del GPIO
	InitECana500();
	configurarMailboxes();

	// Definiciones iniciales
	flagBtnStart = false;
	flagCANconectado = false;
	int flagConectado = false;
    union lcf val;

	while(1) {
	    if (flagConectado == true)   {  //Bandera que verifica la conexion de un vehiculo
	        flagCANconectado = true;
	        val.f = flagCANconectado;
	        enviarPaqueteCAN(IDCANconectado,4,val.f,val.f);
	    }
	    while(ECanaRegs.CANRMP.bit.RMP1 == 1) {
            // Limpio el RMP
            ECanaShadow.CANRMP.all = 0;
            ECanaShadow.CANRMP.bit.RMP1 = 1;
            ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
            ID = ECanaMboxes.MBOX1.MSGID.all;
            val.l = cambiarEndian(ECanaMboxes.MBOX1.MDL.all) +cambiarEndian(ECanaMboxes.MBOX1.MDH.all);
            int i;
            if (ID == IDCANbtnStart && val.f == true)   {
                if (flagBtnStart == true) {
                    flagBtnStart = false;
                    // Habilitar bandera para la carga del vehiculo electrico
                }
            } else if (ID == IDCANconectado && val.f == false)   {
                    flagCANConectado = false;
                    // Desconectar la carga
            } else {
                for(i = 0; i   <  cantidadVariablesCAN; i++)  {
                    if(ID == identificadorCan[i])    {
                         val.f = *(CAN[i]);
                         enviarPaqueteCAN (identificadorCan[i], 4,  val.l,  val.l);
                         //i = cantidadVariables; //Salgo del loop for cuando se identifico el paquete
                         break;
                    }
                }
            }
	    } //while(ECanaRegs.CANRMP.bit.RMP1 == 1) {
	}
}

void Gpio_select(void) {

	EALLOW;

	GpioCtrlRegs.GPAMUX1.all = 0x00000000;	  // GPIO0-GPIO15 = I/O Proposito General
	GpioCtrlRegs.GPAMUX2.all = 0x00000000;	  // GPIO16-GPIO31 = I/O Proposito General

	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;	// CANA_RX
	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;	// CANA_TX

	GpioCtrlRegs.GPBMUX1.all = 0x00000000;	  // GPIO32-GPIO47 = I/O Proposito General
	GpioCtrlRegs.GPBMUX2.all = 0x00000000;	  // GPIO48-GPIO63 = I/O Proposito General
	GpioCtrlRegs.GPCMUX1.all = 0x00000000;	  // GPIO64-GPIO79 = I/O Proposito General
	GpioCtrlRegs.GPCMUX2.all = 0x00000000;	  // GPIO80-GPIO87 = I/O Proposito General

	GpioCtrlRegs.GPADIR.all = 0;              // GPIO0-GPIO31 Como entrada
	GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;	      // peripheral explorer: LED LD1 at GPIO9
	GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;	      // peripheral explorer: LED LD2 at GPIO11
	GpioCtrlRegs.GPBDIR.all = 0;		      // GPIO32-GPIO63 Como entrada
	GpioCtrlRegs.GPCDIR.all = 0;		      // GPIO64-GPIO87 Como entrada

	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;  	  // GPIO0 = EPWM1A
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;  	  // GPIO1 = EPWM1B
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;  	  // GPIO2 = EPWM2A
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;  	  // GPIO3 = EPWM2B
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;  	  // GPIO4 = EPWM3A
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;  	  // GPIO5 = EPWM3B

	GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;	      // GPIO10 = Led   //Como salida

	EDIS;
}
