#include <iostream>
#include <string>

#include "./equation.h"
#include "./chamber.h"

// Chamber
Chamber::Chamber(std::string name, double pressure) : Equation(name), pressure(pressure)
{
    this->SetInitialCondition(pressure);
    return;
};

void Chamber::SetPressure(double press)
{
    pressure = press;

    return;
};

double Chamber::GetPressure()
{
    return pressure;
};

void Chamber::SetInitialCondition(double initCond)
{
    this->initCond = initCond;
};

double Chamber::GetInitialCondition()
{
    return pressure;
};

void Chamber::SetIsDifferential(bool isDifferential)
{
    this->isDifferential = isDifferential;
    return;
}

bool Chamber::GetIsDifferential()
{
    return isDifferential;
};

void Chamber::AddFlowIn(std::string flowName, double flowrate)
{
    this->flowSum = this->flowSum+flowrate;
    this->flowIn.push_back(flowrate);
    this->flowInNames.push_back(flowName);
    return;
};

void Chamber::CalculateRHS()
{

    this->pressure = bulkMod / volume * (flowSum - volDer);

    return;

};

double Chamber::GetRHS()
{
    std::cout << "Chamber GetRHS" << std::endl;
    return this->pressure;
};

// InfChamber
InfChamber::InfChamber(std::string name, double pressure) : Chamber(name, pressure)
{
    this->SetIsDifferential(false);
    this->SetPressure(pressure);

    return;
};

void InfChamber::AddFlowIn(std::string flowName, double flowrate)
{
    // Override the standard AddFlowIn to make sure that no flow is exchanghed
    // with infinite chambers
    this->flowIn.push_back(0.0);
    this->flowInNames.push_back(flowName);

    return;
};

// Chamber
ConstChamber::ConstChamber(std::string name, double pressure, double volume) : Chamber(name, pressure), volume(volume)
{
    this->SetIsDifferential(true);
    return;
};

