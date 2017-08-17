#ifndef __CAN_LIBRARY_H__
#define __CAN_LIBRARY_H__

#include "telnet-client.h"

#define CAN_ERROR 		-1
#define CAN_SUCCESS 	0

#define BOUDRATE500K 	500
#define BOUDRATE250K 	250
#define BOUDRATE125K 	125
#define BOUDRATE100K 	100
#define BOUDRATE95K2 	952
#define BOUDRATE83K3 	833
#define BOUDRATE50K  	50
#define BOUDRATE33k3 	333
#define ALIGNMENTLEFT	1
#define ALIGNMENTRIGHT  2

#define CAN_CH1 1
#define CAN_CH2 2

#define CAN_DEFAULT_MASK 	"ffff"
#define CAN_DEFAULT_ID 		"0030"


typedef struct _CANConfigure
{
	int8_t channel;
	int boudrate;
	char* maskStandard;
	char* maskExtended;
	int8_t filterIndex;
	char* filterStandard;
	char* filterExtended;
	int8_t align;
}CANConfigure;


void CAN_clearCRLF(char* buffer, int8_t bufferSize);
int8_t CAN_findCRLF(char* buffer, int8_t bufferSize);
int8_t CAN_openUserChanel(int8_t channel, int boudrate);
int8_t CAN_setMask(int8_t channel, char* standardId, char* extendedId);
int8_t CAN_setFilter(int8_t channel, int8_t index, char* standardId, char* extendedId);
int8_t CAN_initAlignment(int8_t align);
int8_t CAN_InitCan(CANConfigure* config);
int8_t CAN_RX(char* rxBuffer);
int8_t CAN_TX(int8_t channel, char* standardId, char* extendedId, char* txBuffer);
void CAN_getOBD2Command(char* rxBuffer, char* obd2Command);
void CAN_setOBD2Data(char* obd2Data, char* txHighBuffer, char* txLowBuffer);
int8_t CAN_DeInit();

#endif /*__CAN_LIBRARY_H__*/
