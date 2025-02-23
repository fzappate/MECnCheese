#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include "./object.h"
#include "./chamber.h"

// Chamber
Chamber::Chamber(std::string name, double pressure) : DiffObject(name, 1), initPressure(pressure)
{
    // Set initial conditions
    this->yValuesInit.push_back(pressure);
    this->yDotValuesInit.push_back(0.0);
    this->absTol.push_back(0.001);

    // Initialize the pointers that connect to the dependent variables in the system
    this->yValuesPnt = std::vector<sunrealtype *>(this->nDepVar, nullptr);
    this->yDotValuesPnt = std::vector<sunrealtype *>(this->nDepVar, nullptr);
    this->depVarIndexInSys = std::vector<sunindextype>(this->nDepVar, -1);

    return;
};

double Chamber::GetPressure()
{
    return *yValuesPnt[0];
};

void Chamber::AddFlowIn(std::string flowName, double flowrate)
{
    this->flowSum = this->flowSum + flowrate;
    this->flowIn.push_back(flowrate);
    this->flowInNames.push_back(flowName);
    return;
};

void Chamber::CalculateRHS()
{
    return;
};

void Chamber::ZeroParameters()
{
    flowSum = 0;
    flowIn.clear();
    flowInNames.clear();

    return;
}

void Chamber::PrintHeader(std::ofstream &outputFile)
{
    if (printStruct.printBulkMod == 1)
    {
        outputFile << name << ":Bulk Modulus:Pa*s, ";
    }
    if (printStruct.printPress == 1)
    {
        outputFile << name << ":Pressure:Pa, ";
    }
    if (printStruct.printVolume == 1)
    {
        outputFile << name << ":Volume:m^3, ";
    }
    if (printStruct.printVolDer == 1)
    {
        outputFile << name << ":Volume Derivative:m^3/s, ";
    }
    if (printStruct.printFlowSum == 1)
    {
        outputFile << name << ":Flow Sum:m^3/s, ";
    }

    return;
}

void Chamber::PrintVariables(std::ofstream &outputFile)
{

    // Dereference the pointer that points to the pressure of the chamber
    sunrealtype &press = *yValuesPnt[0];

    if (printStruct.printBulkMod == 1)
    {
        outputFile << this->bulkMod << ",";
    }
    if (printStruct.printPress == 1)
    {
        outputFile << press << ",";
    }
    if (printStruct.printVolume == 1)
    {
        outputFile << this->volume << ",";
    }
    if (printStruct.printVolDer == 1)
    {
        outputFile << this->volDer << ", ";
    }
    if (printStruct.printFlowSum == 1)
    {
        outputFile << this->flowSum << ",";
    }

    return;
}

// InfChamber
InfChamber::InfChamber(std::string name, double pressure) : Chamber(name, pressure)
{
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

// VariableChamber
VariableChamber::VariableChamber(std::string name, double initPressure, double initVolume) : Chamber(name, initPressure)
{
    this->volume = initVolume;
    this->volDer = 0;
    return;
};

void VariableChamber::CalculateRHS()
{
    // Calculate and store the depenedent variable in the object
    this->dpdt = bulkMod / volume * (flowSum - volDer);

    // Updatet the dependent variable in yDot
    *yDotValuesPnt[0] = this->dpdt;
    return;
};

void VariableChamber::SetVolumeDer(double volumeDer)
{
    this->volDer = volumeDer;
    return;
};