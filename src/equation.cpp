#include <string>
#include <iostream>
#include "./equation.h"

Equation::Equation(std::string name) : name(name){};

DiffEquation::DiffEquation(std::string name) : Equation(name){};

sunrealtype DiffEquation::GetAbsTol(sunindextype index)
{
    return this->absTol[index];
};

NonDiffEquation::NonDiffEquation(std::string name) : Equation(name){};