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

    virtual void SetIsDifferential(bool isDifferential) = 0;

    virtual bool GetIsDifferential() = 0; 

    virtual void PrintHeader(std::ofstream &outputFile) = 0;

    virtual void PrintVariables(std::ofstream &outputFile) = 0;
};

class DiffEquation : public Equation
{
    public:
    DiffEquation(std::string name);

    virtual void UpdateDepVar(double depVar) = 0;

    virtual void UpdateDepVar(double depVar) = 0;

    virtual void ZeroParameters() = 0;

    virtual void CalculateRHS() = 0;

    virtual double GetRHS() = 0;

    virtual double GetInitialCondition() = 0;

    virtual double GetAbsTol() = 0;

    virtual double GetRelTol() = 0;
};

class NonDiffEquation : public Equation
{
    public:
    NonDiffEquation(std::string name);

    virtual void CalculateRHS() = 0;

    virtual double GetRHS() = 0;
};