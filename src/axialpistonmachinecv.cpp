#pragma once

#include <cmath> 


#include "./chamber.h"
#include "./axialpistonmachinebody.h"
#include "./axialpistonmachinecv.h"

#define pi 3.14159265358979323846

AxialPistonMachineControlVolume::AxialPistonMachineControlVolume(std::string name,
                                                                 AxialPistonMachineCasing& casing,
                                                                 AxialPistonMachineSwashPlate& swashPlate,
                                                                 AxialPistonMachinePiston& piston,
                                                                 sunrealtype casingOffset,
                                                                 sunrealtype deadVol,
                                                                 double initPressure,
                                                                 double initVolume) : VariableChamber(name, initPressure, initVolume),
                                                                                      casing(casing),
                                                                                      swashPlate(swashPlate),
                                                                                      piston(piston),
                                                                                      casingOffset(casingOffset),
                                                                                      deadVol(deadVol)
{
    return;
};

void AxialPistonMachineControlVolume::CalculateRHS()
{
    // Update volume and volume derivative before calculating the rhs of the differential equation
    this->UpdateVolumeAndVolumeDerivative();

    // Calculate and store the depenedent variable in the object
    this->dpdt = bulkMod / volume * (flowSum - volDer);

    // Updatet the dependent variable in y
    *yDotValuesPnt[0] = this->dpdt;
    return;
};

void AxialPistonMachineControlVolume::UpdateVolumeAndVolumeDerivative()
{
    // Retrieve necessary values
    sunrealtype casingAngPos = this->casing.GetPosPsi();
    sunrealtype pistonCirleRadius = this->casing.GetPistonCircleDiameter() / 2;
    sunrealtype deadVol = this->deadVol;
    sunrealtype swashPlateAng = this->swashPlate.GetSwashPlateAngle();
    sunrealtype pistonRadius = this->piston.GetPistonDiameter() / 2;

    // Calculate stroked volume
    sunrealtype halfStroke = pistonCirleRadius*std::sin(swashPlateAng);
    sunrealtype fullStroke = 2*halfStroke;
    sunrealtype strokeFromLDP = halfStroke + pistonCirleRadius*std::cos(casingAngPos)*std::sin(swashPlateAng);
    sunrealtype pistonArea = pi*pistonRadius*pistonRadius;
    sunrealtype pistonStrokedVol = strokeFromLDP*pistonArea;

    // Calculate control volume's volume
    sunrealtype cylinderMaxVolume = pistonArea*fullStroke + deadVol;
    sunrealtype controlVolumeVolume = cylinderMaxVolume - pistonStrokedVol;
    
    // Calculate control volume derivative 
    sunrealtype strokeDerivative = -pistonCirleRadius*std::sin(casingAngPos)*std::cos(swashPlateAng);
    sunrealtype controlVolumeDerivative = pistonArea*strokeDerivative;


    this->volume = controlVolumeVolume;
    this->volDer = controlVolumeDerivative;

    return;
};