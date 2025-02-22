#pragma once

#include "./axialpistonmachinebody.h"

// Axial Piston Machine Casing
AxialPistonMachineCasing::AxialPistonMachineCasing(std::string name, sunrealtype rotVel, sunrealtype pistonCircleDiameter) : ConstRotVelBody(name, 0, 0, rotVel), pistonCircleDiameter(pistonCircleDiameter)
{
    return;
};

sunrealtype AxialPistonMachineCasing::GetPistonCircleDiameter()
{
    return this->pistonCircleDiameter;
};

// Axial Piston Machine Swash Plate
AxialPistonMachineSwashPlate::AxialPistonMachineSwashPlate(std::string name, sunrealtype swashPlateAng) : Body(name, 1), swashPlateAngle(swashPlateAng)
{
    return;
};

sunrealtype AxialPistonMachineSwashPlate::GetSwashPlateAngle()
{
    return this->swashPlateAngle;
};

// Axial Piston Machine Piston
AxialPistonMachinePiston::AxialPistonMachinePiston(std::string name, sunrealtype pistonDiameter) : Body(name, 1), pistonDiameter(pistonDiameter), pistonArea(3.14159265358979323846 * (pistonDiameter/2)*(pistonDiameter/2))
{
    return;
};

sunrealtype AxialPistonMachinePiston::GetPistonDiameter()
{
    return this->pistonDiameter;
};

sunrealtype AxialPistonMachinePiston::GetPistonArea()
{
    return this->pistonArea;
};