#pragma once

#include "./system.h"


class Solver
{
    public:

    double time;
    double startTime = 0;
    double outTime;
    double stepTime;
    double endTime;

    Solver(double stepTime, double endTime);

    int SolveSystem(System sys);

    int CheckReturnValue(void *returnvalue, const char *funcname, int opt);

    static int fFunction(sunrealtype t, N_Vector y, N_Vector ydot, void *user_data);

};