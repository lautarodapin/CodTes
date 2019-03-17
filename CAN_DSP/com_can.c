#include "DSP2833x_Device.h"
#include "com_can.h"
//#include <math.h>

void InitECana500(void)	{

	struct ECAN_REGS ECanaShadow;

	EALLOW;		// EALLOW enables access to protected bits

/* Configure eCAN RX and TX pins for CAN operation using eCAN regs*/

    ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
    ECanaShadow.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;

    ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
    ECanaShadow.CANRIOC.bit.RXFUNC = 1;
    ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;

/* Configure eCAN for HECC mode - (reqd to access mailboxes 16 thru 31) */
									// HECC mode also enables time-stamping feature

	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.SCB = 1;
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

/* Initialize all bits of 'Master Control Field' to zero */
// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
// all bits (including reserved bits) of MSGCTRL must be initialized to zero

    ECanaMboxes.MBOX0.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX1.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX2.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX3.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX4.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX5.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX6.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX7.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX8.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX9.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX11.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX12.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX13.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX14.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX15.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX16.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX17.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX18.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX19.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX20.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX21.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX22.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX23.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX24.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX25.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX26.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX27.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX28.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX29.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX30.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX31.MSGCTRL.all = 0x00000000;

// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
//	as a matter of precaution.

	ECanaRegs.CANTA.all	= 0xFFFFFFFF;	/* Clear all TAn bits */

	ECanaRegs.CANRMP.all = 0xFFFFFFFF;	/* Clear all RMPn bits */

	ECanaRegs.CANGIF0.all = 0xFFFFFFFF;	/* Clear all interrupt flag bits */
	ECanaRegs.CANGIF1.all = 0xFFFFFFFF;


/* Configure bit timing parameters for eCANA*/
	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaShadow.CANES.all = ECanaRegs.CANES.all;

    do {
	    ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 1 );  		// Wait for CCE bit to be set..

    ECanaShadow.CANBTC.all = 0;

	ECanaShadow.CANBTC.bit.BRPREG = 9;
	ECanaShadow.CANBTC.bit.TSEG2REG = 2;
	ECanaShadow.CANBTC.bit.TSEG1REG = 10;

    ECanaShadow.CANBTC.bit.SAM = 1;
    ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaShadow.CANES.all = ECanaRegs.CANES.all;

    do {
       ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 0 ); 		// Wait for CCE bit to be  cleared..

/* Disable all Mailboxes  */
 	ECanaRegs.CANME.all = 0;		// Required before writing the MSGIDs

    EDIS;
}

void configurarMailboxes() {
	
	struct ECAN_REGS ECanaShadow;

	// Mailbox 0 como salida
	/* Configure Mailbox as transmit mailbox */
	ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;	
	ECanaShadow.CANMD.bit.MD0 = 0;
	ECanaRegs.CANMD.all = ECanaShadow.CANMD.all; 
	
	/* Enable Mailbox 0  */
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;	
	ECanaShadow.CANME.bit.ME0 = 1;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all; 


	// Mailbox 1 como entrada
	/* Primero deshabilitar para configurar  */
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.bit.ME1 = 0;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;
	/* Configuro el ID que va a recibir  */
	ECanaMboxes.MBOX1.MSGID.all     = 0x1;	// message identifier
	ECanaMboxes.MBOX1.MSGID.bit.IDE = 1;  // Extended Identifier
	ECanaMboxes.MBOX1.MSGID.bit.AME = 1;  // Habilito la mascara de aceptación
	ECanaLAMRegs.LAM1.all = 0x1FFFFFFF;            // Mascara (0 tiene que matchear, 1 no importa)
	/* Dirección como entrada   */
	ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
	ECanaShadow.CANMD.bit.MD1 = 1;
	ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
	/* Ya esta configurada, habilito */
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.bit.ME1 = 1;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;

}

void enviarPaqueteCAN(unsigned long ID, unsigned char lng, unsigned long mdl, unsigned long mdh) {

    struct	ECAN_REGS ECanaShadow;

    // Deshabilitar el mailbox para configurar
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.bit.ME0 = 0;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;

	// Escribir el ID del mensaje (29 bits)
	ECanaMboxes.MBOX0.MSGID.all     = ID & 0x1FFFFFFF;
	ECanaMboxes.MBOX0.MSGID.bit.IDE = 1;

    // Escribir el contenido del paquete
	ECanaMboxes.MBOX0.MDL.all = cambiarEndian(mdl);
	ECanaMboxes.MBOX0.MDH.all = cambiarEndian(mdh);
    
    // Longitud del paquete
	ECanaMboxes.MBOX0.MSGCTRL.all = 0;
	ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = lng;

    // Habilitar el mailbox
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.bit.ME0 = 1;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;

    // Iniciar envío
	ECanaShadow.CANTRS.all = 0;
	ECanaShadow.CANTRS.bit.TRS0 = 1; 
	ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;

    // Esperar que termine
	while(ECanaRegs.CANTA.bit.TA0 == 0 ) {
		EALLOW;
		SysCtrlRegs.WDKEY = 0xAA;
		EDIS;
	}

    // Aceptar que esta todo listo
	ECanaShadow.CANTA.all = 0;
	ECanaShadow.CANTA.bit.TA0 = 1;
	ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
    
}

unsigned long cambiarEndian(unsigned long tmp) {
    return (tmp << 24) |
          ((tmp <<  8) & 0x00ff0000) |
          ((tmp >>  8) & 0x0000ff00) |
          ((tmp >> 24) & 0x000000ff);
}

