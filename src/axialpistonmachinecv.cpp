#pragma once

#include <cmath> 


#include "./chamber.h"
#include "./axialpistonmachinebody.h"
#include "./axialpistonmachinecv.h"

#define pi 3.14159265358979323846

AxialPistonMachineControlVolume::AxialPistonMachineControlVolume(std::string name,
                                                                 AxialPistonMachineCasing& casing,
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
    sunrealtype deg = 2 * pi / 360;
    sunrealtype mm = 1e-3;

    // Retrieve values
    sunrealtype casingAngPos = this->casing.GetPosPsi();
    sunrealtype deadVol = this->deadVol;

    // Pump geometry
    sunrealtype swashPlateAng = 20*deg;
    sunrealtype axialPistonMachineBodyRadius = 100*mm;
    sunrealtype pistonRadius = 10*mm;

    // Calculate stroked volume
    sunrealtype halfStroke = axialPistonMachineBodyRadius*std::sin(swashPlateAng);
    sunrealtype fullStroke = 2*halfStroke;
    sunrealtype strokeFromLDP = halfStroke + axialPistonMachineBodyRadius*std::cos(casingAngPos)*std::sin(swashPlateAng);
    sunrealtype pistonArea = pi*pistonRadius*pistonRadius;
    sunrealtype pistonStrokedVol = strokeFromLDP*pistonArea;

    // Calcukate control volume volume
    sunrealtype cylinderMaxVolume = pistonArea*fullStroke + deadVol;
    sunrealtype controlVolumeVolume = cylinderMaxVolume - pistonStrokedVol;
    
    // Calculate control volume derivative 
    sunrealtype strokeDerivative = -axialPistonMachineBodyRadius*std::sin(casingAngPos)*std::cos(swashPlateAng);
    sunrealtype controlVolumeDerivative = pistonArea*strokeDerivative;


    this->volume = controlVolumeVolume;
    this->volDer = controlVolumeDerivative;

    // Calculate and store the depenedent variable in the object
    this->dpdt = bulkMod / volume * (flowSum - volDer);

    // Updatet the dependent variable in y
    *yDotValuesPnt[0] = this->dpdt;
    return;
};