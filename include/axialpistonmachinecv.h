#pragma once

#include "./chamber.h"

class AxialPistonMachineControlVolume : public VariableChamber
{
    AxialPistonMachineCasing& casing;
    AxialPistonMachineSwashPlate& swashPlate;
    AxialPistonMachinePiston& piston;
    sunrealtype casingOffset;
    sunrealtype deadVol;

public:
    AxialPistonMachineControlVolume(std::string name,
                                    AxialPistonMachineCasing& casing,
                                    AxialPistonMachineSwashPlate& swashPlate,
                                    AxialPistonMachinePiston& piston,
                                    sunrealtype casingOffset,
                                    sunrealtype deadVol,
                                    double initPressure,
                                    double initVolume);

    void CalculateRHS() override;
};