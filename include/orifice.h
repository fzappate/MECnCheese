#pragma once

#include <string>

#include "./object.h"
#include "./chamber.h"

// Class Orifice
class Orifice : public NonDiffObject
{
    double area;
    double Cf = 0.678;
    double flowrate;
    Chamber &upChamber;
    Chamber &downChamber;

    struct PrintStruct
    {
        bool printArea = 1;
        bool printCf = 1;
        bool printFlowrate = 1;
    };
    PrintStruct printStruct;

public:
    Orifice(std::string name, double area, Chamber &upCh, Chamber &downCh);

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

    void PrintHeader(std::ofstream &outputFile) override;

    void PrintVariables(std::ofstream &outputFile) override;
};
