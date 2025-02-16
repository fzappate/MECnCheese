#pragma once

#include "./chamber.h"

class AxialPistonMachineControlVolume : public VariableChamber
{
public:
    AxialPistonMachineControlVolume(std::string name, double initPressure, double initVolume) : VariableChamber(name, initPressure, initVolume)
    {
        return;
    };

    void CalculateRHS() override
    {
        // Calculate and store the depenedent variable in the object
        this->dpdt = bulkMod / volume * (flowSum - volDer);

        // Updatet the dependent variable in y
        *yDotValuesPnt[0] = this->dpdt;
        return;
    };
};