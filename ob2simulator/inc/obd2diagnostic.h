#ifndef __ODB2DIAGNOSTIC_H__
#define __ODB2DIAGNOSTIC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "generator.h"
#include "pids_codes.h"

typedef struct _OBD2VehicleData
{
	int8_t vehicleSpeed;
    int16_t engineCoolantTemperature;
    int16_t engineRPM;
    int8_t fuelRemaining;
    int8_t numberOfDTC;
	int8_t statusOfMIL;
	int16_t engineRunTime;
	int16_t distanceTravelWithMILOn;
	int16_t timeRunWithMILOn;
}tOBD2VehicleData;

void OBD2_getDiagnostic(char* commandBuffer, char* dataBuffer);
void OBD2_setVehileData(tOBD2VehicleData* obdData);

#endif /* __ODB2DIAGNOSTIC_H__ */