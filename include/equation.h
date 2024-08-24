#pragma once

#include <string>

class Equation 
{
    protected:
    // Properties
    std::string name;
    bool isDifferential;

    public:
    // Methods
    Equation(std::string name);

    bool GetIsDifferential()
    {
        return isDifferential;
    };
};

class DiffEquation : public Equation
{
protected:
    // Properties
    double rTol; 
    double aTol;
    const bool isDifferential = 1;

public:
    // Methods
    DiffEquation(std::string name);

    virtual void UpdateDepVar(double depVar) = 0;

    virtual void ZeroParameters() = 0;

    virtual void CalculateRHS() = 0;

    virtual double GetRHS() = 0;

    virtual double GetInitialCondition() = 0;

    virtual double GetAbsTol() = 0;

    virtual double GetRelTol() = 0;

};

class AuxEquation : public Equation
{
protected:
    const bool isDifferential = 0;

public:
    // Methods
    AuxEquation(std::string name);

    virtual void CalculateRHS() = 0;

};
