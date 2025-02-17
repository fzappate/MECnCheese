#pragma once

#include "./chamber.h"

class AxialPistonMachineControlVolume : public VariableChamber
{
    AxialPistonMachineCasing& casing;
    sunrealtype casingOffset;
    sunrealtype deadVol;

public:
    AxialPistonMachineControlVolume(std::string name,
                                    AxialPistonMachineCasing& casing,
                                    sunrealtype casingOffset,
                                    sunrealtype deadVol,
                                    double initPressure,
                                    double initVolume);

    void CalculateRHS() override;
};