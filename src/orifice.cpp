#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <corecrt_math_defines.h>

#include "./object.h"
#include "./orifice.h"
#include "./chamber.h"

// Constructors
Orifice::Orifice(std::string name, Chamber &upCh, Chamber &downCh) : NonDiffObject(name), upChamber(upCh), downChamber(downCh)
{
    return;
};

Orifice::Orifice(std::string name, double area, Chamber &upCh, Chamber &downCh) : NonDiffObject(name), area(area), upChamber(upCh), downChamber(downCh)
{
    return;
};

void Orifice::CalculateFlowrate()
{
    double upPress = this->upChamber.GetPressure();
    double downPress = this->downChamber.GetPressure();

    double isDpNeg = signbit(-upPress + downPress);
    int sign;
    if (isDpNeg == 0)
    {
        sign = -1;
    }
    else
    {
        sign = 1;
    }

    flowrate = sign * this->Cf * this->area * sqrt(abs(upPress - downPress));

    UpdateChambersFlow();
    return;
};

void Orifice::UpdateChambersFlow()
{
    // Flow entering a chamber is negative
    upChamber.AddFlowIn(name, -flowrate);
    downChamber.AddFlowIn(name, flowrate);
    return;
};

void Orifice::SetArea(sunrealtype area)
{
    this->area = area;
};

void Orifice::CalculateRHS()
{
    // Calculate and update the flowrate
    this->CalculateFlowrate();

    return;
};

void Orifice::PrintHeader(std::ofstream &outputFile)
{
    if (printStruct.printCf == 1)
    {
        outputFile << name << ":Cf:-, ";
    }
    if (printStruct.printArea == 1)
    {
        outputFile << name << ":Area:m^2, ";
    }
    if (printStruct.printFlowrate == 1)
    {
        outputFile << name << ":Flowrate:m^3/s, ";
    }

    return;
}

void Orifice::PrintVariables(std::ofstream &outputFile)
{
    if (printStruct.printCf == 1)
    {
        outputFile << Cf << ",";
    }
    if (printStruct.printArea == 1)
    {
        outputFile << area << ",";
    }
    if (printStruct.printFlowrate == 1)
    {
        outputFile << flowrate << ",";
    }

    return;
}