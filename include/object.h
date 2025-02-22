#pragma once
#include "./sundials.h"
#include <string>
#include <vector>

// #include <nvector/nvector_serial.h> // access to serial N_Vector

class Object
{
protected:
    // Properties
    std::string name;
    bool isDifferential;

public:
    // Constructor
    Object(std::string name);

    // Calculate RHS of the equation
    virtual void CalculateRHS() = 0;

    // Print heder of the object in the output file
    virtual void PrintHeader(std::ofstream &outputFile) = 0;

    // Print variables of the object in the output file
    virtual void PrintVariables(std::ofstream &outputFile) = 0;
};

class DiffObject : public Object
{
protected:
    // How many dependent variables the object has
    sunindextype nDepVar;

    // Indeces assigned to the equation dependent variables in the system
    std::vector<sunindextype> depVarIndexInSys;

    // Store here the inital value of the object's dependent variables
    std::vector<sunrealtype> yValuesInit;

    // Store here the inital value of the object's dependent variables
    std::vector<sunrealtype> yDotValuesInit;

    // Store here the pointer to the object's dependent variables into the N_Vector y
    std::vector<sunrealtype *> yValuesPnt;

    // Store here the pointer to the object's dependent variables into the N_Vector y
    std::vector<sunrealtype *> yDotValuesPnt;

    // Relative tolerance of the object's equation
    std::vector<sunrealtype> absTol;

public:
    // Constructor initialize name and number of dependent variables
    DiffObject(std::string name, sunindextype nDepVar);

    // Get the number of dependent variables of the object
    sunindextype GetNoOfDepVar();

    // Set the index the dependent variables of the object has in the N_Vector y
    void SetDepVarIndex(sunindextype objDepVarIndex, sunindextype sysDepVarIndex);

    // Get the initial value of the dependent variables of the equation
    sunrealtype GetYValuesInit(sunindextype index);

    // Save the pointer to the location of a dependent variable in the N_Vector y
    void SetYValuesPnt(sunindextype index, sunrealtype *yValuePnt);

    // Save the pointer to the location of a dependent variable derivative in the N_Vector ydoy
    void SetYDotValuesPnt(sunindextype index, sunrealtype *yDotValuePnt);

    // Get the index of the dependent variables of the equation in the system
    sunindextype GetDepVarIndex(sunindextype index);

    // Get absolute tolerance of the equations of the object
    sunrealtype GetAbsTol(sunindextype index);

    // Zero the summation (forces, flows etc.) parameters of the equation
    virtual void ZeroParameters() = 0;
};

class NonDiffObject : public Object
{
public:
    // Constructor
    NonDiffObject(std::string name);
};