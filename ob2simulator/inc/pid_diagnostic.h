#ifndef __PID_DIAGNOSTIC_H__
#define __PID_DIAGNOSTIC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obd2diagnostic.h"

void PID_pidsSupport0020(char* dataBuffer, int8_t mode);
void PID_pidsSupport2140(char* dataBuffer, int8_t mode);
void PID_pidsSupport4160(char* dataBuffer, int8_t mode);
void PID_diagnosticTroubleCode(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode);
void PID_dtcFrizeFrame(char* dataBuffer, tOBD2VehicleData* obd2Data);
void PID_fuelSystemStatus(char* dataBuffer, int8_t mode);
void PID_engineLoad(char* dataBuffer, int8_t mode);
void PID_engineCoolantTemp(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode);
void PID_stFuelTrimB13(char* dataBuffer, int8_t mode);
void PID_ltFuelTrimB13(char* dataBuffer, int8_t mode);
void PID_stFuelTrimB24(char* dataBuffer, int8_t mode);
void PID_ltFuelTrimB24(char* dataBuffer, int8_t mode);
void PID_fuel_pressuere(char* dataBuffer, int8_t mode);
void PID_intakeManifoldPressure(char* dataBuffer, int8_t mode);
void PID_engineRPM(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode);
void PID_speedVehicle(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode);
void PID_timingAdvance(char* dataBuffer, int8_t mode);
void PID_airIntakeTemp(char* dataBuffer, int8_t mode);
void PID_maf(char* dataBuffer, int8_t mode);
void PID_throttlePos(char* dataBuffer, int8_t mode);
void PID_secondaryAirStatus(char* dataBuffer, int8_t mode);
void PID_engineRuntime(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode);
void PID_distanceTraveledWithMilOn(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode);
void PID_fuelLevel(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode);
void PID_barometricPressure(char* dataBuffer, int8_t mode);
void PID_airFuelRatio(char* dataBuffer, int8_t mode);
void PID_ambientAirTemp(char* dataBuffer, int8_t mode);
void PID_timeRunWithMilOn(char* dataBuffer, tOBD2VehicleData* obd2Data, int8_t mode);
void PID_fuelType(char* dataBuffer, int8_t mode);
void PID_engineOilTemperature(char* dataBuffer, int8_t mode);

#endif /*__PID_DIAGNOSTIC_H__*/