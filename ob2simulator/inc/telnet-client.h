#ifndef __TELNET_CLIENT_H__
#define __TELNET_CLIENT_H__


#if !defined(_POSIX_SOURCE)
#	define _POSIX_SOURCE
#endif
#if !defined(_BSD_SOURCE)
#	define _BSD_SOURCE
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>

#ifdef HAVE_ZLIB
#include "zlib.h"
#endif

#include "libtelnet.h"

#define TEL_ERROR 		-1
#define TEL_SUCCESS 	 0

int8_t TEL_InitTelnetClient();
int8_t TEL_deInitTelentClient();
int8_t TEL_pollTelnet(char* buffer);
void TEL_sendData(char* sendBuffer, int8_t bufferSize);
void TEL_turnOnLed1();
void TEL_turnOffLed1();
void TEL_turnOnLed2();
void TEL_turnOffLed2();
#endif /*__TELNET_CLIENT_H__ */