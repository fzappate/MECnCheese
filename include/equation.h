#pragma once

#include <string>
#include <vector>

#include <nvector/nvector_serial.h> // access to serial N_Vector


class Equation
{
protected:
    // Properties
    std::string name;
    bool isDifferential;

public:
    // Methods
    Equation(std::string name);

    virtual void CalculateRHS() = 0;

    virtual double GetRHS() = 0;

    virtual void PrintHeader(std::ofstream &outputFile) = 0;

    virtual void PrintVariables(std::ofstream &outputFile) = 0;
};

class DiffEquation : public Equation
{
    public:

    // Store here the inital value of the object's dependent variables
    std::vector<sunrealtype> yValues;

    // Store here the pointer to the object's dependent variables into the N_Vector y
    std::vector<sunrealtype*> yValuesPnt;

    // Indeces assigned to the equation dependent variables in the system
    std::vector<int> depVarIndexInSys;

    // Constructor
    DiffEquation(std::string name);

    // Set the index the dependent variables of the equation has in the system 
    virtual int SetDepVarIndex(int sysDepVarIndex) = 0;

    // Update the dependent variables of the equation with the values in y
    virtual void UpdateDepVar(std::vector<sunrealtype> &yValues) = 0;

    // Zero the summation (forces, flows etc.) parameters of the equation
    virtual void ZeroParameters() = 0;

    // Get the deendent variable of the equation 
    virtual std::vector<double> GetInitialCondition() = 0;

    virtual double GetAbsTol() = 0;

    virtual double GetRelTol() = 0;
};

class NonDiffEquation : public Equation
{
    public:
    NonDiffEquation(std::string name);
};