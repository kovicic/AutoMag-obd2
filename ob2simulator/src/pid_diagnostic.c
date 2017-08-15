#include "pid_diagnostic.h"

void PID_pidsSupport0020(char* dataBuffer, int8_t mode)
{
    if(mode == 1)
    {
        sprintf(dataBuffer, "01 00 FF FF C0 03 >");
    }
    else
    {
        sprintf(dataBuffer, "02 00 00 FF FF C0 03 >");
    }
}

void PID_pidsSupport2140(char* dataBuffer, int8_t mode)
{

    if(mode == 1)
    {
        sprintf(dataBuffer, "01 20 80 02 20 01 >");
    }
    else
    {
         sprintf(dataBuffer, "02 00 20 80 02 20 01 >");
    }
}
void PID_pidsSupport4160(char* dataBuffer, int8_t mode)
{
    if(mode == 1)
    {
        sprintf(dataBuffer, "01 40 14 08 80 10 >");  
    }
    else
    {
        sprintf(dataBuffer, "02 00 40 14 08 80 10 >");
    }
    
}

void PID_diagnosticTroubleCode(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode)
{   
    if(obd2Data->statusOfMIL)
    {
        if(obd2Data->numberOfDTC == 1)
        {
            sprintf(dataBuffer, "0%d 01 81 00 00 00 >", mode);
        }
        else if (obd2Data->numberOfDTC == 2)
        {
            sprintf(dataBuffer, "0%d 01 82 00 00 00 >", mode);
        }
        else
        {
            sprintf(dataBuffer, "0%d 01 83 00 00 00 >", mode);
        }
    }
    else
    {
        sprintf(dataBuffer, "0%d 01 00 00 00 00 >", mode);
    }
}
void PID_dtcFrizeFrame(char* dataBuffer, tOBD2VehicleData* obd2Data)
{
    if(!obd2Data->statusOfMIL)
    {
        sprintf(dataBuffer, "02 02 00 00 >");
    }
    else
    {
        sprintf(dataBuffer, "02 02 11 11 >");
    }
} 
void PID_fuelSystemStatus(char* dataBuffer, int8_t mode)
{   
    sprintf(dataBuffer, "0%d 03 22 >", mode);
}

void PID_engineLoad(char* dataBuffer, int8_t mode)
{      
    /* 50% is answer */
    sprintf(dataBuffer, "0%d 04 80 >", mode);
}

void PID_engineCoolantTemp(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode)
{   
     sprintf(dataBuffer, "0%d 05 %X >", mode, obd2Data->engineCoolantTemperature + 40);  
} 
void PID_stFuelTrimB13(char* dataBuffer, int8_t mode)
{
    /* ~2% is answer, */
    sprintf(dataBuffer, "0%d 06 83 >", mode);
}

void PID_ltFuelTrimB13(char* dataBuffer, int8_t mode)
{
    /* ~3% is answer */
    sprintf(dataBuffer, "0%d 07 84 >", mode);  
} 

void PID_stFuelTrimB24(char* dataBuffer, int8_t mode)
{
    /* ~2% is answer */
    sprintf(dataBuffer, "0%d 08 83 >", mode);
} 

void PID_ltFuelTrimB24(char* dataBuffer, int8_t mode)
{   
    /* ~4% is answer */
    sprintf(dataBuffer, "0%d 09 86 >", mode);  
}

void PID_fuelPressuere(char* dataBuffer, int8_t mode)
{     
    /* 378 kPa is answer */
    sprintf(dataBuffer, "0%d 0A 7E >", mode); 
} 

void PID_intakeManifoldPressure(char* dataBuffer, int8_t mode)
{
    /* 50 kPa is answer */
    sprintf(dataBuffer, "0%d 0B 32 >", mode);  
} 
void PID_engineRPM(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode)
{
        char rpmHexaBuffer[5];

        sprintf(rpmHexaBuffer, "%X", 4 * obd2Data->engineRPM);

        if(4 * obd2Data->engineRPM < 4096)
        {
            sprintf(dataBuffer, "0%d 0C 0%c %c%c >", mode, rpmHexaBuffer[0], rpmHexaBuffer[1], rpmHexaBuffer[2]);
        }
        else
        {
            sprintf(dataBuffer, "0%d 0C %c%c %c%c >", mode, rpmHexaBuffer[0], rpmHexaBuffer[1], rpmHexaBuffer[2], rpmHexaBuffer[3]);
        }
}

void PID_speedVehicle(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode)
{   
    if(obd2Data->vehicleSpeed < 16)
    {
        sprintf(dataBuffer, "0%d 0D 0%X >", mode, obd2Data->vehicleSpeed);
    }
    else
    {
        sprintf(dataBuffer, "0%d 0D %X >", mode, obd2Data->vehicleSpeed);
    }
}

void PID_timingAdvance(char* dataBuffer, int8_t mode)
{
    /* 30 is answer */
    sprintf(dataBuffer, "0%d 0E 9E >", mode);
} 

void PID_airIntakeTemp(char* dataBuffer, int8_t mode)
{ 
    /*answer is 25 ^C */
    sprintf(dataBuffer, "0%d 0F 41 >", mode);  
} 

void PID_maf(char* dataBuffer, int8_t mode)
{
    /*answer is 160 g/sec */
    sprintf(dataBuffer, "0%d 10 3E 80 >", mode);
}

void PID_throttlePos(char* dataBuffer, int8_t mode)
{
    /*20% is answer */
    sprintf(dataBuffer, "0%d 11 33 >", mode);
    
} 
void PID_secondaryAirStatus(char* dataBuffer, int8_t mode)
{

    /* From the outside atmosphere or off */
    sprintf(dataBuffer, "0%d 12 04 >", mode);
} 

void PID_engineRuntime(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode)
{   
    
    if(obd2Data->engineRunTime < 16)
    {
        sprintf(dataBuffer, "0%d 1F 00 0%X >", mode, obd2Data->engineRunTime);
    }
    else if(obd2Data->engineRunTime < 256)
    {
        sprintf(dataBuffer, "0%d 1F 00 %X >", mode, obd2Data->engineRunTime);
    }
    else if(obd2Data->engineRunTime < 4096)
    {
        sprintf(dataBuffer, "0%d 1F 0%X >", mode, obd2Data->engineRunTime);
    }
    else
    {
        sprintf(dataBuffer, "0%d 1F %X >", mode, obd2Data->engineRunTime);
    }
    
} 

void PID_distanceTraveledWithMilOn(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode)
{   

    if(!obd2Data->statusOfMIL)
    {
        /* 0km */
        sprintf(dataBuffer, "0%d 21 00 00 >", mode);
    }
    else
    {
        if(obd2Data->distanceTravelWithMILOn < 16)
        {
            sprintf(dataBuffer, "0%d 21 00 0%X >", mode, obd2Data->distanceTravelWithMILOn);
        }
        else if(obd2Data->distanceTravelWithMILOn < 256)
        {
            sprintf(dataBuffer, "0%d 21 00 %X >", mode, obd2Data->distanceTravelWithMILOn);
        }
        else if(obd2Data->distanceTravelWithMILOn > 4096)
        {
            sprintf(dataBuffer, "0%d 21 0%X >", mode, obd2Data->distanceTravelWithMILOn);
        }
        else
        {
            sprintf(dataBuffer, "0%d 21 %X >", mode, obd2Data->distanceTravelWithMILOn);
        }
    }

}

void PID_fuelLevel(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode)
{   
    int fuelLevel = obd2Data->fuelRemaining;
    fuelLevel *= 255;
    fuelLevel /= 100;

    if(obd2Data->fuelRemaining < 16)
    {
        sprintf(dataBuffer, "0%d 2F 0%X >", mode, fuelLevel);
    }
    else
    {
        sprintf(dataBuffer, "0%d 2F %X >", mode, fuelLevel);
    }    
} 

void PID_barometricPressure(char* dataBuffer, int8_t mode)
{
    /*160 kPa*/
    sprintf(dataBuffer, "0%d 33 A0 >", mode); 
} 
 
void PID_airFuelRatio(char* dataBuffer, int8_t mode)
{
    /* ~1 ratio */
    sprintf(dataBuffer, "0%d 44 80 00 >", mode);  
} 

void PID_ambientAirTemp(char* dataBuffer, int8_t mode)
{
    /* 25^C */
    sprintf(dataBuffer, "0%d 46 41 >", mode);  
} 

void PID_timeRunWithMilOn(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode)
{
    if(obd2Data->timeRunWithMILOn < 16)
    {
        sprintf(dataBuffer, "0%d 4D 00 0%X >", mode, obd2Data->timeRunWithMILOn);
    }
    else if(obd2Data->timeRunWithMILOn < 256)
    {
        sprintf(dataBuffer, "0%d 4D 00 %X >", mode, obd2Data->timeRunWithMILOn);
    }
    else if(obd2Data->timeRunWithMILOn < 4096)
    {
        sprintf(dataBuffer, "0%d 4D 0%X >", mode, obd2Data->timeRunWithMILOn);
    }
    else
    {
        sprintf(dataBuffer, "0%d 1F %X >", mode, obd2Data->timeRunWithMILOn);
    }
} 


void PID_fuelType(char* dataBuffer, int8_t mode)
{
    /* Disel */
    sprintf(dataBuffer, "0%d 51 04 >", mode);  
}

void PID_engineOilTemperature(char* dataBuffer, int8_t mode)
{
    /* 110 ^C */
    sprintf(dataBuffer, "0%d 5C 96 >", mode); 
} 


