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
    // Set initial conditions
    this->yValuesInit.push_back(pressure);
    this->yDotValuesInit.push_back(0.0);

    // Initialize the pointers that connect to the dependent variables in the system
    this->yValuesPnt = std::vector<sunrealtype*>(1, nullptr);
    this->yDotValuesPnt = std::vector<sunrealtype*>(1, nullptr);
    this->depVarIndexInSys = std::vector<sunindextype>(1, -1);

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

void Chamber::SetDepVarIndex(sunindextype objDepVarIndex, sunindextype sysDepVarIndex)
{   
    this->depVarIndexInSys[objDepVarIndex] = sysDepVarIndex;
    return;
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

    // Dereference the pointer that points to the pressure of the chamber
    sunrealtype &press = *yValuesPnt[0];

    if(printStruct.printBulkMod == 1)
    {
    outputFile << bulkMod << ",";
    }
    if(printStruct.printPress == 1)
    {
        outputFile << press << ",";
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
    // Calculate and store the depenedent variable in the object
    this->dpdt = 0;

    // Updatet the dependent variable in y
    *yDotValuesPnt[0] = this->dpdt;

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
    // Calculate and store the depenedent variable in the object
    this->dpdt = bulkMod / volume * (flowSum - volDer);

    // Updatet the dependent variable in y
    *yDotValuesPnt[0] = this->dpdt;
    return;

};