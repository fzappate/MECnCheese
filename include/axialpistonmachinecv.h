#pragma once

#include "./chamber.h"

class AxialPistonMachineControlVolume : public VariableChamber
{
    AxialPistonMachineBody& casing;
    sunrealtype casingOffset;
    sunrealtype deadVol;

public:
    AxialPistonMachineControlVolume(std::string name,
                                    AxialPistonMachineBody& casing,
                                    sunrealtype casingOffset,
                                    sunrealtype deadVol,
                                    double initPressure,
                                    double initVolume);

    void CalculateRHS() override;
};