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
    // Instead of N_VNew use N_VMake
    // https://sundials.readthedocs.io/en/latest/cvode/Usage/index.html
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

std::vector<double> System::CalculateSystemRHS()
{
    int noOfDiffEquations = diffEquations.size();
    std::vector<double> RHS;
    double RHStemp;
    
    for (int ii = 0; ii < noOfDiffEquations; ii++)
    {
        RHStemp = diffEquations[ii].CalculateRHS();
        RHS.push_back(RHStemp);
    };
    
    return RHS;
};