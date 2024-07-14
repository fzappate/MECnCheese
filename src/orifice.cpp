#include <math.h>
#include <string>
#include <iostream>
#include <corecrt_math_defines.h>

#include "./equation.h"
#include "./orifice.h"
#include "./chamber.h"

// Constructors
Orifice::Orifice(std::string name, double area, Chamber &upCh, Chamber &downCh) : Equation(name), area(area), upChamber(upCh), downChamber(downCh)
{
    
    isDifferential = false;

    CalculateFlowrate();

    initCond = flowrate;

    return;
};

void Orifice::SetIsDifferential(bool isDifferential)
{
    this->isDifferential = false;
    return;
}

bool Orifice::GetIsDifferential()
{
    return this->isDifferential;
}

void Orifice::SetArea(double area)
{
    this->area = area;
    return;
};

double Orifice::GetArea()
{
    return this->area;
};

void Orifice::SetCf(double Cf)
{
    this->Cf = Cf;
    return;
};

double Orifice::GetCf()
{
    return this->Cf;
};

void Orifice::SetFlowrate(double flowrate)
{
    this->flowrate = flowrate;
    return;
};

double Orifice::GetFlowrate()
{
    return this->flowrate;
};

void Orifice::SetUpChamber(Chamber &upChamber)
{
    this->upChamber = upChamber;
    return;
};

Chamber Orifice::GetUpChamber()
{
    return this->upChamber;
};

void Orifice::SetDownChamber(Chamber &downChamber)
{
    this->downChamber = downChamber;
    return;
};

Chamber Orifice::GetDownChamber()
{
    return this->downChamber;
};

void Orifice::CalculateFlowrate()
{
    Cf = GetCf();
    area = GetArea();
    Chamber &upCh = this->upChamber;
    Chamber &downCh = this->downChamber;
    double upPress = upCh.GetPressure();
    double downPress = downCh.GetPressure();

    flowrate = Cf * area * sqrt(upPress - downPress);

    UpdateChambersFlow();
    return;
};

void Orifice::UpdateChambersFlow(){
    upChamber.AddFlowIn(name,flowrate);
    downChamber.AddFlowIn(name,-flowrate);
    return;
};

void Orifice::CalculateRHS()
{
    // Calculate and update the flowrate
    this->CalculateFlowrate();

    return;
};

double Orifice::GetRHS()
{
    std::cout << "Orifice GetRHS" << std::endl;
    return this->flowrate;
};