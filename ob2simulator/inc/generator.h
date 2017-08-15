#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "generator_intern_types.h"

#define GENERATOR_ERROR -1
#define GENERATOR_SUCCESS 0

#define CVS_GENERATOR 0
#define INI_GENERATOR 1


typedef struct _GeneratorData
{
	int8_t fuelRemaining;
	int8_t engineCoolantTemperature;
	int8_t vehicleSpeed;
	int16_t engineRPM;
	int8_t numberOfDTC;
	int8_t statusOfMIL;
	int16_t engineRunTime;
	int16_t distanceTravelWithMILOn;
	int16_t timeRunWithMILOn;
}tGeneratorData;


typedef void(*GeneratorCallback)(tGeneratorData*);

int8_t GEN_initGenerator(int32_t typeOfGenerator);
int8_t GEN_deInitGenerator();
int8_t GEN_registerGeneratorCallback(GeneratorCallback callbackFunction);
int8_t GEN_unRegisterGeneratorCallback();

#endif /* __GENERATOR_H__*/