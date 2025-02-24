#pragma once

#include <string>

#include "./object.h"
#include "./chamber.h"

// Class Orifice
class Orifice : public NonDiffObject
{
    sunrealtype area;
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
    Orifice(std::string name, Chamber &upCh, Chamber &downCh);

    Orifice(std::string name, double area, Chamber &upCh, Chamber &downCh);

    void CalculateFlowrate();

    void UpdateChambersFlow();

    void SetArea(sunrealtype area);

    void CalculateRHS() override;

    void PrintHeader(std::ofstream &outputFile) override;

    void PrintVariables(std::ofstream &outputFile) override;
};

