#include <pthread.h>
#include "generator.h"
#include "tcp_server.h"
#include "obd2diagnostic.h"
#include "telnet-client.h"
#include "can_library.h"

pthread_mutex_t mainMutex;
pthread_cond_t mainCondVariable;
CANConfigure config;

void getGeneratorData(tGeneratorData* parsedDataGenerator)
{
	tOBD2VehicleData obd2Data;
    
	obd2Data.vehicleSpeed = parsedDataGenerator->vehicleSpeed;
	obd2Data.engineCoolantTemperature = parsedDataGenerator->engineCoolantTemperature;
	obd2Data.engineRPM = parsedDataGenerator->engineRPM;
	obd2Data.fuelRemaining = parsedDataGenerator->fuelRemaining;
    obd2Data.numberOfDTC = parsedDataGenerator->numberOfDTC;
    obd2Data.statusOfMIL = parsedDataGenerator->statusOfMIL;
    obd2Data.engineRunTime = parsedDataGenerator->engineRunTime;
    obd2Data.distanceTravelWithMILOn = parsedDataGenerator->distanceTravelWithMILOn;
    obd2Data.timeRunWithMILOn = parsedDataGenerator->timeRunWithMILOn;

	OBD2_setVehileData(&obd2Data);
}

void* tcpCommunication(void* args)
{
    char receiveBuffer[100];
    char sendBuffer[100];

    TCP_InitTCPServer();
    TCP_InitODB2Application();

    while(1)
    {
        TCP_ReceiveCommand(receiveBuffer);

        OBD2_getDiagnostic(receiveBuffer, sendBuffer);

        TCP_SendData(sendBuffer, strlen(sendBuffer));

        memset(receiveBuffer, 0x0, strlen(receiveBuffer));
        memset(sendBuffer, 0x0, strlen(sendBuffer));
    }
}

void* canCommunication(void* args)
{

    char rxBuffer[100];
    char obd2Data[100];
    char obd2Command[5];
    char txHighBuffer[8];
    char txLowBuffer[8];

    while(1)
    {
        CAN_RX(rxBuffer);
        printf("CAN %s\n", rxBuffer);

        CAN_getOBD2Command(rxBuffer, obd2Command);
        OBD2_getDiagnostic(obd2Command, obd2Data);
        CAN_setOBD2Data(obd2Data, txHighBuffer, txLowBuffer);


        CAN_TX(config.channel, CAN_DEFAULT_ID, NULL, txLowBuffer);
        TEL_pollTelnet(rxBuffer);
        if(memcmp(rxBuffer, "OK", 2))
        {
            printf("%s\n", rxBuffer);
            pthread_mutex_lock(&mainMutex);
            pthread_cond_signal(&mainCondVariable);
            pthread_mutex_unlock(&mainMutex);
        }


        if(strlen(txHighBuffer) != 0)
        {
            CAN_TX(config.channel, CAN_DEFAULT_ID, NULL, txHighBuffer);
            TEL_pollTelnet(rxBuffer);
            if(memcmp(rxBuffer, "OK", 2))
            {
                printf("%s\n", rxBuffer);
                pthread_mutex_lock(&mainMutex);
                pthread_cond_signal(&mainCondVariable);
                pthread_mutex_unlock(&mainMutex);
            }
        }
        
        
        memset(rxBuffer, 0x0, 100);
        memset(txHighBuffer, 0x0, 8);
        memset(txLowBuffer, 0x0, 8);
        memset(obd2Data, 0x0, 100);
        memset(obd2Command, 0x0, 5);
    }
}

void* endProgram(void* args)
{
    char inputCaracter = 0;
    inputCaracter = getchar();

    while(1)
    {
        switch(inputCaracter)
        {
            case 'q':
            case 'Q':
                pthread_mutex_lock(&mainMutex);
                pthread_cond_signal(&mainCondVariable);
                pthread_mutex_unlock(&mainMutex);
                break;

            default:
                break;
        }
    }
}

int main(int argc, char *argv[])
{
    pthread_t tcpThread;
    pthread_t canThread;
    pthread_t exitThread;
    int32_t typeOfGenerator = 0;

    pthread_mutex_init(&mainMutex, NULL);
    pthread_cond_init (&mainCondVariable, NULL);

    if(argc != 2)
    {
        printf("Usage: %s typeOfGenerator\n", argv[0]);
        return -1;
    }
    typeOfGenerator = atoi(argv[1]);


    if(typeOfGenerator == INI_GENERATOR)
    {
        GEN_initGenerator(INI_GENERATOR);
    }
    else if(typeOfGenerator == CVS_GENERATOR)
    {
        GEN_initGenerator(CVS_GENERATOR);
    }
    else
    {
        printf("Wrong input\n");
        return -1;
    }
    
    GEN_registerGeneratorCallback(getGeneratorData);

    if(TEL_InitTelnetClient() == TEL_ERROR)
    {
        printf("Can't initalize libtelnet\n");
        return -1;
    }

    config.channel = CAN_CH2;
    config.boudrate = BOUDRATE500K;
    config.maskStandard = CAN_DEFAULT_MASK;
    config.maskExtended = NULL;
    config.filterIndex = 0;
    config.filterStandard = CAN_DEFAULT_ID;
    config.filterExtended = NULL;

    if(CAN_InitCan(&config) == CAN_ERROR)
    {
        printf("Error in CAN initalize\n");
        return -1;
    }


    if(pthread_create(&canThread, NULL, canCommunication, NULL) != 0)
    {
        printf("Thread is not created \n");
        return -1;
    }

    if(pthread_create(&tcpThread, NULL, tcpCommunication, NULL) != 0)
    {
        printf("Thread is not created \n");
        return -1;
    }

    if(pthread_create(&exitThread, NULL, endProgram, NULL) != 0)
    {
        printf("Thread is not created \n");
        return -1;
    }

    pthread_mutex_lock(&mainMutex);
    pthread_cond_wait(&mainCondVariable, &mainMutex);
    pthread_mutex_unlock(&mainMutex);

    CAN_DeInit();
    GEN_deInitGenerator();
    TCP_DeInitTCPServer();
	return 0;
}


