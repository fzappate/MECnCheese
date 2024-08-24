#pragma once

#include <string>

#include "./equation.h"
#include "./chamber.h"

// Class Orifice
class Orifice: public AuxEquation
{
    double area;
    double Cf = 0.678;
    double flowrate;
    Chamber &upChamber;
    Chamber &downChamber;

public:

    Orifice(std::string name, double area, Chamber &upCh, Chamber &downCh);

    Orifice(std::string name, double area, PressureSource &upCh, Chamber &downCh);

    Orifice(std::string name, double area, Chamber &upCh, PressureSource &downCh);

    void SetArea(double area);

    double GetArea();

    void SetCf(double Cf);

    double GetCf();

    void SetFlowrate(double flowrate);

    double GetFlowrate();

    void SetUpChamber(Chamber &upChamber);

    Chamber GetUpChamber();

    void SetDownChamber(Chamber &downChamber);

    Chamber GetDownChamber();

    void CalculateFlowrate();

    void UpdateChambersFlow();

    void CalculateRHS() override;
};
