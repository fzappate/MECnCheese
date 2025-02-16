#pragma once 

#include "./axialpistonmachinebody.h"

class AxialPistonMachineBody : public ConstRotVelBody
{
public:
    AxialPistonMachineBody(std::string name, sunrealtype rotVel) : ConstRotVelBody(name, 0, 0, rotVel)
    {
        return;
    };
};