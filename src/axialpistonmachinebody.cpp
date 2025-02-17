#pragma once

#include "./axialpistonmachinebody.h"

AxialPistonMachineCasing::AxialPistonMachineCasing(std::string name, sunrealtype rotVel) : ConstRotVelBody(name, 0, 0, rotVel)
{
    return;
};


AxialPistonMachineSwashPlate::AxialPistonMachineSwashPlate(std::string name, sunrealtype swashPlateAng) : Body(name, 1), swashPlateAngle(swashPlateAng)
{
    return;
};

sunrealtype AxialPistonMachineSwashPlate::GetSwashPlateAngle()
{
    return this->swashPlateAngle;
};