#include "can_library.h"

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
static char buffer50kBoudrate[]  = "50k";
static char buffer33k3Boudrate[] = "33k3";

int8_t CAN_findCRLF(char* buffer, int8_t bufferSize)
{
	int8_t i = 0;

	for(i = 0; i < bufferSize; i++)
	{
		if(buffer[i] == CR && buffer[i + 1] == LF)
		{
			return CRLF_FIND;
		}
	}

	if(buffer[bufferSize - 1] == CR)
	{
		return CR_FIND;
	}

	return CRLF_NO_FIND;
}

void CAN_clearCRLF(char* buffer, int8_t bufferSize)
{
	int8_t i = 0;
	for(i = 0; i < bufferSize; i++)
	{
		if(buffer[i] == CR || buffer[i] == LF)
		{
			buffer[i] = 0;
		}
	}
}

int8_t CAN_openUserChanel(int8_t channel, int boudrate)
{
	char bufferSend[30];

	if(channel != 1 && channel != 2)
	{
		printf("Invalid channel\n");
		return CAN_ERROR;
	}

	switch (boudrate)
	{
		case BOUDRATE500K:
			sprintf(bufferSend, "CAN USER OPEN CH%d %s\n", channel, buffer500kBoudrate);
			break;

		case BOUDRATE250K:
			sprintf(bufferSend, "CAN USER OPEN CH%d %s\n", channel, buffer250kBoudrate);
			break;

		case BOUDRATE125K:
			sprintf(bufferSend, "CAN USER OPEN CH%d %s\n", channel, buffer125kBoudrate);
			break;

		case BOUDRATE100K:
			sprintf(bufferSend, "CAN USER OPEN CH%d %s\n", channel, buffer100kBoudrate);
			break;

		case BOUDRATE95K2:
			sprintf(bufferSend, "CAN USER OPEN CH%d %s\n", channel, buffer95k2Boudrate);
			break;

		case BOUDRATE83K3:
			sprintf(bufferSend, "CAN USER OPEN CH%d %s\n", channel, buffer83k3Boudrate);
			break;

		case BOUDRATE50K:
			sprintf(bufferSend, "CAN USER OPEN CH%d %s\n", channel, buffer50kBoudrate);
			break;

		case BOUDRATE33k3:
			sprintf(bufferSend, "CAN USER OPEN CH%d %s\n", channel, buffer33k3Boudrate);
			break;

		default:
			printf("Invalid boudrate\n");
			return CAN_ERROR;
	}

	TEL_sendData(bufferSend, strlen(bufferSend));
	return CAN_SUCCESS;
}

int8_t CAN_setMask(int8_t channel, char* standardId, char* extendedId)
{
	char bufferSend[30];

	if(channel != 1 && channel != 2)
	{
		printf("Invalid channel\n");
		return CAN_ERROR;
	}

	if(extendedId != NULL)
	{
		if(strlen(standardId) != STANDARD_ID_LENGHT && strlen(extendedId) != EXTENDED_ID_LENGHT)
		{
			printf("Invalid ID\n");
			return CAN_ERROR;
		}

		sprintf(bufferSend, "CAN USER MASK CH%d %s:%s\n", channel, standardId, extendedId);
		TEL_sendData(bufferSend, strlen(bufferSend));

		return CAN_SUCCESS;
	}
	else
	{
		if(strlen(standardId) != STANDARD_ID_LENGHT)
		{
			printf("Invalid ID\n");
			return CAN_ERROR;
		}

		sprintf(bufferSend, "CAN USER MASK CH%d %s\n", channel, standardId);
		TEL_sendData(bufferSend, strlen(bufferSend));

		return CAN_SUCCESS;
	}
}

int8_t CAN_setFilter(int8_t channel, int8_t index, char* standardId, char* extendedId)
{
	char bufferSend[40];

	if(channel != CAN_CH1 && channel != CAN_CH2)
	{
		printf("Invalid channel\n");
		return CAN_ERROR;
	}

	if(index < 0 || index > 31)
	{
		printf("Invalid index\n");
		return CAN_ERROR;
	}

	if(extendedId != NULL)
	{
		if(strlen(standardId) != STANDARD_ID_LENGHT && strlen(extendedId) != EXTENDED_ID_LENGHT)
		{
			printf("Invalid ID\n");
			return CAN_ERROR;
		}

		sprintf(bufferSend, "CAN USER FILTER CH%d %d %s:%s\n", channel, index, standardId, extendedId);
		TEL_sendData(bufferSend, strlen(bufferSend));
		return CAN_SUCCESS;
	}
	else
	{
		if(strlen(standardId) != STANDARD_ID_LENGHT)
		{
			printf("Invalid ID\n");
			return CAN_ERROR;
		}

		sprintf(bufferSend, "CAN USER FILTER CH%d %d %s\n", channel, index, standardId);
		TEL_sendData(bufferSend, strlen(bufferSend));
		return CAN_SUCCESS;
	}
}

int8_t CAN_initAlignment(int8_t align)
{
	char bufferSend[30];
	if(align != 1 || align !=2)
	{
		if(align == 1)
		{
			sprintf(bufferSend, "CAN USER ALIGN LEFT\n");
			printf("buffer is %s\n", bufferSend);	
		}
		else
		{
			sprintf(bufferSend, "CAN USER ALIGN RIGHT\n");	
		}
		TEL_sendData(bufferSend, strlen(bufferSend));
	}
	else
	{
		printf( " invalid align\n");
		return CAN_ERROR;
	}
	return CAN_SUCCESS;
}

int8_t CAN_InitCan(CANConfigure* config)
{
	char receiveBuffer[100];

	if(CAN_openUserChanel(config->channel, config->boudrate) == CAN_ERROR)
	{
		printf("Can't open CH%d \n", CAN_CH2);
		return CAN_ERROR;
	}
	else
	{
		TEL_pollTelnet(receiveBuffer);
		printf("Telnet: %s\n", receiveBuffer);
		if(memcmp(receiveBuffer, "OK", 2))
		{
			return CAN_ERROR;
		}
	}
	
	if(CAN_initAlignment(config->align) == CAN_ERROR)
	{
		printf("Can't set alignment\n");
		return CAN_ERROR;
	}
	else
	{
		TEL_pollTelnet(receiveBuffer);
		printf("Telnet: %s\n", receiveBuffer);
		if(memcmp(receiveBuffer, "OK", 2))
		{
			return CAN_ERROR;
		}
	}
	/*
	if(CAN_setMask(config->channel, config->maskStandard, config->maskExtended) == CAN_ERROR)
	{
		printf("Can't set mask\n");
		return CAN_ERROR;
	}
	else
	{
		TEL_pollTelnet(receiveBuffer);
		printf("Telnet: %s\n", receiveBuffer);
		if(memcmp(receiveBuffer, "OK", 2))
		{
			return CAN_ERROR;
		}
	}
*/
	if(CAN_setFilter(config->channel, config->filterIndex, config->filterStandard, config->filterExtended) == CAN_ERROR)
	{
		printf("Can't set filter\n");
		return CAN_ERROR;
	}
	else
	{
		TEL_pollTelnet(receiveBuffer);
		printf("Telnet: %s\n", receiveBuffer);
		if(memcmp(receiveBuffer, "OK", 2))
		{
			return CAN_ERROR;
		}
	}

	printf("CAN is successfuly initalize\n");
	return CAN_SUCCESS;
}

int8_t CAN_RX(char* rxBuffer)
{
	char myBuffer[30];
	char firstPartMessage[TELNET_MESSGAGE_LENGHT];
	char secondPartMessage[TELNET_MESSGAGE_LENGHT];
	char thirdPartMessage[TELNET_MESSGAGE_LENGHT];
	char fourthPartMessage[TELNET_MESSGAGE_LENGHT];
	int8_t lenghtFirstPart = 0;
	int8_t lenghtSecondPart = 0;
	int8_t lenghtThirdPart = 0;
	int8_t ret = 0;
	int8_t counter = 0;
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
	counter ++;
	
	printf("first - %s cnt is %d\n", firstPartMessage,counter);
	//char ID = firstPartMessage & 0x0f;
	//printf("ID is %d\n", &ID);	
	if(TEL_pollTelnet(secondPartMessage) == TEL_ERROR)
	{
		return CAN_ERROR;
	}

	lenghtSecondPart = strlen(secondPartMessage);
	sprintf(rxBuffer + lenghtFirstPart, "%s", secondPartMessage);
	printf("second - %s cnt is %d\n", secondPartMessage,counter);

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
	printf("third - %s cnt is %d\n", &thirdPartMessage[0],counter);
	printf("Whole message is %s\n",&rxBuffer[16]);
	printf("Size of buffer is %d\n", strlen(rxBuffer));
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

int8_t CAN_TX(int8_t channel, char* standardId, char* extendedId, char* txBuffer)
{
	char bufferSend[100];

	if(channel != CAN_CH1 && channel != CAN_CH2)
	{
		printf("Invalid channel\n");
		return CAN_ERROR;
	}

	if(extendedId != NULL)
	{
		if(strlen(standardId) != STANDARD_ID_LENGHT && strlen(extendedId) != EXTENDED_ID_LENGHT)
		{
			printf("Invalid ID\n");
			return CAN_ERROR;
		}

		sprintf(bufferSend, "CAN USER TX CH%d %s:%s %s\n", channel, standardId, extendedId, txBuffer);
		TEL_sendData(bufferSend, strlen(bufferSend));
		return CAN_SUCCESS;
	}
	else
	{
		if(strlen(standardId) != STANDARD_ID_LENGHT)
		{
			printf("Invalid ID\n");
			return CAN_ERROR;
		}

		sprintf(bufferSend, "CAN USER TX CH%d %s %s\n", channel, standardId, txBuffer);
		TEL_sendData(bufferSend, strlen(bufferSend));
		return CAN_SUCCESS;
	}
}

void CAN_getOBD2Command(char* rxBuffer, char* obd2Command)
{
	char buffer[10];
	int8_t i = 0;

	if(rxBuffer[8] != ':')
	{
		sprintf(buffer, "%s", rxBuffer + 9);
	}
	else
	{
		sprintf(buffer, "%s", rxBuffer + 16);
	}

	for(i = 0; i < strlen(buffer); i++)
	{
		if(buffer[i] >= 'A' && buffer[i] <= 'F')
		{
			buffer[i] += 32;
		}
	}

	sprintf(obd2Command, "%c%c %c%c", buffer[0], buffer[1], buffer[2], buffer[3]);
}

void CAN_setOBD2Data(char* obd2Data, char* txHighBuffer, char* txLowBuffer)
{
	int8_t i = 0;
	int8_t j = 0;
	char txBuffer[12];

	/* clear space from data */
	for (i = 0; obd2Data[i] != 0; i++)
	{
		if(obd2Data[i] == '>')
		{
			break;
		}
		if (obd2Data[i] != ' ') 
		{
		   txBuffer[j] = obd2Data[i];
		   j++;
		}
		txBuffer[j] = 0;
	}

	if(strlen(txBuffer) > 8)
	{
		for(i = 0; i < 8; i++)
		{
			sprintf(txHighBuffer + i, "%c", txBuffer[i]);
		}

		for(i = 8; i < strlen(txBuffer); i++)
		{
			sprintf(txLowBuffer + i - 8, "%c", txBuffer[i]);
		}
	}
	else
	{
		sprintf(txLowBuffer, "%s", txBuffer);
	}
}

int8_t CAN_DeInit()
{
	TEL_deInitTelentClient();
	return CAN_SUCCESS;
}
