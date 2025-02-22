#pragma once

#include "./body.h"

class AxialPistonMachineCasing : public ConstRotVelBody
{
    sunrealtype pistonCircleDiameter;
public:
    AxialPistonMachineCasing(std::string name, sunrealtype rotVel, sunrealtype pistonCircleDiameter);

    sunrealtype GetPistonCircleDiameter();
};

class AxialPistonMachineSwashPlate : public Body
{
    sunrealtype swashPlateAngle;
public:
    AxialPistonMachineSwashPlate(std::string name, sunrealtype swashPlateAngle);

    sunrealtype GetSwashPlateAngle();
};

class AxialPistonMachinePiston : public Body
{
    sunrealtype pistonDiameter;
    sunrealtype pistonArea;

    public:
    AxialPistonMachinePiston(std::string name, sunrealtype pistonDiameter);

    sunrealtype GetPistonDiameter();

    sunrealtype GetPistonArea();
};