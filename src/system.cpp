#pragma once

#include <vector>
#include <iostream>
#include <fstream>

#include <nvector/nvector_serial.h> // access to serial N_Vector

#include "system.h"
#include "equation.h"

#define Ith(v, i) NV_Ith_S(v, i - 1)

System::System() {};

System::System(SUNContext sunctx) : sunctx(sunctx) {};

void System::AddEquation(DiffEquation &equation)
{
    // Save the equation in the container diffEquations
    diffEquations.push_back(&equation);

    // Assign a system index to the object dependent variables
    // int depVarUpdatedIndex = equation.SetDepVarIndex(this->sysDepVarIndex);

    // Increment the sysDepVarIndex by the number of dependent variables in the equation
    // this->sysDepVarIndex = depVarUpdatedIndex;

    //Get the initial conditions of the equation in the form of a vector
    std::vector<double> eqInitConditions = equation.GetInitialCondition();

    // Append them in the initConditions vector
    this->initConditions.insert(this->initConditions.end(), eqInitConditions.begin(), eqInitConditions.end());

    // Increment the number of differential equations
    this->AddDiffEqCount();
};

void System::AddEquation(NonDiffEquation &equation)
{
    nonDiffEquations.push_back(&equation);
    this->AddNonDiffEqCount();
};

void System::AddSUNContext(SUNContext &sunctx)
{
    sunctx = sunctx;
    return;
};

SUNContext System::GetSUNContext()
{

    return sunctx;
};

std::vector<NonDiffEquation *> System::GetNonDiffEquations()
{
    return nonDiffEquations;
}

std::vector<DiffEquation *> System::GetDiffEquations()
{
    return diffEquations;
}

void System::AddDiffEqCount()
{
    this->noOfDiffEq++;
};

void System::AddNonDiffEqCount()
{
    this->noOfAuxEq++;
}

N_Vector System::GetInitCondition()
{
    N_Vector initCondOut = NULL;
    
    // Instead of N_VNew use N_VMake
    // https://sundials.readthedocs.io/en/latest/cvode/Usage/index.html
    initCondOut = N_VNew_Serial(this->noOfDiffEq, this->sunctx);

    sunrealtype temp;
    
    // for (double initCondTemp : this->initConditions)
    for (int ii = 0; ii < this->noOfDiffEq; ii++)
    {
        double initCondTemp = this->initConditions[ii];
        Ith(initCondOut, ii + 1) = initCondTemp;
    };

    return initCondOut;
};

N_Vector System::GetEqAbsTol()
{
    int noOfDiffEq = diffEquations.size();

    N_Vector eqAbsTol = N_VNew_Serial(noOfDiffEq, sunctx);
    for (int ii = 0; ii < noOfDiffEq; ii++)
    {
        DiffEquation &eqTemp = *diffEquations[ii];
        Ith(eqAbsTol, ii + 1) = eqTemp.GetAbsTol();
    };

    return eqAbsTol;
}

int System::GetNoOfDiffEq()
{
    return this->noOfDiffEq;
}

int System::GetNoOfAuxEq()
{
    return this->noOfAuxEq;
}

double System::GetRelTol()
{
    return relTol;
}

void System::CalculateAuxEqRHS()
{
    int noOfAuxEquations = nonDiffEquations.size();

    for (int ii = 0; ii < noOfAuxEquations; ii++)
    {
        Equation &eqTemp = *nonDiffEquations[ii];
        eqTemp.CalculateRHS();
    };
};

void System::CalculateDiffEqRHS()
{

    int noOfDiffEquations = diffEquations.size();
    for (int ii = 0; ii < noOfDiffEquations; ii++)
    {
        Equation &eqTemp = *diffEquations[ii];
        eqTemp.CalculateRHS();
    };
};

std::vector<double> System::GetDiffEqRHS()
{
    std::vector<double> diffEqRHS;

    int noOfDiffEquations = diffEquations.size();

    for (int ii = 0; ii < noOfDiffEquations; ii++)
    {
        Equation &tempEq = *diffEquations[ii];

        double rhs = tempEq.GetRHS();
        diffEqRHS.push_back(rhs);
    };

    return diffEqRHS;
};

void System::ResetDiffEq(N_Vector y)
{

    // EXTRACT THE DEPENDENT VARIABLE FROM Y
    // Exract the size of y 
    size_t n = N_VGetLength_Serial(y);

    // Access the raw data pointer of the N_Vector
    sunrealtype *yData = N_VGetArrayPointer_Serial(y);

    // Copy the data into a std::vector
    std::vector<sunrealtype> yValues(n);
    for (size_t i = 0; i < n; ++i) 
    {
        yValues[i] = yData[i];
    }

    // UPDATE THE OBJECTS OF THE SYSTEM
    int noOfDiffEquations = this->diffEquations.size();

    for (int ii = 0; ii < noOfDiffEquations; ii++)
    {
        DiffEquation &tempEq = *diffEquations[ii];

        tempEq.UpdateDepVar(yValues);
        tempEq.ZeroParameters();
    };

    return;
}

void System::ConnectYToDepVar(){

    // Create the N_Vector y
    y = N_VNew_Serial(this->noOfDiffEq, this->sunctx);

    // Extract the pointer to the elements of the N_Vector y
    realtype *yData = N_VGetArrayPointer_Serial(y);

    sunindextype noOfEq = 0;

    // Iterate on the objects of the system 
    for (int ii = 0; ii < this->noOfDiffEq; ii++)
    {
        DiffEquation &tempEq = *diffEquations[ii];

        // Iterate on the equations of the object
        for (int jj = 0; jj < tempEq.yValues.size(); jj++)
        {
            // Save the index of the object dependent variables in the system
            tempEq.SetDepVarIndex(jj,noOfEq);

            // Assign the value of the dependent variable to the N_Vector
            yData[noOfEq] = tempEq.yValues[jj];

            // Save in the object the pointer to the dependent variable in the N_Vector
            tempEq.yValuesPnt[jj] = &yData[noOfEq];

            // Increment the number of equations
            noOfEq++;
        };
        
    };
    std::cout << "Equations added to the system: " << noOfEq << std::endl;
};

void System::ConnectYDotToDepVarDerivatives(N_Vector ydot)
{
    // Extract the pointer to the elements of the N_Vector ydot
    realtype *yDotData = N_VGetArrayPointer_Serial(ydot);

    // Iterate on the objects of the system 
    for (int ii = 0; ii < this->noOfDiffEq; ii++)
    {
        DiffEquation &tempEq = *diffEquations[ii];

        // Iterate on the equation of the object
        for (int jj = 0; jj < tempEq.yDotValues.size(); jj++)
        {
            // Save in the object the pointer to the dependent variable derivative in the N_Vector
            tempEq.yDotValuesPnt[jj] = &yDotData[tempEq.depVarIndexInSys[jj]];
        };
    };
};


N_Vector System::GetY()
{
    return this->y;
};