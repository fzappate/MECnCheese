#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include "./equation.h"
#include "./chamber.h"

// Chamber
Chamber::Chamber(std::string name, double pressure) : Equation(name), pressure(pressure)
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

void Chamber::PrintHeader(std::ofstream& outputFile)
{
    if(printStruct.printBulkMod == 1)
    {
    outputFile << name << "_Bulk Modulus, ";
    }
    if(printStruct.printBulkMod == 1)
    {
        outputFile << name << "_Pressure, ";
    }
    if(printStruct.printBulkMod == 1)
    {
        outputFile << name << "_Volume, ";
    }
    if(printStruct.printBulkMod == 1)
    {
        outputFile << name << "_Volume Derivative, ";
    }
    if(printStruct.printBulkMod == 1)
    {
        outputFile << name << "_Flow Sum, ";
    }

    
    return;
}

void Chamber::PrintVariables(std::ofstream& outputFile)
{
    if(printStruct.printBulkMod == 1)
    {
    outputFile << bulkMod << ",";
    }
    if(printStruct.printPress == 1)
    {
        outputFile << pressure << ",";
    }
    if(printStruct.printVolume == 1)
    {
        outputFile << volume << ",";
    }
    if(printStruct.printVolDer == 1)
    {
        outputFile << volDer << ", ";
    }
    if(printStruct.printFlowSum == 1)
    {
        outputFile << flowSum << ",";
    }

    
    return;
}


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

void InfChamber::CalculateRHS()
{

    return;

};

// ConstChamber
ConstChamber::ConstChamber(std::string name, double pressure, double volume) : Chamber(name, pressure)
{
    this->volume = volume;
    this->volDer = 0;
    this->SetIsDifferential(true);
    return;
};

void ConstChamber::CalculateRHS()
{

    this->dpdt = bulkMod / volume * (flowSum - volDer);

    return;

};