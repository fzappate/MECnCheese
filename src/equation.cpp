#include <string>
#include "./equation.h"

Equation::Equation(std::string name) : name(name){};

void Equation::SetIsDifferential(bool isDifferential){};

bool Equation::GetIsDifferential(){ return isDifferential; }; 

void Equation::SetInitialCondition(double initCond){
    this->initCond = initCond;
};

double Equation::GetInitialCondition(){ return initCond; };

double Equation::CalculateRHS() { return RHS; };

