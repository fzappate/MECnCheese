#include <iostream>
#include <string>

#include "./equation.h"
#include "./chamber.h"

// Chamber
Chamber::Chamber(std::string name, double pressure) : DiffEquation(name), pressure(pressure)
{
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

double Chamber::GetAbsTol()
{
    return aTol;
}

double Chamber::GetRelTol()
{
    return rTol;
}

void Chamber::AddFlowIn(std::string flowName, double flowrate)
{
    this->flowSum = this->flowSum+flowrate;
    this->flowIn.push_back(flowrate);
    this->flowInNames.push_back(flowName);
    return;
};

void Chamber::UpdateDepVar(double pressure)
{

    this->pressure = pressure;
    return; 
}

void Chamber::ZeroParameters()
{
    flowSum = 0;
    flowIn.clear();
    flowInNames.clear();
    
    return;
}

void Chamber::CalculateRHS()
{

    this->dpdt = bulkMod / volume * (flowSum - volDer);

    return;

};

double Chamber::GetRHS()
{
    return this->dpdt;
};

double Chamber::GetInitialCondition()
{
    return this->pressure;
};

// PressureSource
PressureSource::PressureSource(std::string name, double pressure) : AuxEquation(name)
{
    
    this->pressure = pressure;

    return;
};

void PressureSource::AddFlowIn(std::string flowName, double flowrate)
{
    // Override the standard AddFlowIn to make sure that no flow is exchanghed
    // with infinite chambers
    this->flowIn.push_back(0.0);
    this->flowInNames.push_back(flowName);

    return;
};

void PressureSource::CalculateRHS()
{
    return;
}
