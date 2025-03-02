#pragma once

#include "./system.h"

class Solver
{
private:
    // Time value used in the simulation
    sunrealtype time;

    // Simulation start time
    sunrealtype startTime = 0;

    // Timestep between printing
    sunrealtype printTime;

    // Time at which the simulation ends
    sunrealtype endTime;

    // Time at which the results are printed out
    sunrealtype outTime;

    // Max timestep taken in the simulation.
    // By default, it is set to 0.1 the print time.
    sunrealtype stepTime;

    // Relative tolerance used to solve the system
    sunrealtype relTol = 0.001;

    // Max number of iteration 
    sunindextype maxNoOfIter = 1e6;

    // Maximum order of back differentiation integration method
    sunindextype maxBfgOrder = 1;

public:

    // Constructor
    Solver(sunrealtype printTime, sunrealtype endTime);

    // Set the simulation max step value
    void SetStepTime(sunrealtype stepTime);

    // Set the solver maximum number of iteration 
    void SetMaxNoOfIter(sunindextype maxNoOfIter);

    // Set maximum order of back differentiation integration method
    void SetMaxOrderBfg(sunindextype maxBfgOrder);

    // Solve the system
    int SolveSystem(System sys);

    // Check CVode solver return value
    int CheckReturnValue(void* returnvalue, const char* funcname, int opt);

    // Function that calculates the RHS of the system of equation
    static int fFunction(sunrealtype t, N_Vector y, N_Vector ydot, void* user_data);
};