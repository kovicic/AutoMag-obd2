#include "initialize.h" 

#define EXTENDED_ID_LENGHT 	6
#define STANDARD_ID_LENGHT	4

#define CRLF_FIND 		1
#define CR_FIND 		2
#define CRLF_NO_FIND 	3

#define CR 13
#define LF 10

#define TELNET_MESSGAGE_LENGHT 8

static char buffer500kBoudrate[] = "500k";
static char buffer250kBoudrate[] = "250k";
static char buffer125kBoudrate[] = "125k";
static char buffer100kBoudrate[] = "100k";
static char buffer95k2Boudrate[] = "95k2";
static char buffer83k3Boudrate[] = "83k3";
static char buffer50kBoudrate[] = "50k";
static char buffer33k3Boudrate[] = "33k3";

int8_t tryNewThread()
{
	printf(" ***************************" );	
	return 5;
}

char readBuffer[512];
char tempBuffer[512][30];
static int8_t counter = 0;
char canID[4];
int8_t dataBuffer[TELNET_MESSGAGE_LENGHT];
void* init_Main(void *data)
{

	while(1)
	{
		CAN_RX_THREAD(data);
		if(counter > 5)
		{
			break;
		}
	}
	calculateValues();
	
}

static int htoi(char c)
{
	 int ret = 0;
	 
	 if('a'<= c && c <= 'f')
	 {
		ret = c - 'a' + 10;
	 }
	 else if('A' <= c && c <= 'F')
	 {
		ret = c - 'A' + 10;
	 }
	 else if('0' <= c && c <= '9')
	 {
		ret = c - '0';
	 }
	 
	 return ret;
}

int8_t calculateValues()
{
	int8_t i = 0;
	int8_t j = 0;
	int8_t value = 0;
	//char ID = &tempBuffer[i][
	for(i = 0; i < counter; i++)
	{
		//int ID = &tempBuffer[i][4] >> 13;
		//printf(" ID IS %s\n", canID);
		//printf("Package %d is %s\n",i,&tempBuffer[i][4]);
		//value = htoi(tempBuffer[i][16])+(htoi(tempBuffer[i][15])<<4);
		//printf(" Value is %d\n",value);
		//od 9
		for(j = 0; j < TELNET_MESSGAGE_LENGHT ; j ++)
		{
			dataBuffer[j] = htoi(tempBuffer[i][j + 9]);
		}
		
		value = dataBuffer[7] + dataBuffer[6];
		printf(" %d value :\n",dataBuffer[0]);
		printf(" %d value :\n",dataBuffer[1]);
		printf(" %d value :\n",dataBuffer[4] * 1000 + dataBuffer[5] * 100 + dataBuffer[6] * 10 + dataBuffer[7]);
		/*
		for( j = 0; j < TELNET_MESSGAGE_LENGHT; j++)
		{
			printf("Value of %d is %d\n", j, dataBuffer[j]);
		}
		*/
	}
	return 0;
}

int8_t CAN_RX_THREAD(char* rxBuffer)
{
	//printf(" USAO U THREAD JBNI");
	char myBuffer[30];
	char firstPartMessage[TELNET_MESSGAGE_LENGHT];
	char secondPartMessage[TELNET_MESSGAGE_LENGHT];
	char thirdPartMessage[TELNET_MESSGAGE_LENGHT];
	char fourthPartMessage[TELNET_MESSGAGE_LENGHT];
	int8_t lenghtFirstPart = 0;
	int8_t lenghtSecondPart = 0;
	int8_t lenghtThirdPart = 0;
	int8_t ret = 0;
	
	if(TEL_pollTelnet(firstPartMessage) == TEL_ERROR)
	{
		return CAN_ERROR;
	}

	lenghtFirstPart = strlen(firstPartMessage);

	if(memcmp(firstPartMessage, "RX", 2))
	{
		return CAN_ERROR;
	}

	sprintf(rxBuffer, "%s", firstPartMessage);
	
	//printf("first - %s cnt is %d\n", &firstPartMessage[4],counter);
	//canID = &firstPartMessage[4];
	//printf("first - %s cnt is %d\n", &firstPartMessage[4],counter);
	strcpy(canID, (&firstPartMessage[4]));

	if(!strcmp(canID, "5500"))
	{
		printf("IMAMO ID 5500");
	}
	else if(!strcmp(canID, "4400"))
	{
		printf("IMAMO ID 4400");
	}
	    
	printf(". %d --- %s\n",strlen(canID),canID);
	//char ID = firstPartMessage & 0x0f;
	//printf("ID is %d\n", &ID);	
	if(TEL_pollTelnet(secondPartMessage) == TEL_ERROR)
	{
		return CAN_ERROR;
	}

	lenghtSecondPart = strlen(secondPartMessage);
	sprintf(rxBuffer + lenghtFirstPart, "%s", secondPartMessage);
	//printf("second - %s cnt is %d\n", secondPartMessage,counter);

	ret = CAN_findCRLF(secondPartMessage, TELNET_MESSGAGE_LENGHT);
	if(ret == CRLF_FIND)
	{
		CAN_clearCRLF(rxBuffer, strlen(rxBuffer));
		return CAN_SUCCESS;
	}
	else if(ret == CR_FIND)
	{
		if(TEL_pollTelnet(thirdPartMessage) == TEL_ERROR)
		{
			return CAN_ERROR;
		}
		CAN_clearCRLF(rxBuffer, strlen(rxBuffer));
		return CAN_SUCCESS;
	}
	else
	{
		if(TEL_pollTelnet(thirdPartMessage) == TEL_ERROR)
		{
			return CAN_ERROR;
		}

		lenghtThirdPart = strlen(thirdPartMessage);
		sprintf(rxBuffer + lenghtFirstPart + lenghtSecondPart, "%s", thirdPartMessage);
		
		memcpy(readBuffer,rxBuffer, strlen(rxBuffer));
		memcpy(&tempBuffer[counter], readBuffer, strlen(readBuffer));
		counter ++;
		//printf("Whole message is %s and CND is %d\n",&readBuffer, counter);
		ret = CAN_findCRLF(thirdPartMessage, TELNET_MESSGAGE_LENGHT);
		if(ret == CRLF_FIND)
		{
			CAN_clearCRLF(rxBuffer, strlen(rxBuffer));
			return CAN_SUCCESS;
		}
		else
		{
			if(TEL_pollTelnet(fourthPartMessage) == TEL_ERROR)
			{
				return CAN_ERROR;
			}

			return CAN_SUCCESS;
		}
	}

}
