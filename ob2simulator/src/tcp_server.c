#include "tcp_server.h"

#define ETH0 "eth0"
#define PORT 35000
#define BUFFER_SIZE 100

static int socketServer;
static int socketClient;

static char atzBuffer[]     = "AT Z";
static char ate0Buffer[]    = "AT E0";
static char atl0Buffer[]    = "AT L0";
static char atscBuffer[]    = "AT ST 3c";
static char atspBuffer[]    = "AT SP 0";
static char responseOK[]    = "OK >";

static int8_t atzBufferLength  = 4;
static int8_t ate0BufferLength = 5;
static int8_t atl0BufferLength = 5;
static int8_t atscBufferLength = 8;
static int8_t atspBufferLength = 7;
static int8_t resopnseOKLength = 4;


int8_t TCP_InitTCPServer()
{
    int enable = 1;;
    int family;
    int s;
    struct sockaddr_in sraddr;
    struct sockaddr_in claddr;
    struct ifaddrs *ifaddr, *ifa;
    int length = sizeof(struct sockaddr);
    char host[NI_MAXHOST];
  
    memset(&sraddr, 0x0, sizeof(struct sockaddr_in));
    memset(&claddr, 0x0, sizeof(struct sockaddr_in));
    sraddr.sin_family = AF_INET;
   
    /* creates a linked list of structures describing the network interfaces of the local system */
    if(getifaddrs(&ifaddr) == -1)
    {
        printf("Error getiing ifaddr\n");
        return TCP_SERVER_ERROR;
    }
    
    /* walk through linked list, maintaining head pointer so we
       can free list later */
    for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if(ifa->ifa_addr == NULL)
        {
            continue;
        }

        /*  address-to-name translation in protocol-independent manner */
        s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        
        /* find ipv4 address in eth0 */
        if((strcmp(ifa->ifa_name, ETH0) == 0) && (ifa->ifa_addr->sa_family == AF_INET))
        {
            if(s != 0)
            {
                printf("Error whit getnameinfo()\n");
                return TCP_SERVER_ERROR;
            }
            /* set server ip address */
            inet_aton(host, (struct in_addr *)&sraddr.sin_addr.s_addr);
            printf("##########################################################################\n");
            printf("Generator IPv4 address: %s\n", host);
        }
    }
    /* free ifaddr */
    freeifaddrs(ifaddr);

    sraddr.sin_port = htons(PORT);
    
    /*Create socket */
    socketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == socketServer) 
    {
        printf("Error opening socket: %d\n", errno);
        return TCP_SERVER_ERROR;
    }
    else
    {
        printf("Socket is open.\n");
    }
    
    /* This use to prevent bind problem 98: address already in use */
    if (setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) 
    {
        perror("setsockopt");
        return TCP_SERVER_ERROR;
    }
 
    /* bind socket and server ip address */ 
    if (bind(socketServer, (struct sockaddr *)&sraddr, sizeof(struct sockaddr)) == -1) 
    {
        printf("Bind problem: %d\n", errno);
        return TCP_SERVER_ERROR;
    }
    else
    {
        printf("Bind done.\n");
    }

    /* listen for socketClient */
    if (listen(socketServer, 3) == -1) 
    {
        printf("Listen problem: %d\n", errno);
        return TCP_SERVER_ERROR;
    }
    else
    {
        printf("Listen done.\n");
        printf("##########################################################################\n");
    }
    
    /* accept socketClient */
    socketClient = accept(socketServer,(struct sockaddr *)&claddr, &length);
    if (socketClient == -1)
    {
        printf("Accept problem: %d\n", errno);
        return TCP_SERVER_ERROR;
    }
    else
    {
        printf("Accept done.\n");
        printf("##########################################################################\n");
    }

    return TCP_SERVER_SUCCESS;
}

void TCP_DeInitTCPServer()
{
    close(socketServer);
    close(socketClient);
}

int8_t TCP_ReceiveCommand(char* receiveBuffer)
{
	int8_t bytesReceive = 0;

	bytesReceive = recv(socketClient, receiveBuffer, BUFFER_SIZE, 0);
	if(bytesReceive < 0)
	{
		printf( "TCP:Receive problem: %d\n", errno);
		return TCP_SERVER_ERROR;
	}
	else
	{
		printf( "TCP:Received data: %s\n", receiveBuffer);
		return TCP_SERVER_SUCCESS;
	}
}

int8_t TCP_SendData(char* sendBuffer, int dataSize)
{
	int8_t bytesSend = 0;

	bytesSend = send(socketClient, sendBuffer,dataSize, 0);
	if(bytesSend < 0)
	{
		printf("TCP:Send problem: %d\n", errno);
		return TCP_SERVER_ERROR;
	}
	else
	{
		printf("TCP:Send data: %s\n", sendBuffer);
		return TCP_SERVER_SUCCESS;
	}
}

int8_t TCP_InitODB2Application()
{
	int8_t i = 0;
	char bufferReceive[10];

	TCP_ReceiveCommand(bufferReceive);
	if(memcmp(bufferReceive, atzBuffer, atzBufferLength))
	{
		printf("Invalid command: %s\n", bufferReceive);
		return TCP_SERVER_ERROR;
	}
	TCP_SendData(responseOK, resopnseOKLength);

	memset(bufferReceive, 0x0, strlen(bufferReceive));


	for(i = 0; i < 3; i++)
	{
		TCP_ReceiveCommand(bufferReceive);
		if(memcmp(bufferReceive, ate0Buffer, ate0BufferLength))
		{
			printf("Invalid command: %s\n", bufferReceive);
			return TCP_SERVER_ERROR;
		}
		TCP_SendData(responseOK, resopnseOKLength);

		memset(bufferReceive, 0x0, strlen(bufferReceive));
	}

	TCP_ReceiveCommand(bufferReceive);
	if(memcmp(bufferReceive, atl0Buffer, atl0BufferLength))
	{
		printf("Invalid command: %s\n", bufferReceive);
		return TCP_SERVER_ERROR;
	}
	TCP_SendData(responseOK, resopnseOKLength);

	memset(bufferReceive, 0x0, strlen(bufferReceive));

	TCP_ReceiveCommand(bufferReceive);
	if(memcmp(bufferReceive, atscBuffer, atscBufferLength))
	{
		printf("Invalid command: %s\n", bufferReceive);
		return TCP_SERVER_ERROR;
	}
	TCP_SendData(responseOK, resopnseOKLength);

	memset(bufferReceive, 0x0, strlen(bufferReceive));

	TCP_ReceiveCommand(bufferReceive);
	if(memcmp(bufferReceive, atspBuffer, atspBufferLength))
	{
		printf("Invalid command: %s\n", bufferReceive);
		return TCP_SERVER_ERROR;
	}
	TCP_SendData(responseOK, resopnseOKLength);

	memset(bufferReceive, 0x0, strlen(bufferReceive));

	printf("Initialize communication with application success\n");
	return TCP_SERVER_SUCCESS;

}