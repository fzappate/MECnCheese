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
    // Save the differential equation reference in the system
    diffEquations.push_back(&equation);

    // Increment the number of differential equations
    this->noOfDiffEq++;
};

void System::AddEquation(NonDiffEquation &equation)
{
    // Save the non-differential equation reference in the system
    nonDiffEquations.push_back(&equation);

    // Increment the number of non-differential equations
    this->noOfAuxEq++;
    return;
};

void System::AddSUNContext(SUNContext &sunctx)
{
    // Save the SUNDIALS context in the system
    this->sunctx = sunctx;
    return;
};

SUNContext System::GetSUNContext()
{
    // Return the SUNDIALS context saved in the system
    return sunctx;
};

std::vector<NonDiffEquation *> System::GetNonDiffEquations()
{
    // Return the non-differential equations saved in the system
    return nonDiffEquations;
};

std::vector<DiffEquation *> System::GetDiffEquations()
{
    // Return the differential equations saved in the system
    return diffEquations;
};

N_Vector System::GetEqAbsTol()
{
    // Create the N_Vector eqAbsTol
    N_Vector eqAbsTol = N_VNew_Serial(this->noOfDiffEq, sunctx);

    // Extract the pointer to the elements of the N_Vector eqAbsTol
    realtype *eqAbsTolData = N_VGetArrayPointer_Serial(eqAbsTol);

    // Save the absolute tolerance of the differential equations in eqAbsTol
    for (int ii = 0; ii < this->noOfDiffEq; ii++)
    {
        DiffEquation &eqTemp = *diffEquations[ii];
        eqAbsTolData[ii] = eqTemp.GetAbsTol();
    };

    return eqAbsTol;
};

int System::GetNoOfDiffEq()
{
    return this->noOfDiffEq;
};

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

void System::ResetDiffEq(N_Vector y)
{
    // Zero the summation parameters of the equations of the differential objects
    for (int ii = 0; ii < this->noOfDiffEq; ii++)
    {
        DiffEquation &tempEq = *diffEquations[ii];
        tempEq.ZeroParameters();
    };

    return;
}

void System::ConnectYToDepVar()
{
    // Define the N_Vector yInitCond
    this->yInitCond = N_VNew_Serial(this->noOfDiffEq, this->sunctx);

    // Extract the pointer to the elements of the N_Vector yInitCond
    realtype *yData = N_VGetArrayPointer_Serial(this->yInitCond);

    // Start a counter to keep track of the equations' index in the system
    sunindextype noOfEq = 0;

    // Iterate on the objects of the system
    for (int ii = 0; ii < this->noOfDiffEq; ii++)
    {
        DiffEquation &tempEq = *diffEquations[ii];

        // Iterate on the equations of the object
        for (int jj = 0; jj < tempEq.yValuesInit.size(); jj++)
        {
            // Save the index of the object dependent variables in the system
            tempEq.SetDepVarIndex(jj, noOfEq);

            // Assign the value of the dependent variable to the N_Vector
            yData[noOfEq] = tempEq.yValuesInit[jj];

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
        for (int jj = 0; jj < tempEq.yDotValuesInit.size(); jj++)
        {
            // Save in the object the pointer to the dependent variable derivative in the N_Vector
            tempEq.yDotValuesPnt[jj] = &yDotData[tempEq.depVarIndexInSys[jj]];
        };
    };
};

N_Vector System::GetYInitCond()
{
    return this->yInitCond;
};

N_Vector System::GetYDot()
{
    return this->ydot;
};

void System::SetYDot(N_Vector ydot)
{
    this->ydot = ydot;
    return;
};