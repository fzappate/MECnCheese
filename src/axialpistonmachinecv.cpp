#pragma once

#include "./chamber.h"
#include "./axialpistonmachinebody.h"
#include "./axialpistonmachinecv.h"

AxialPistonMachineControlVolume::AxialPistonMachineControlVolume(std::string name,
                                                                 AxialPistonMachineBody casing,
                                                                 sunrealtype casingOffset,
                                                                 sunrealtype deadVol,
                                                                 double initPressure,
                                                                 double initVolume) : VariableChamber(name, initPressure, initVolume),
                                                                                      casing(casing),
                                                                                      casingOffset(casingOffset),
                                                                                      deadVol(deadVol)
{
    return;
};

void AxialPistonMachineControlVolume::CalculateRHS()
{
    // Calculate and store the depenedent variable in the object
    this->dpdt = bulkMod / volume * (flowSum - volDer);

    // Updatet the dependent variable in y
    *yDotValuesPnt[0] = this->dpdt;
    return;
};