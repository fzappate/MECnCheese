#pragma once

#include <iostream>
#include <fstream>

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
  N_Vector y = sys.GetInitCondition();

  // Create SUNDIALS context
  int retval = SUNContext_Create(NULL, &sunctx);
  if (CheckReturnValue(&retval, "SUNContext_Create", 1))
    return (1);

  sys.AddSUNContext(sunctx);

  // Absolute tolerance
  N_Vector absTol = sys.GetEqAbsTol();
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
  retval = CVodeSetUserData(cvode_mem,sysPtr);
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

  Printer printer = Printer(sys);
  printer.OpenFile();
  printer.PrintResultsHeader();

  // std::string fileName = "Results.csv";
  // std::ofstream outputFile;
  // outputFile.open(fileName);
  // outputFile << "Writing this to a file.\n";
  // outputFile << "Time,p,\n";

  while (1)
  {

    retval = CVode(cvode_mem, outTime, y, &t, CV_NORMAL);
    if (CheckReturnValue(&retval, "CVode", 1))
      break;
      
    std::cout << "t: " << outTime << " p1: " << Ith(y, 1) << " p2: " << Ith(y, 2) << std::endl;
    printer.PrintResults();
    // outputFile << outTime << "\t" << Ith(y, 1) << "\t" << Ith(y, 2) << std::endl;

    if (retval == CV_SUCCESS)
    {
      outTime = outTime + stepTime;
    }

    if (outTime > endTime)
    {
      break;
    }
  }

  // outputFile.close();

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
  System * sysPtr =  static_cast<System *>(user_data);
  // Dereference the pointer 
  System sys = *sysPtr;

  // Set new depenedent variable and reset equation factors
  sys.ResetDiffEq(y);

  // Calculate RHS from the system
  sys.CalculateAuxEqRHS();
  sys.CalculateDiffEqRHS();

  // Extract RHS from system and store it in ydot
  std::vector<sunrealtype> RHS = sys.GetDiffEqRHS();
  sunrealtype noOfDiffEq = sys.GetNoOfDiffEq();
  for (int ii = 0; ii < noOfDiffEq; ii++)
  {    
    Ith(ydot, ii + 1) = RHS[ii];
  };

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