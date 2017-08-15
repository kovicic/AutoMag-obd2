#include "telnet-client.h"
#include "can_library.h"

#define PID0020 "0100"
#define PID2140 "0120"
#define PID4160 "0140"

#define PID_RPM             "010C"
#define PID_SPEED           "010D"
#define PID_COOLANT_TEMP    "0105"
#define PID_FUEL_LEVEL      "012F"

static CANConfigure config;
static char *checkPid[] = { PID0020, PID2140, PID4160};
void initOBD2()
{   
    char rxBuffer[100];
    int8_t i = 0;

    for(i = 0; i < 3; i++)
    {
        CAN_TX(config.channel, CAN_DEFAULT_ID, NULL, checkPid[i]);
        TEL_pollTelnet(rxBuffer);
        if(memcmp(rxBuffer, "OK", 2))
        {
            printf("%s\n", rxBuffer);
            return;
        }

        CAN_RX(rxBuffer);
        printf("%s\n", rxBuffer);

        CAN_RX(rxBuffer);
        printf("%s\n", rxBuffer);
    }

    printf("****************************************\n");
    printf("\n\n");
}


int main()
{
    char rxBuffer[100];
    char obdCommand[5];
    int option;
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

    initOBD2();

    while(1)
    {
        printf("Chose car information: \n");
        printf("1.Vehicle speed\n");
        printf("2.Vehicle rpm\n");
        printf("3.Coolant enigne temperature\n");
        printf("4.Fuel level\n");
        printf("5.Exit\n");

        scanf("%d", &option);

        switch(option)
        {
            case 1:
                strcpy(obdCommand, PID_SPEED);
                break;

            case 2:
                strcpy(obdCommand, PID_RPM);
                break;

            case 3:
                strcpy(obdCommand, PID_COOLANT_TEMP);              
                break;

            case 4:

                strcpy(obdCommand, PID_FUEL_LEVEL);
                break;
            case 5:

                return 0;
            default:
                break;
        }

        CAN_TX(config.channel, CAN_DEFAULT_ID, NULL, obdCommand);
        TEL_pollTelnet(rxBuffer);
        if(memcmp(rxBuffer, "OK", 2))
        {
            printf("%s\n", rxBuffer);
            return -1;
        }

        CAN_RX(rxBuffer);
        printf("%s\n", rxBuffer);

        printf("\n\n");
    }
    return 0;
}