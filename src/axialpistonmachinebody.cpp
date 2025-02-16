#pragma once

#include "./axialpistonmachinebody.h"

AxialPistonMachineBody::AxialPistonMachineBody(std::string name, sunrealtype rotVel) : ConstRotVelBody(name, 0, 0, rotVel)
{
    return;
};
