#pragma once

#include <string>

class Equation
{

protected:
    // Properties
    std::string name;
    bool isDifferential;
    double initCond;
    double RHS;

public:

    // Methods
    Equation(std::string name);

    virtual void SetIsDifferential(bool isDifferential);

    virtual bool GetIsDifferential();

    virtual void SetInitialCondition(double initCond);

    virtual double GetInitialCondition();

    virtual double CalculateRHS();

};
