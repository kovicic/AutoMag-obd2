#include "initialize.h" 

#define EXTENDED_ID_LENGHT 	6
#define STANDARD_ID_LENGHT	4

#define CRLF_FIND 		1
#define CR_FIND 		2
#define CRLF_NO_FIND 	3

#define CR 13
#define LF 10

#define TELNET_MESSGAGE_LENGHT 8

#define PID0020 "0100"
#define PID2140 "0120"
#define PID4160 "0140"

static char buffer500kBoudrate[] = "500k";
static char buffer250kBoudrate[] = "250k";
static char buffer125kBoudrate[] = "125k";
static char buffer100kBoudrate[] = "100k";
static char buffer95k2Boudrate[] = "95k2";
static char buffer83k3Boudrate[] = "83k3";
static char buffer50kBoudrate[] = "50k";
static char buffer33k3Boudrate[] = "33k3";
static CANConfigure config;

char readBuffer[512];
char tempBuffer[512][30];
static int8_t counter = 0;
char canID[4];
int8_t dataBuffer[TELNET_MESSGAGE_LENGHT];
static char *checkPid[] = { PID0020, PID2140, PID4160};


void initOBD2(CANConfigure *config)
{   
    char rxBuffer[100];
    int8_t i = 0;
    
    for(i = 0; i < 3; i++)
    {
        CAN_TX(config->channel, CAN_DEFAULT_ID, NULL, checkPid[i]);
        TEL_pollTelnet(rxBuffer);
        if(memcmp(rxBuffer, "OK", 2))
        {
            printf("%s\n", rxBuffer);
            return;
        }
     }   
    
    //CAN_RX_THREAD(rxBuffer);
	printf("%s\n", rxBuffer);
    

    printf("****************************************\n");
    printf("\n\n");
}

void initTELNET()
{
	
	if(TEL_InitTelnetClient() == TEL_ERROR)
    {
        printf("Can't initalize libtelnet\n");
    }
    else
    {
		printf("Telnet initialized successfully\n");
	}
    
}

void initCAN(CANConfigure *config)
{
    
	if(CAN_InitCan(config) == CAN_ERROR)
    {
        printf("Error in CAN initalize\n");
    }
	
}

void initData(dashboard *db)
{
	db->kmh = 0;
	db->rpm = 0;
	db->fuel = 0;
}
void setConfig(CANConfigure *config)
{
	config->channel = CAN_CH2;
    config->boudrate = BOUDRATE250K;
    config->maskStandard = CAN_DEFAULT_MASK;
    config->maskExtended = NULL;
    config->filterIndex = 0;
    config->filterStandard = CAN_DEFAULT_ID;
    config->filterExtended = NULL;
    config->align = ALIGNMENTLEFT;

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
		//printf(" %d value :\n",dataBuffer[0]);
		//printf(" %d value :\n",dataBuffer[1]);
		printf("Second value is  %d\n",dataBuffer[4] * 1000 + dataBuffer[5] * 100 + dataBuffer[6] * 10 + dataBuffer[7]);
		/*
		for( j = 0; j < TELNET_MESSGAGE_LENGHT; j++)
		{
			printf("Value of %d is %d\n", j, dataBuffer[j]);
		}
		*/
	}
	return 0;
}

int8_t CAN_RX_THREAD(dashboard *db)
{
	char rxBuffer[30];
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
		
		db->ID = 5500;
		//printf(" ID IS %d\n", db->ID);
		//printf("IMAMO ID 5500\n");
	}
	else if(!strcmp(canID, "4400"))
	{
		db->ID = 4400;
		//printf("IMAMO ID 4400\n");
	}
	 printf(".");   
	//printf(". %d --- %s\n",strlen(canID),canID);
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
		//memcpy(&tempBuffer[counter], readBuffer, strlen(readBuffer));
		counter ++;
		//printf("Whole message is %s and CND is %d\n",&readBuffer, counter);
		
		pthread_mutex_lock(&db->lock);
		db->count ++;
		int value = 0;
		strcpy(&db->data[db->count], &readBuffer[9]);
		switch(db->ID)
		{
			case 5500:
				db->kmh = htoi((&db->data[db->count])[1]) + 10 * htoi((&db->data[db->count])[0]);
				db->fuel = htoi((&db->data[db->count])[7]) + 10 * htoi((&db->data[db->count])[6]);
				
				//printf(" kmh- %d fuel - %d \n\n",db->kmh, db->fuel);
				//strcpy(db->kmh,&db->data[db->count]);
			break;
			case 4400:
				db->rpm = 1000 * htoi((&db->data[db->count])[4]) + 
							100 * htoi((&db->data[db->count])[5]) +
							 10 * htoi((&db->data[db->count])[6]) + 
								  htoi((&db->data[db->count])[7]);
				//printf(" rpm-s: %d\n", db->rpm);
			break;
			default:
				break;
		}
		//mmstrcpy(db->kmh,&readBuffer[9]);
		//printf("staro %s\n", thirdPartMessage);
		printf("WRITE data - %s i ID %d and cnt %d\n", &db->data[db->count],db->ID,db->count);
		
		
		//printf(" Punimo cnt - %d a data  %s\n",db->count,&db->data[db->count + 1]);
		pthread_mutex_unlock(&db->lock);
		
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


void* init_Main(void * data)
{
	int i = 1;
	int cnt = 0;
	dashboard *db = (dashboard *)data;
	/*for(i = 0; i < 100; i++)
	{
		db->data[i] = 2 * i;
		db->head ++;
		printf(" Data init is %d and head is %d\n", db->data[i], db->head);
	}
	*/
	setConfig(&config);
    initTELNET();
    initCAN(&config);
    initData(db);
	//initOBD2(&config);
	while(1)
	{	
		CAN_RX_THREAD(db);
		/*printf("ispreD1:\n");
		pthread_mutex_lock(&db->lock);
		
		
		db->data[db->count + 1] = i;
		printf("WRITE: data[%d] = %d \n", db->count + 1, i);
		db->count ++;
		
		pthread_mutex_unlock(&db->lock);
		printf("iza:1\n");
		i++;
		if(cnt > 120)
		{
			
			break;
		}
		cnt ++;*/
	}
	printf("\n");
	calculateValues();
	
}
