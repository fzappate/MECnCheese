#pragma once

#include "./body.h"

class AxialPistonMachineCasing : public ConstRotVelBody
{
public:
    AxialPistonMachineCasing(std::string name, sunrealtype rotVel);
};

class AxialPistonMachineSwashPlate : public Body
{
    sunrealtype swashPlateAngle = 0;
public:
    AxialPistonMachineSwashPlate(std::string name, sunrealtype swashPlateAngle);

    sunrealtype GetSwashPlateAngle();
};