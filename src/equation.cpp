#include <string>
#include <iostream>
#include "./equation.h"

Equation::Equation(std::string name) : name(name){};

DiffEquation::DiffEquation(std::string name) : Equation(name){};

NonDiffEquation::NonDiffEquation(std::string name) : Equation(name){};