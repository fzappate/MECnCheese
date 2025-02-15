#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>

#include <sundials/sundials_math.h>    // Import math functions
#include <cvode/cvode.h>               // prototypes for CVODE fcts., consts.
#include <nvector/nvector_serial.h>    // access to serial N_Vector
#include <sunmatrix/sunmatrix_dense.h> // access to dense SUNMatrix
#include <sunlinsol/sunlinsol_dense.h> // access to dense SUNLinearSolver

#include "./solver.h"
#include "./printer.h"

#define Ith(v, i) NV_Ith_S(v, i - 1)                /* i-th vector component i=1..NEQ */
#define IJth(A, i, j) SM_ELEMENT_D(A, i - 1, j - 1) /* (i,j)-th matrix component i,j=1..NEQ */

Solver::Solver(double stepTime, double endTime) : stepTime(stepTime), endTime(endTime)
{
  outTime = stepTime;
  return;
}

int Solver::SolveSystem(System sys)
{
  // Initialize the SUNDIALS variables
  double t;
  SUNContext sunctx;
  N_Vector abstol;
  abstol = NULL;
  SUNMatrix A;
  A = NULL;
  SUNLinearSolver LS;
  LS = NULL;
  void *cvode_mem;
  cvode_mem = NULL;

  int noOfDiffEq = sys.GetNoOfDiffEq();
  N_Vector y = sys.GetYInitCond();

  // Create SUNDIALS context
  int retval = SUNContext_Create(NULL, &sunctx);
  if (CheckReturnValue(&retval, "SUNContext_Create", 1))
    return (1);

  sys.AddSUNContext(sunctx);

  // Absolute tolerance
  N_Vector absTol = sys.GetObjAbsTol();
  if (CheckReturnValue((void *)absTol, "GetEqAbsTol", 0))
    return (1);

  // Relative tolerance
  double relTol = sys.GetRelTol();

  // Call CVodeCreate to create the solver memory and specify the
  // solver scheme
  cvode_mem = CVodeCreate(CV_BDF, sunctx);
  if (CheckReturnValue((void *)cvode_mem, "CVodeCreate", 0))
    return (1);

  // Give user data to the solver function
  System *sysPtr = &sys;
  retval = CVodeSetUserData(cvode_mem, sysPtr);
  if (CheckReturnValue(&retval, "CVodeSetUserData", 1))
    return (1);

  // Call CVodeInit to initialize the integrator memory and specify the
  // user's right hand side function in y'=f(t,y), the initial time T0, and
  // the initial dependent variable vector y
  retval = CVodeInit(cvode_mem, fFunction, startTime, y);
  if (CheckReturnValue(&retval, "CVodeInit", 1))
    return (1);

  // Call CVodeSVtolerances to specify the scalar relative tolerance
  // and vector absolute tolerances
  retval = CVodeSVtolerances(cvode_mem, relTol, absTol);
  if (CheckReturnValue(&retval, "CVodeSVtolerances", 1))
    return (1);

  // Create dense SUNMatrix for use in linear solves
  A = SUNDenseMatrix(noOfDiffEq, noOfDiffEq, sunctx);
  if (CheckReturnValue((void *)A, "SUNDenseMatrix", 0))
    return (1);

  // Create dense SUNLinearSolver object for use by CVode
  LS = SUNLinSol_Dense(y, A, sunctx);
  if (CheckReturnValue((void *)LS, "SUNLinSol_Dense", 0))
    return (1);

  // Attach the matrix and linear solver
  retval = CVodeSetLinearSolver(cvode_mem, LS, A);
  if (CheckReturnValue(&retval, "CVodeSetLinearSolver", 1))
    return (1);

  // In loop, call CVode, print results, and test for error.
  // Break out of loop when NOUT preset output times have been reached.
  printf(" \nSolving the system\n\n");

  // Impose a maximum timestep of stepTime
  CVodeSetMaxStep(cvode_mem, stepTime);

  Printer printer = Printer(sys);
  printer.OpenFile();
  printer.PrintResultsHeader();

  auto startTimer = std::chrono::high_resolution_clock::now();
  while (1)
  {

    retval = CVode(cvode_mem, outTime, y, &t, CV_NORMAL);
    if (CheckReturnValue(&retval, "CVode", 1))
      break;

    double bar = 1e-5;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "t: " << outTime << " || HPChamber: " << Ith(y, 1) * bar << " || LPChamber: " << Ith(y, 2) * bar;
    std::cout << " || inletChamber: " << Ith(y, 3) * bar << " || variableChamber: " << Ith(y, 4) * bar << " || outletChamber: " << Ith(y, 5) * bar << std::endl;

    printer.PrintResults(outTime);

    if (retval == CV_SUCCESS)
    {
      outTime = outTime + stepTime;
    }

    if (outTime > endTime)
    {
      auto nowTime = std::chrono::high_resolution_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - startTimer);
      std::cout << "Simulation time: " << elapsed.count() << " ms" << std::endl;
      break;
    }
  }

  // Free memory
  N_VDestroy(y);            // Free y vector
  N_VDestroy(abstol);       // Free abstol vector
  CVodeFree(&cvode_mem);    // Free CVODE memory
  SUNLinSolFree(LS);        // Free the linear solver memory
  SUNMatDestroy(A);         // Free the matrix memory
  SUNContext_Free(&sunctx); // Free the SUNDIALS context

  return (retval);
}

int Solver::fFunction(sunrealtype t, N_Vector y, N_Vector ydot, void *user_data)
{
  // This function computes the ODE right-hand side for a given value of the independent variable
  //  and state vector
  // Arguments:
  // t – is the current value of the independent variable.
  // y – is the current value of the dependent variable vector,
  // ydot – is the output vector
  // user_data – is the user_data pointer passed to CVodeSetUserData().
  // Return value:
  // A CVRhsFn should return 0 if successful, a positive value if a recoverable error occurred (in
  // which case CVODE will attempt to correct), or a negative value if it failed unrecoverably (in
  // which case the integration is halted and CV_RHSFUNC_FAIL is returned).

  // Cast the user_data void pointer to a pointer to system
  System *sysPtr = static_cast<System *>(user_data);

  sunrealtype y1 = Ith(y, 1 );
  sunrealtype y2 = Ith(y, 2 );
  sunrealtype y3 = Ith(y, 3 );
  sunrealtype y4 = Ith(y, 4 );
  sunrealtype y5 = Ith(y, 5 );
  sunrealtype y6 = Ith(y, 6 );
  sunrealtype y7 = Ith(y, 7 );
  sunrealtype y8 = Ith(y, 8 );
  sunrealtype y9 = Ith(y, 9 );
  sunrealtype y10 = Ith(y,10);
  sunrealtype y11 = Ith(y,11);
  sunrealtype y12 = Ith(y,12);
  sunrealtype y13 = Ith(y,13);
  sunrealtype y14 = Ith(y,14);
  sunrealtype y15 = Ith(y,15);
  sunrealtype y16 = Ith(y,16);
  sunrealtype y17 = Ith(y,17);

  // If the ydot pointer stored in the system is not the same as the ydot pointer used by the solver
  if (sysPtr->GetYDot() != ydot)
  {
    // Save the current ydot pointer in the system
    sysPtr->SetYDot(ydot);

    // Initialize the dependent variable pointers
    sysPtr->ConnectYDotToDepVarDeriv(ydot);
  }

  // Set new depenedent variable and reset equation factors
  sysPtr->ResetDiffEq(y);

  // Calculate RHS from the system
  sysPtr->CalculateAuxEqRHS();
  sysPtr->CalculateDiffEqRHS();

  sunrealtype yDot1 = Ith(ydot, 1 );
  sunrealtype yDot2 = Ith(ydot, 2 );
  sunrealtype yDot3 = Ith(ydot, 3 );
  sunrealtype yDot4 = Ith(ydot, 4 );
  sunrealtype yDot5 = Ith(ydot, 5 );
  sunrealtype yDot6 = Ith(ydot, 6 );
  sunrealtype yDot7 = Ith(ydot, 7 );
  sunrealtype yDot8 = Ith(ydot, 8 );
  sunrealtype yDot9 = Ith(ydot, 9 );
  sunrealtype yDot10 = Ith(ydot,10);
  sunrealtype yDot11 = Ith(ydot,11);
  sunrealtype yDot12 = Ith(ydot,12);
  sunrealtype yDot13 = Ith(ydot,13);
  sunrealtype yDot14 = Ith(ydot,14);
  sunrealtype yDot15 = Ith(ydot,15);
  sunrealtype yDot16 = Ith(ydot,16);
  sunrealtype yDot17 = Ith(ydot,17);

  return (0);
}

int Solver::CheckReturnValue(void *returnvalue, const char *funcname, int opt)
{
  int *retval;

  /* Check if SUNDIALS function returned NULL pointer - no memory allocated */
  if (opt == 0 && returnvalue == NULL)
  {
    fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n\n",
            funcname);
    return (1);
  }

  /* Check if retval < 0 */
  else if (opt == 1)
  {
    retval = (int *)returnvalue;
    if (*retval < 0)
    {
      fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed with retval = %d\n\n",
              funcname, *retval);
      return (1);
    }
  }

  /* Check if function returned NULL pointer - no memory allocated */
  else if (opt == 2 && returnvalue == NULL)
  {
    fprintf(stderr, "\nMEMORY_ERROR: %s() failed - returned NULL pointer\n\n",
            funcname);
    return (1);
  }

  return (0);
}