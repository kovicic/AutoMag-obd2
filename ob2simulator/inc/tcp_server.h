#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>   
#include <netdb.h> 
#include <ifaddrs.h>      
#include <unistd.h>
#include <string.h>

#define TCP_SERVER_ERROR -1
#define TCP_SERVER_SUCCESS 0

int8_t TCP_InitTCPServer();
void TCP_DeInitTCPServer();
int8_t TCP_ReceiveCommand(char* receiveBuffer);
int8_t TCP_SendData(char* sendBuffer, int dataSize);
int8_t TCP_InitODB2Application();

#endif /* __TCP_SERVER_H__*/