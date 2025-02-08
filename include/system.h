#pragma once

#include <vector>

#include <cvode/cvode.h>            // prototypes for CVODE fcts., consts.
#include <nvector/nvector_serial.h> // access to serial N_Vector

#include "equation.h"

class System
{
protected:
    // Number of non differential equations in the system
    int noOfAuxEq = 0;

    // Number of differential equations in the system
    int noOfDiffEq = 0;

    // Number of dependent variables in the system
    int sysDepVarIndex = 0;

    // Relative tolerance of the system
    double relTol = 0.001;

    // Pointers to the non-differential object in the system
    std::vector<NonDiffEquation *> nonDiffEquations;

    // Pointers to the differential object in the system
    std::vector<DiffEquation *> diffEquations;

    // Initial conditions of the system
    std::vector<sunrealtype> initConditions;

    // N_Vector y used as initial conditions in the solver
    N_Vector y;

    // SUNContext used to run the simulation
    SUNContext sunctx;

    // Save ydot to detect when it changes
    N_Vector ydot;

public:

    // Default constructor
    System();

    // Constructor
    System(SUNContext sunctx);

    // Save differential object pointer in the system
    void AddEquation(DiffEquation &equation);

    // Save non-differential object pointer in the system
    void AddEquation(NonDiffEquation &equation);

    // Save the SUNDIALS context in the system
    void AddSUNContext(SUNContext &sunctx);

    // Get the SUNDIALS context stored in the system
    SUNContext GetSUNContext();

    // Get vector of pointers to non-differential objects
    std::vector<NonDiffEquation *> GetNonDiffEquations();

    // Get vector of pointers to differential objects
    std::vector<DiffEquation *> GetDiffEquations();

    // Get the initial conditions of the system
    N_Vector GetInitCondition();

    N_Vector GetEqAbsTol();

    int GetNoOfDiffEq();

    int GetNoOfAuxEq();

    double GetRelTol();

    void CalculateDiffEqRHS();

    void CalculateAuxEqRHS();

    std::vector<double> GetDiffEqRHS();

    void ResetDiffEq(N_Vector y);

    void ConnectYToDepVar();

    void System::ConnectYDotToDepVarDerivatives(N_Vector ydot);

    // Set y vector
    N_Vector GetY();

    // Get the ydot vector
    N_Vector GetYDot();

    // Set the ydot vector
    void SetYDot(N_Vector ydot);
};