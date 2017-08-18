#ifndef __INITIALIZE_H__
#define __INITIALIZE_H__

#include "telnet-client.h"
#include "can_library.h"
//#include "signals.h" 



typedef struct 
{
	int8_t ID;
	
}CanData;

void* init_Main();
void setConfig(CANConfigure *config);
void initOBD2();
void initTELNET();
void initCAN(CANConfigure *config);

int8_t CAN_RX_THREAD();
int8_t calculateValues();


#endif  /*__INITIALIZE_H__*/

