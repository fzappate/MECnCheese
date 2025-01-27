#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include <nvector/nvector_serial.h> // access to serial N_Vector

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

void Chamber::AddFlowIn(std::string flowName, double flowrate)
{
    this->flowSum = this->flowSum+flowrate;
    this->flowIn.push_back(flowrate);
    this->flowInNames.push_back(flowName);
    return;
};

void Chamber::CalculateRHS()
{
    return;
};

double Chamber::GetRHS()
{
    return this->dpdt;
};

int Chamber::SetDepVarIndex(int sysDepVarIndex)
{   

    this->depVarIndexInSys.push_back(sysDepVarIndex);
    sysDepVarIndex++;
    return sysDepVarIndex;
};

void Chamber::UpdateDepVar(std::vector<sunrealtype> &yValues)
{

    this->pressure = yValues[this->depVarIndexInSys[0]];
    return; 
}

void Chamber::ZeroParameters()
{
    flowSum = 0;
    flowIn.clear();
    flowInNames.clear();
    
    return;
}

std::vector<double> Chamber::GetInitialCondition()
{
    std::vector<double> initCond;
    initCond.push_back(pressure);
    return initCond;
};

double Chamber::GetAbsTol()
{
    return aTol;
}

double Chamber::GetRelTol()
{
    return rTol;
}

void Chamber::PrintHeader(std::ofstream& outputFile)
{
    if(printStruct.printBulkMod == 1)
    {
    outputFile << name << ":Bulk Modulus:Pa*s, ";
    }
    if(printStruct.printBulkMod == 1)
    {
        outputFile << name << ":Pressure:Pa, ";
    }
    if(printStruct.printBulkMod == 1)
    {
        outputFile << name << ":Volume:m^3, ";
    }
    if(printStruct.printBulkMod == 1)
    {
        outputFile << name << ":Volume Derivative:m^3/s, ";
    }
    if(printStruct.printBulkMod == 1)
    {
        outputFile << name << ":Flow Sum:m^3/s, ";
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
    this->dpdt = 0;
    return;
};


// ConstChamber
ConstChamber::ConstChamber(std::string name, double pressure, double volume) : Chamber(name, pressure)
{
    this->volume = volume;
    this->volDer = 0;
    return;
};

void ConstChamber::CalculateRHS()
{

    this->dpdt = bulkMod / volume * (flowSum - volDer);

    return;

};