#ifndef __INITIALIZE_H__
#define __INITIALIZE_H__

#include "telnet-client.h"
#include "can_library.h"

void* init_Main(void *);
int8_t CAN_RX_THREAD(char* rxBuffer);

int8_t tryNewThread();


#endif  /*__INITIALIZE_H__*/
