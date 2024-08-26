#pragma once

#include <vector>

#include <cvode/cvode.h>               // prototypes for CVODE fcts., consts.
#include <nvector/nvector_serial.h>    // access to serial N_Vector

#include "equation.h"

class System{
protected:

    // Properties
    int noOfAuxEq = 0;
    int noOfDiffEq = 0;
    double relTol = 0.001;
    std::vector<Equation*> auxEquations;
    std::vector<Equation*> diffEquations;
    std::vector<sunrealtype> initConditions;
    N_Vector N_VectInitConditions;
    SUNContext sunctx;

    // Methods
    void AddDiffEqCount();
    void AddLinEqCount();

public:

    System();

    System(SUNContext sunctx);

    void AddEquation(Equation& equation);

    void AddSUNContext(SUNContext &sunctx);

    SUNContext GetSUNContext();

    N_Vector GetInitCondition();

    N_Vector GetEqAbsTol();

    int GetNoOfDiffEq();
    
    double GetRelTol();

    void CalculateDiffEqRHS();

    void CalculateAuxEqRHS();

    std::vector<double> GetDiffEqRHS();

    void ResetDiffEq(N_Vector y);

};