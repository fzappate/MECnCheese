#pragma once

#include "./chamber.h"

class AxialPistonMachineControlVolume : public VariableChamber
{
public:
    AxialPistonMachineControlVolume(std::string name, double initPressure, double initVolume);

    void CalculateRHS() override;
};