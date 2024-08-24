#include <vector>
#include <nvector/nvector_serial.h>    // access to serial N_Vector

#include "system.h"
#include "equation.h"

#define Ith(v, i) NV_Ith_S(v, i - 1)  

System::System(SUNContext &sunctx): sunctx(sunctx){};

void System::AddEquation(Equation& equation)
{

    // Store initial condition in the system

    // Increase the proper counter by one
    if (equation.GetIsDifferential())
    {
        diffEquations.push_back(&equation);
        double initConditions = equation.GetInitialCondition();
        this->initConditions.push_back(initConditions);
        this->AddDiffEqCount();
    }
    else
    {
        auxEquations.push_back(&equation);
        this->AddLinEqCount();
    };
};

void System::AddSUNContext(SUNContext &sunctx){
    sunctx = sunctx;
};

SUNContext System::GetSUNContext(){
    // SUNContext* sunctxPtr = sunctx;
    return sunctx; 
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
     
    return initCondTemp;
};

N_Vector System::GetEqAbsTol()
{
    int noOfDiffEq = diffEquations.size();
    
    N_Vector eqAbsTol = N_VNew_Serial(noOfDiffEq, sunctx);
    for (int ii = 0; ii < noOfDiffEq; ii++)
    {
        Equation &eqTemp = *diffEquations[ii];
        Ith(eqAbsTol,ii+1) = eqTemp.GetAbsTol();
    };

    return eqAbsTol;
}

double System::GetRelTol()
{
    return relTol;
}

void System::CalculateAuxEqRHS()
{
    int noOfAuxEquations = auxEquations.size();
    
    for (int ii = 0; ii < noOfAuxEquations; ii++)
    {
        Equation &eqTemp = *auxEquations[ii];
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
    int noOfDiffEquations = diffEquations.size();

    
    for (int ii = 0; ii < noOfDiffEquations; ii++)
    {
        Equation &tempEq = *diffEquations[ii];
        double tempDepVar = Ith(y,ii+1);
        
        tempEq.UpdateDepVar(tempDepVar);
        tempEq.ZeroParameters();

    };

    return;

}
