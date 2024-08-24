#pragma once

#include <string>

#include "./equation.h"
#include "./chamber.h"

// Class Orifice
class Orifice: public Equation
{
    double area;
    double Cf = 0.678;
    double flowrate;
    Chamber &upChamber;
    Chamber &downChamber;

public:

    Orifice(std::string name, double area, Chamber &upCh, Chamber &downCh);

    void SetIsDifferential(bool isDifferential) override;

    bool GetIsDifferential() override;

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

    void UpdateDepVar(double depVar) override;

    void ZeroParameters() override;
    
    void CalculateRHS() override;

    double GetRHS() override;

    double GetInitialCondition() override;
};
