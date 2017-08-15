#include "generator.h"

#define milIsON     1
#define milIsOFF    0

#define SIMULATOR_DELAY 1
#define LINE 1024

/* use for easly find data in cvs file */
#define C_TEMPERATURE 13
#define E_RMP 14
#define E_SPEED 15
#define FUEL_L 20

static GeneratorCallback callbackGeneratorFunction;
static pthread_t dataVehicleThread;

static char numberOfLapsBuffer[]        = "number_of_laps=";
static char startFuelBuffer[]           = "start_fuel=";
static char startCoolantTempBuffer[]    = "start_coolant_temp=";
static char startEngineRPMBuffer[]      = "start_rpm=";
static char lapLengthBuffer[]           = "length=";
static char vehicleSpeedBuffer[]        = "speed=";
static char vehicleGearBuffer[]         = "gear=";
static char numberOfDTCBuffer[]         = "dtc_number=";

static int8_t lengthNumberOfLapsBuffer      = 15;
static int8_t lengthStartFuelBuffer         = 11;
static int8_t lengthStartCoolantTemp        = 19;
static int8_t lengthStartEngineRPM          = 10;
static int8_t lengthLapLengthBuffer         = 7;
static int8_t lengthVehicleSpeedBuffer      = 6;
static int8_t lengthVehicleGearBuffer       = 5;
static int8_t lengthNumberOfDTCBuffer       = 11;

static int8_t lengthLapSection              = 5;
static int8_t lengthGeneralSection          = 5;

static double convertKMHtoMS                = 2.8;
                    
                        //for gear: 1      2     3     4     5     6
static double gearRatio[]       = { 2.97, 2.07, 1.43, 1.00, 0.84, 0.56 };
static double finalGear         = 3.42;
static double circumference     = 5.79;
static double convertKPH_MPH    = 0.621;
static double combinFactors     = 88;

static int8_t generatorType = -1;



static void GEN_iniParser(tParsedDataIni* pData, FILE* filePointer)
{
    int8_t i = 0;
    int8_t j = 0;
    int16_t length = 1024;
    char* buffer = (char*) malloc(sizeof(char) * length); 


    for(i = 0; i < 5; i++)
    {
        getline(&buffer, (size_t*)&length, filePointer);
        if(!memcmp(buffer, numberOfLapsBuffer, lengthNumberOfLapsBuffer))
        {
            pData->pDataIniGeneral.numberOfLaps = atoi(buffer + lengthNumberOfLapsBuffer);
        }

        if(!memcmp(buffer, startFuelBuffer, lengthStartFuelBuffer))
        {
            pData->pDataIniGeneral.startFuelRemaining = atoi(buffer + lengthStartFuelBuffer);
        }

        if(!memcmp(buffer, startCoolantTempBuffer, lengthStartCoolantTemp))
        {
            pData->pDataIniGeneral.startEngineCoolantTemperature = atoi(buffer + lengthStartCoolantTemp);
        }

        if(!memcmp(buffer, startEngineRPMBuffer, lengthStartEngineRPM))
        {
            pData->pDataIniGeneral.startEngineRPM = atoi(buffer + lengthStartEngineRPM);
        }
    }

    getline(&buffer, (size_t*)&length, filePointer); // blank line

    for(i = 0; i < pData->pDataIniGeneral.numberOfLaps; i++)
    {
        for(j = 0; j < lengthLapSection; j++)
        {
            getline(&buffer, (size_t*)&length, filePointer);
            if(!memcmp(buffer, lapLengthBuffer, lengthLapLengthBuffer))
            {
                pData->pDataIniLap[i].lengthOfLap = atoi(buffer + lengthLapLengthBuffer);
            }

            if(!memcmp(buffer, vehicleSpeedBuffer, lengthVehicleSpeedBuffer))
            {
                pData->pDataIniLap[i].vehicleSpeed = atoi(buffer + lengthVehicleSpeedBuffer);
            }

            if(!memcmp(buffer, vehicleGearBuffer, lengthVehicleGearBuffer))
            {
                pData->pDataIniLap[i].vehicleGear = atoi(buffer + lengthVehicleGearBuffer);
            }

            if(!memcmp(buffer, numberOfDTCBuffer, lengthNumberOfDTCBuffer))
            {
                pData->pDataIniLap[i].numberOfDTC = atoi(buffer + lengthNumberOfDTCBuffer);
            }
        }
        getline(&buffer, (size_t*)&length, filePointer); // blank line
    }
}

static void GEN_INIGenerator(tParsedDataIni* iniData, tGeneratorData* generatorData)
{
    int8_t i = 0;
    int8_t lapTime = 0;

    generatorData->fuelRemaining = iniData->pDataIniGeneral.startFuelRemaining;
    generatorData->engineCoolantTemperature = iniData->pDataIniGeneral.startEngineCoolantTemperature;


    generatorData->timeRunWithMILOn  = 0;
    generatorData->distanceTravelWithMILOn = 0; 

    for(i = 0; i < iniData->pDataIniGeneral.numberOfLaps; i++)
    {
        generatorData->vehicleSpeed = iniData->pDataIniLap[i].vehicleSpeed;
        generatorData->numberOfDTC = iniData->pDataIniLap[i].numberOfDTC;

        if(generatorData->numberOfDTC != 0)
        {
            generatorData->statusOfMIL = milIsON;
        }
        else
        {
            generatorData->statusOfMIL = milIsOFF;
        }

        lapTime = 
            iniData->pDataIniLap[i].lengthOfLap / (iniData->pDataIniLap[i].vehicleSpeed * convertKMHtoMS);

        while(lapTime != 0)
        {

            generatorData->engineRunTime += 1;

            if(generatorData->statusOfMIL == milIsON)
            {

                generatorData->timeRunWithMILOn += 1;
                generatorData->distanceTravelWithMILOn = 
                    generatorData->timeRunWithMILOn * (generatorData->vehicleSpeed * convertKMHtoMS);
            }
            else
            {
               generatorData->timeRunWithMILOn  = 0;
               generatorData->distanceTravelWithMILOn = 0; 
            }

            /* formula : RPM = (MPH * GEAR RATIO * FINAL GEAR * FACTORS ) / CIRCUMFERENCE */
            generatorData->engineRPM = (convertKPH_MPH * generatorData->vehicleSpeed * gearRatio[iniData->pDataIniLap[i].vehicleGear - 1]
                                         * finalGear *combinFactors) / circumference;

            if(generatorData->engineRunTime%60 == 0)
            {
                generatorData->fuelRemaining--;
            }

            callbackGeneratorFunction(generatorData);

            lapTime--;
            sleep(SIMULATOR_DELAY);
        }


    }
}

static void GEN_CvsParser(tGeneratorData* pData, FILE *filePointer)
{
    char str[LINE];
    char ignore[LINE];
    char *token;
    char *temp;
    int8_t cnt = 1;
   

    fgets(ignore, LINE, filePointer); //skip first and second line;
    fgets(ignore, LINE, filePointer);

    if(fgets(str, LINE, filePointer) != NULL)    
    {
        token = strtok(str, ","); // breaks string into tokens

        while(token != NULL)
        {       

            if(cnt == C_TEMPERATURE) // simulator dont need all values from cvs, only four
            {                                     
                pData->engineCoolantTemperature = atoi(token);

            }

            if (cnt == E_RMP)
            {   
            	pData->engineRPM = atoi(token);

            }
            
            if (cnt == E_SPEED)
            {                         
                pData->vehicleSpeed = atoi(token);

            }

            if (cnt == FUEL_L)
            {   
                pData->fuelRemaining = atoi(token);
                break;
            }

            cnt++;
            token = strtok(NULL, ",");
         }   
    } 
} 

static void* GEN_dataVehicleUpdater(void* args)
{

    tGeneratorData pdata;
    tParsedDataIni pdataIni;
    FILE* filePointer;

    if(generatorType == INI_GENERATOR)
    {

        filePointer = fopen("./data/test.ini", "r");
        if(filePointer == NULL)
        {
            printf("Invalid file pointer\n");
            return (void*)GENERATOR_ERROR;
        }

        GEN_iniParser(&pdataIni, filePointer);
        GEN_INIGenerator(&pdataIni, &pdata);
    }
    
    else
    {
        filePointer = fopen("./data/carInfoReadings1May2015.csv", "r");        
        if(filePointer == NULL)
        {
            printf("Invalid file pointer\n");
            return (void*)GENERATOR_ERROR;
        }

        while(1)
        {  
            GEN_CvsParser(&pdata, filePointer);

            pdata.numberOfDTC = -1;
            pdata.statusOfMIL = -1;
            pdata.engineRunTime = -1;
            pdata.distanceTravelWithMILOn = -1;
            pdata.timeRunWithMILOn = -1;

            callbackGeneratorFunction(&pdata);
            sleep(SIMULATOR_DELAY);
        }
    }
}

int8_t GEN_initGenerator(int32_t typeOfGenerator)
{
    if(typeOfGenerator != INI_GENERATOR && typeOfGenerator != CVS_GENERATOR)
    {
        printf("Invalid argument\n");
        return GENERATOR_ERROR;
    }
    
    generatorType = typeOfGenerator;

    if(pthread_create(&dataVehicleThread, NULL, GEN_dataVehicleUpdater, NULL) != 0)
    {
        printf("Thread for vehicle data update is not created \n");
        return GENERATOR_ERROR;
    }

    return GENERATOR_SUCCESS;
}

int8_t GEN_deInitGenerator()
{
    void* ret;
    pthread_kill(dataVehicleThread, SIGUSR1);
    if(pthread_join(dataVehicleThread, &ret) < 0)
    {
        printf("deInitGenerator fail\n");
        return GENERATOR_ERROR;
    }

    return GENERATOR_SUCCESS;
}

int8_t GEN_registerGeneratorCallback(GeneratorCallback callbackFunction)
{
    if(callbackFunction == NULL)
    {
        printf("Error in register CVSCallback \n");
        return GENERATOR_ERROR;
    }

    callbackGeneratorFunction = callbackFunction;
    return GENERATOR_SUCCESS;
}

int8_t GEN_unRegisterGeneratorCallback()
{
    callbackGeneratorFunction = NULL;
    return GENERATOR_SUCCESS;
}
