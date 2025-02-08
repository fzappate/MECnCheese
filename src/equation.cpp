#include <string>
#include <iostream>
#include "./equation.h"

Equation::Equation(std::string name) : name(name){};

DiffEquation::DiffEquation(std::string name) : Equation(name){};

sunrealtype DiffEquation::GetAbsTol(sunindextype index)
{
    return this->absTol[index];
};

void DiffEquation::SetDepVarIndex(sunindextype objDepVarIndex, sunindextype sysDepVarIndex)
{   
    this->depVarIndexInSys[objDepVarIndex] = sysDepVarIndex;
    return;
};

NonDiffEquation::NonDiffEquation(std::string name) : Equation(name){};