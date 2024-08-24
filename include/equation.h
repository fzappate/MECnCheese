#pragma once

#include <string>

class Equation
{

protected:
    // Properties
    std::string name;
    bool isDifferential;
    double rTol; 
    double aTol;

public:

    // Methods
    Equation(std::string name);

    virtual void SetIsDifferential(bool isDifferential) = 0;

    virtual bool GetIsDifferential() = 0;

    virtual void UpdateDepVar(double depVar) = 0;

    virtual void ZeroParameters() = 0;

    virtual void CalculateRHS() = 0;

    virtual double GetRHS() = 0;

    virtual double GetInitialCondition() = 0;

    virtual double GetAbsTol() = 0;

    virtual double GetRelTol() = 0;
};
