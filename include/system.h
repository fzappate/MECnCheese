#pragma once

#include <vector>

#include <cvode/cvode.h>            // prototypes for CVODE fcts., consts.
#include <nvector/nvector_serial.h> // access to serial N_Vector

#include "./object.h"

class System
{
protected:
    // Number of non differential equations in the system
    int noOfAuxEq = 0;

    // Number of differential equations in the system
    int noOfDiffEq = 0;

    // Number of differential objects in the system
    sunindextype noOfDiffObj = 0;

    // Number of dependent variables in the system
    int sysDepVarIndex = 0;

    // Relative tolerance of the system
    double relTol = 0.001;

    // Pointers to the non-differential object in the system
    std::vector<NonDiffObject *> nonDiffObjects;

    // Pointers to the differential object in the system
    std::vector<DiffObject *> diffObjects;

    // Initial conditions of the system
    std::vector<sunrealtype> initConditions;

    // N_Vector y used as initial conditions in the solver
    N_Vector yInitCond;

    // Save ydot to detect when it changes
    N_Vector ydot;

    // SUNContext used to run the simulation
    SUNContext sunctx;

public:
    // Default constructor
    System();

    // Constructor
    System(SUNContext sunctx);

    // Save differential object pointer in the system
    void AddObject(DiffObject &equation);

    // Save non-differential object pointer in the system
    void AddObject(NonDiffObject &equation);

    // Get vector of pointers to non-differential objects
    std::vector<NonDiffObject *> GetNonDiffObjects();

    // Get vector of pointers to differential objects
    std::vector<DiffObject *> GetDiffObjects();

    // Get the number of differential equations in the system
    int GetNoOfDiffEq();

    // Get the number of non-differential equations in the system
    int GetNoOfAuxEq();

    // Get the number of differential objects in the system
    int GetNoOfDiffObj();

    // Connect the dependent variables of the differential objects to the N_Vector y
    void ConnectYToDepVar();

    // Connect the ydot vector to the derivatives of the dependent variables of the differential objects
    void System::ConnectYDotToDepVarDeriv(N_Vector ydot);

    // Save the SUNDIALS context in the system
    void AddSUNContext(SUNContext &sunctx);

    // Get the SUNDIALS context stored in the system
    SUNContext GetSUNContext();

    // Calculate the right hand side of the equations of the differential objects
    void CalculateDiffEqRHS();

    // Calculate the right hand side of the equations of the non-differential objects
    void CalculateAuxEqRHS();

    // Reset the differential equations parameters
    void ResetDiffEq(N_Vector y);

    // Get the y vector
    N_Vector GetYInitCond();

    // Get the ydot vector
    N_Vector GetYDot();

    // Set the ydot vector
    void SetYDot(N_Vector ydot);

    // Get the absolute tolerance of the equations of the differential objects
    N_Vector GetObjAbsTol();

    // Get the relative tolerance of the system
    double GetRelTol();
};