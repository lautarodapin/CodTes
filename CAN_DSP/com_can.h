#include "DSP2833x_Device.h"
struct bytes { 
   Uint16      BYTE3:8;     // 31:24
   Uint16      BYTE2:8;     // 23:16
   Uint16      BYTE1:8;     // 15:8
   Uint16      BYTE0:8;     // 7:0
};

union lcf {
	unsigned long l;
	struct bytes c;
	float f;
};

void InitECana500(void);
void configurarMailboxes(void);
void enviarPaqueteCAN(unsigned long ID, unsigned char lng, unsigned long mdl, unsigned long mdh);
unsigned long cambiarEndian(unsigned long tmp);
