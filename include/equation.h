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

    // Calculate RHS of the equation 
    virtual void CalculateRHS() = 0;

    // Print heder of the object in the output file
    virtual void PrintHeader(std::ofstream &outputFile) = 0;

    // Print variables of the object in the output file
    virtual void PrintVariables(std::ofstream &outputFile) = 0;
};

class DiffEquation : public Equation
{
    public:

    // Store here the inital value of the object's dependent variables
    std::vector<sunrealtype> yValuesInit;

    // Store here the inital value of the object's dependent variables
    std::vector<sunrealtype> yDotValuesInit;

    // Store here the pointer to the object's dependent variables into the N_Vector y
    std::vector<sunrealtype*> yValuesPnt;

    // Store here the pointer to the object's dependent variables into the N_Vector y
    std::vector<sunrealtype*> yDotValuesPnt;

    // Indeces assigned to the equation dependent variables in the system
    std::vector<sunindextype> depVarIndexInSys;

    // Relative tolerance of the object's equation 
    std::vector<sunrealtype> absTol;

    // Constructor
    DiffEquation(std::string name);

    // Set the index the dependent variables of the equation has in the system 
    virtual void SetDepVarIndex(sunindextype objDepVarIndex, sunindextype sysDepVarIndex) = 0;

    // Zero the summation (forces, flows etc.) parameters of the equation
    virtual void ZeroParameters() = 0;

    // Get absolute tolerance of the equations of the object
    sunrealtype GetAbsTol(sunindextype index);

};

class NonDiffEquation : public Equation
{
    public:
    // Constructor
    NonDiffEquation(std::string name);
};