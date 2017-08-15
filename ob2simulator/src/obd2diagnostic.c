#include "obd2diagnostic.h"

#define MODE01 1
#define MODE02 2

static char mode01Command[] = "01";
static char mode02Command[] = "02";
static char mode03Command[] = "03";

static tOBD2VehicleData OBD2_VehicleData;
static pthread_mutex_t OBD2_mutex;

static void OBD2_Mode01(char* commandBuffer, char* dataBuffer)
{
	char command[3];
	command[0] = commandBuffer[3];
	command[1] = commandBuffer[4];
	command[2] = '\0';

	if(!strcmp(command, PIDS_SUPPORT_00_20))
  {
      PID_pidsSupport0020(dataBuffer, MODE01); 
  }
  else if(!strcmp(command, DIAGNOSTIC_TROUBLE_CODE))
  {
      pthread_mutex_lock(&OBD2_mutex);
      PID_diagnosticTroubleCode(dataBuffer, &OBD2_VehicleData, MODE01);
      pthread_mutex_unlock(&OBD2_mutex);
  }    
  else if(!strcmp(command, FUEL_SYSTEM_STATUS))
  {
      PID_fuelSystemStatus(dataBuffer, MODE01); 
  }    
  else if(!strcmp(command, ENGINE_LOAD))
  {
     PID_engineLoad(dataBuffer, MODE01);
  }
  else if(!strcmp(command, ENGINE_COOLANT_TEMP))
  {
     pthread_mutex_lock(&OBD2_mutex);
     PID_engineCoolantTemp(dataBuffer, &OBD2_VehicleData, MODE01);
     pthread_mutex_unlock(&OBD2_mutex);  
  }
  else if(!strcmp(command, ST_FUEL_TRIM_B13))
  {
     PID_stFuelTrimB13(dataBuffer, MODE01); 
  }
  else if(!strcmp(command, LT_FUEL_TRIM_B13))
  {
     PID_ltFuelTrimB13(dataBuffer, MODE01);  
  }
  else if(!strcmp(command, ST_FUEL_TRIM_B24))
  {
      PID_stFuelTrimB24(dataBuffer, MODE01);  
  }
  else if(!strcmp(command, LT_FUEL_TRIM_B24))
  {
     PID_ltFuelTrimB24(dataBuffer, MODE01); 
  }
  else if(!strcmp(command, FUEL_PRESSURE))
  {
     PID_fuelPressuere(dataBuffer, MODE01);  
  }
  else if(!strcmp(command, INTAKE_MANIFOLD_PRESSURE))
  {
     PID_intakeManifoldPressure(dataBuffer, MODE01); 
  }
  else if(!strcmp(command, ENGINE_RPM))
  {
     pthread_mutex_lock(&OBD2_mutex);
     PID_engineRPM(dataBuffer, &OBD2_VehicleData, MODE01);
     pthread_mutex_unlock(&OBD2_mutex);
  }
  else if(!strcmp(command, SPEED))
  {
     pthread_mutex_lock(&OBD2_mutex);
     PID_speedVehicle(dataBuffer, &OBD2_VehicleData, MODE01);
     pthread_mutex_unlock(&OBD2_mutex);  
  }
  else if(!strcmp(command, TIMING_ADVANCE))
  {
     PID_timingAdvance(dataBuffer, MODE01);
  }
  else if(!strcmp(command, AIR_INTAKE_TEMP))
  {
     PID_airIntakeTemp(dataBuffer, MODE01);
  }
  else if(!strcmp(command, MAF))
  {
     PID_maf(dataBuffer, MODE01);
  }
  else if(!strcmp(command, THROTTLE_POS))
  {
     PID_throttlePos(dataBuffer, MODE01); 
  }
  else if(!strcmp(command, SECONDARY_AIR_STATUS))
  {
     PID_secondaryAirStatus(dataBuffer, MODE01);
  }
  else if(!strcmp(command, ENGINE_RUNTIME))
  {
     pthread_mutex_lock(&OBD2_mutex);
     PID_engineRuntime(dataBuffer, &OBD2_VehicleData, MODE01);
     pthread_mutex_unlock(&OBD2_mutex);
  }
  else if(!strcmp(command, PIDS_SUPPORT_21_40))
  {
      PID_pidsSupport2140(dataBuffer, MODE01);
  }
  else if(!strcmp(command, DISTANCE_TRAVELED_WITH_MIL_ON))
  {
    pthread_mutex_lock(&OBD2_mutex);
    PID_distanceTraveledWithMilOn(dataBuffer, &OBD2_VehicleData, MODE01);
    pthread_mutex_unlock(&OBD2_mutex);  
  }
  else if(!strcmp(command, FUEL_LEVEL))
  {
     pthread_mutex_lock(&OBD2_mutex);
     PID_fuelLevel(dataBuffer, &OBD2_VehicleData, MODE01);
     pthread_mutex_unlock(&OBD2_mutex); 
  }
  else if(!strcmp(command, BAROMETRIC_PRESSURE))
  {
     PID_barometricPressure(dataBuffer, MODE01); 
  }
  else if(!strcmp(command, PIDS_SUPPORT_41_60))
  {
      PID_pidsSupport4160(dataBuffer, MODE01); 
  }
  else if(!strcmp(command, AIR_FUEL_RATIO))
  {
     PID_airFuelRatio(dataBuffer, MODE01); 
  }
  else if(!strcmp(command, AMBIENT_AIR_TEMP))
  {
     PID_ambientAirTemp(dataBuffer, MODE01);
  }
  else if(!strcmp(command, TIME_RUN_WITH_MIL_ON))
  {
     pthread_mutex_lock(&OBD2_mutex);
     PID_timeRunWithMilOn(dataBuffer, &OBD2_VehicleData, MODE01);
     pthread_mutex_unlock(&OBD2_mutex); 
  }
  else if(!strcmp(command, FUEL_TYPE))
  {
     PID_fuelType(dataBuffer, MODE01);  
  }
  else if(!strcmp(command, ENGINE_OIL_TEMPERATURE))
  {
     PID_engineOilTemperature(dataBuffer, MODE01); 
  }
  else
  {
     printf("Command is not supported.\n");
  }

}

static void OBD2_Mode02(char* commandBuffer, char* dataBuffer)
{
	char command[3];
  command[0] = commandBuffer[3];
  command[1] = commandBuffer[4];
  command[2] = '\0';

  if(!strcmp(command, PIDS_SUPPORT_00_20))
  {
      PID_pidsSupport0020(dataBuffer, MODE02); 
  }
  else if(!strcmp(command, DIAGNOSTIC_TROUBLE_CODE))
  {
      pthread_mutex_lock(&OBD2_mutex);
      PID_diagnosticTroubleCode(dataBuffer, &OBD2_VehicleData, MODE02);
      pthread_mutex_unlock(&OBD2_mutex);
  }
  else if(!strcmp(command, DTC_FREEZE_FRAME))
  {
      pthread_mutex_lock(&OBD2_mutex);
      PID_dtcFrizeFrame(dataBuffer, &OBD2_VehicleData);
      pthread_mutex_unlock(&OBD2_mutex);
  }  
  else if(!strcmp(command, FUEL_SYSTEM_STATUS))
  {
      PID_fuelSystemStatus(dataBuffer, MODE02); 
  }    
  else if(!strcmp(command, ENGINE_LOAD))
  {
     PID_engineLoad(dataBuffer, MODE02);
  }
  else if(!strcmp(command, ENGINE_COOLANT_TEMP))
  {
     pthread_mutex_lock(&OBD2_mutex);
     PID_engineCoolantTemp(dataBuffer, &OBD2_VehicleData, MODE02);
     pthread_mutex_unlock(&OBD2_mutex);  
  }
  else if(!strcmp(command, ST_FUEL_TRIM_B13))
  {
     PID_stFuelTrimB13(dataBuffer, MODE02); 
  }
  else if(!strcmp(command, LT_FUEL_TRIM_B13))
  {
     PID_ltFuelTrimB13(dataBuffer, MODE02);  
  }
  else if(!strcmp(command, ST_FUEL_TRIM_B24))
  {
      PID_stFuelTrimB24(dataBuffer, MODE02);  
  }
  else if(!strcmp(command, LT_FUEL_TRIM_B24))
  {
     PID_ltFuelTrimB24(dataBuffer, MODE02); 
  }
  else if(!strcmp(command, FUEL_PRESSURE))
  {
     PID_fuelPressuere(dataBuffer, MODE02);  
  }
  else if(!strcmp(command, INTAKE_MANIFOLD_PRESSURE))
  {
     PID_intakeManifoldPressure(dataBuffer, MODE02); 
  }
  else if(!strcmp(command, ENGINE_RPM))
  {
     pthread_mutex_lock(&OBD2_mutex);
     PID_engineRPM(dataBuffer, &OBD2_VehicleData, MODE02);
     pthread_mutex_unlock(&OBD2_mutex);
  }
  else if(!strcmp(command, SPEED))
  {
     pthread_mutex_lock(&OBD2_mutex);
     PID_speedVehicle(dataBuffer, &OBD2_VehicleData, MODE02);
     pthread_mutex_unlock(&OBD2_mutex);  
  }
  else if(!strcmp(command, TIMING_ADVANCE))
  {
     PID_timingAdvance(dataBuffer, MODE02);
  }
  else if(!strcmp(command, AIR_INTAKE_TEMP))
  {
     PID_airIntakeTemp(dataBuffer, MODE02);
  }
  else if(!strcmp(command, MAF))
  {
     PID_maf(dataBuffer, MODE02);
  }
  else if(!strcmp(command, THROTTLE_POS))
  {
     PID_throttlePos(dataBuffer, MODE02); 
  }
  else if(!strcmp(command, SECONDARY_AIR_STATUS))
  {
     PID_secondaryAirStatus(dataBuffer, MODE02);
  }
  else if(!strcmp(command, ENGINE_RUNTIME))
  {
     pthread_mutex_lock(&OBD2_mutex);
     PID_engineRuntime(dataBuffer, &OBD2_VehicleData, MODE02);
     pthread_mutex_unlock(&OBD2_mutex);
  }
  else if(!strcmp(command, PIDS_SUPPORT_21_40))
  {
      PID_pidsSupport2140(dataBuffer, MODE02);
  }
  else if(!strcmp(command, DISTANCE_TRAVELED_WITH_MIL_ON))
  {
    pthread_mutex_lock(&OBD2_mutex);
    PID_distanceTraveledWithMilOn(dataBuffer, &OBD2_VehicleData, MODE02);
    pthread_mutex_unlock(&OBD2_mutex);  
  }
  else if(!strcmp(command, FUEL_LEVEL))
  {
     pthread_mutex_lock(&OBD2_mutex);
     PID_fuelLevel(dataBuffer, &OBD2_VehicleData, MODE02);
     pthread_mutex_unlock(&OBD2_mutex); 
  }
  else if(!strcmp(command, BAROMETRIC_PRESSURE))
  {
     PID_barometricPressure(dataBuffer, MODE02); 
  }
  else if(!strcmp(command, PIDS_SUPPORT_41_60))
  {
      PID_pidsSupport4160(dataBuffer, MODE02); 
  }
  else if(!strcmp(command, AIR_FUEL_RATIO))
  {
     PID_airFuelRatio(dataBuffer, MODE02); 
  }
  else if(!strcmp(command, AMBIENT_AIR_TEMP))
  {
     PID_ambientAirTemp(dataBuffer, MODE02);
  }
  else if(!strcmp(command, TIME_RUN_WITH_MIL_ON))
  {
     pthread_mutex_lock(&OBD2_mutex);
     PID_timeRunWithMilOn(dataBuffer, &OBD2_VehicleData, MODE02);
     pthread_mutex_unlock(&OBD2_mutex); 
  }
  else if(!strcmp(command, FUEL_TYPE))
  {
     PID_fuelType(dataBuffer, MODE02);  
  }
  else if(!strcmp(command, ENGINE_OIL_TEMPERATURE))
  {
     PID_engineOilTemperature(dataBuffer, MODE02); 
  }
  else
  {
     printf("Command is not supported.\n");
  }
}

static void OBD2_Mode03(char* commandBuffer, char* dataBuffer)
{
    if(!OBD2_VehicleData.statusOfMIL)
    {
        sprintf(dataBuffer, "03 00 00 >");
    }
    else
    {
        if(OBD2_VehicleData.numberOfDTC == 1)
        {
            sprintf(dataBuffer, "03 11 11 00 00 >");
        }
        else
        {
            sprintf(dataBuffer, "03 11 11 11 22 00 00 >");
        }

    }
}
    
void OBD2_getDiagnostic(char* commandBuffer, char* dataBuffer)
{
  pthread_mutex_init(&OBD2_mutex, NULL);
  
	if(!memcmp(commandBuffer, mode01Command, 2))
	{
		OBD2_Mode01(commandBuffer, dataBuffer);
	}

	else if(!memcmp(commandBuffer, mode02Command, 2))
	{
		OBD2_Mode02(commandBuffer, dataBuffer);
	}

	else if(!memcmp(commandBuffer, mode03Command, 2))
	{
		OBD2_Mode03(commandBuffer, dataBuffer);
	}

	else
	{
		printf("This mode is not supported \n");
	}
}

void OBD2_setVehileData(tOBD2VehicleData* obdData)
{
    pthread_mutex_lock(&OBD2_mutex);
    OBD2_VehicleData = *obdData;
    pthread_mutex_unlock(&OBD2_mutex);
}