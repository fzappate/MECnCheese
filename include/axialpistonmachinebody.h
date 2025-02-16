#pragma once

#include "./body.h"

class AxialPistonMachineBody : public ConstRotVelBody
{
public:
    AxialPistonMachineBody(std::string name, sunrealtype rotVel);
};