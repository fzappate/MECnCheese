#include <vector>
#include <nvector/nvector_serial.h>    // access to serial N_Vector

#include "system.h"
#include "equation.h"

#define Ith(v, i) NV_Ith_S(v, i - 1)  

System::System(SUNContext &sunctx): sunctx(sunctx){};

void System::AddEquation(Equation equation)
{

    // Store initial condition in the system

    // Increase the proper counter by one
    if (equation.GetIsDifferential())
    {
        diffEquations.push_back(equation);
        double initConditions = equation.GetInitialCondition();
        this->initConditions.push_back(initConditions);
        this->AddDiffEqCount();
    }
    else
    {
        auxEquations.push_back(equation);
        this->AddLinEqCount();
    };
};

void System::AddSunContext(SUNContext &sunctx){
    sunctx = sunctx;
};

void System::AddDiffEqCount()
{
    this->noOfDiffEq++;
};

void System::AddLinEqCount()
{
    this->noOfAuxEq++;
}

N_Vector System::GetInitCondition()
{
    N_Vector initCondTemp;
    initCondTemp = NULL;
    initCondTemp = N_VNew_Serial(this->noOfDiffEq, this->sunctx);
    sunrealtype temp;
    // Ith(initCondTemp,1) = this->initConditions[ii];
    for (int ii = 0; ii<noOfDiffEq; ii++)
    {
        temp = this->initConditions[ii]; // ok
        Ith(initCondTemp,ii+1) = this->initConditions[ii];

    };
    // Store initial conditions
    N_VectInitConditions = initCondTemp;
     
    sunrealtype testCont = Ith(initCondTemp, 1);
    // Output initial conditions
    return initCondTemp;
};

N_Vector System::CalculateSystemRHS(sunrealtype t, N_Vector y, N_Vector ydot, void *user_data)
{
    // Update RHS of the aux equations 
    // Update RHS of differential equations 

    int noOfDiffEquations = diffEquations.size();
    for (int ii = 0; ii < noOfDiffEquations; ii++)
    {
        Ith(ydot,ii+1) = 111;
    };
    N_Vector u;
    u = NULL;
    u = N_VNew_Serial(this->noOfDiffEq, this->sunctx);
    return u;
};