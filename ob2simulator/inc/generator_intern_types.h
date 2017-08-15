#ifndef __GENERATOR_INTERN_TYPES_H__
#define __GENERATOR_INTERN_TYPES_H__

#include <stdio.h>

#define MAX_LAP_NUMBER 10

typedef struct _ParsedDataIniGeneral
{
	int8_t numberOfLaps;
	int8_t startFuelRemaining;
	int8_t startEngineCoolantTemperature;
	int16_t startEngineRPM;

}tParsedDataIniGeneral;

typedef struct _parsedDataIniLap
{
	int16_t lengthOfLap;
	int8_t vehicleSpeed;
	int8_t vehicleGear;
	int8_t numberOfDTC;

}tParsedDataIniLap;

typedef struct _parsedDataIni
{
	tParsedDataIniGeneral pDataIniGeneral;
	tParsedDataIniLap pDataIniLap[MAX_LAP_NUMBER];

}tParsedDataIni;

#endif /* __GENERATOR_INTERN_TYPES_H__ */