#include <math.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sundials/sundials_math.h>    // Import math functions
#include <cvode/cvode.h>               // prototypes for CVODE fcts., consts.
#include <nvector/nvector_serial.h>    // access to serial N_Vector
#include <sunmatrix/sunmatrix_dense.h> // access to dense SUNMatrix
#include <sunlinsol/sunlinsol_dense.h> // access to dense SUNLinearSolver

#include "./orifice.h"
#include "./chamber.h"
#include "./equation.h"
#include "./system.h"

// Define handy macros
#define PI 3.1415926535897932
#define Ith(v, i) NV_Ith_S(v, i - 1)                /* i-th vector component i=1..NEQ */
#define IJth(A, i, j) SM_ELEMENT_D(A, i - 1, j - 1) /* (i,j)-th matrix component i,j=1..NEQ */

// Problem Constants
#define RTOL RCONST(1.0e-4) // scalar relative tolerance
#define ATOL RCONST(1.0e-8) // vector absolute tolerance components

// Functions Called by the Solver
static int hydraulic_circuit(sunrealtype t, N_Vector y, N_Vector ydot, void *user_data);
static void PrintOutput(sunrealtype t, sunrealtype y1);
static int check_retval(void *returnvalue, const char *funcname, int opt);

int main()
{
  // Initialize the SUNDIALS variables
  SUNContext sunctx;
  N_Vector abstol;
  abstol = NULL;
  SUNMatrix A;
  A = NULL;
  SUNLinearSolver LS;
  LS = NULL;
  void *cvode_mem;
  cvode_mem = NULL;

  // Define constant 
  sunrealtype t;
  sunrealtype tstart = 0;
  sunrealtype tout = 0.01;
  sunrealtype tEnd = 10;

  // Create SUNDIALS context
  int retval = SUNContext_Create(NULL, &sunctx);
  if (check_retval(&retval, "SUNContext_Create", 1))
    return (1);

  // Create system
  System sys = System(sunctx);

  InfChamber HPChamber = InfChamber("HPChamber", 10*1e5);
  sys.AddEquation(HPChamber);
  InfChamber LPChamber = InfChamber("LPChamber", 1*1e5);
  sys.AddEquation(LPChamber);
  ConstChamber chamber = ConstChamber("MidCh", 2*1e5, 1e-3);
  sys.AddEquation(chamber);
  Orifice upOrif = Orifice("UpOrif", 5*1e-6, HPChamber, chamber);
  sys.AddEquation(upOrif);
  Orifice downOrif = Orifice("DownOrif", 5*1e-6, chamber, LPChamber);
  sys.AddEquation(downOrif);

  int noOfDiffEq = sys.noOfDiffEq;
  N_Vector y = sys.GetInitCondition();
  

  // Absolute tolerance
  N_Vector absTol = sys.GetEqAbsTol();
  if (check_retval((void *)absTol, "GetEqAbsTol", 0))
    return (1);

  // Relative tolerance
  double relTol = sys.GetRelTol();

  // Call CVodeCreate to create the solver memory and specify the
  // solver scheme
  cvode_mem = CVodeCreate(CV_BDF, sunctx);
  if (check_retval((void *)cvode_mem, "CVodeCreate", 0))
    return (1);

  // Give user data to the solver function 
  System *sysPtr = &sys;
  retval = CVodeSetUserData(cvode_mem,sysPtr);
  if (check_retval(&retval, "CVodeSetUserData", 1))
    return (1);

  // Call CVodeInit to initialize the integrator memory and specify the
  // user's right hand side function in y'=f(t,y), the initial time T0, and
  // the initial dependent variable vector y
  retval = CVodeInit(cvode_mem, hydraulic_circuit, tstart, y);
  if (check_retval(&retval, "CVodeInit", 1))
    return (1);

  // Call CVodeSVtolerances to specify the scalar relative tolerance
  // and vector absolute tolerances 
  retval = CVodeSVtolerances(cvode_mem, relTol, absTol);
  if (check_retval(&retval, "CVodeSVtolerances", 1))
    return (1);

  // Create dense SUNMatrix for use in linear solves
  A = SUNDenseMatrix(noOfDiffEq, noOfDiffEq, sunctx);
  if (check_retval((void *)A, "SUNDenseMatrix", 0))
    return (1);

  // Create dense SUNLinearSolver object for use by CVode
  LS = SUNLinSol_Dense(y, A, sunctx);
  if (check_retval((void *)LS, "SUNLinSol_Dense", 0))
    return (1);

  // Attach the matrix and linear solver
  retval = CVodeSetLinearSolver(cvode_mem, LS, A);
  if (check_retval(&retval, "CVodeSetLinearSolver", 1))
    return (1);

  // In loop, call CVode, print results, and test for error.
  // Break out of loop when NOUT preset output times have been reached.
  printf(" \n3-hydraulic_circuit problem\n\n");

  std::string fileName = "hydraulic_circuit_results.csv";
  std::ofstream outputFile;
  outputFile.open(fileName);
  outputFile << "Writing this to a file.\n";
  outputFile << "Time,p,\n";

  while (1)
  {

    retval = CVode(cvode_mem, tout, y, &t, CV_NORMAL);
    if (check_retval(&retval, "CVode", 1))
      break;
      
    std::cout << "t: " << t << " p: " << Ith(y, 1) << std::endl;
    outputFile << tout << "\t" << Ith(y, 1) << std::endl;

    if (retval == CV_SUCCESS)
    {
      tout = tout + 0.01;
    }

    if (tout > tEnd)
    {
      break;
    }
  }

  outputFile.close();

  // Free memory
  N_VDestroy(y);            // Free y vector
  N_VDestroy(abstol);       // Free abstol vector
  CVodeFree(&cvode_mem);    // Free CVODE memory
  SUNLinSolFree(LS);        // Free the linear solver memory 
  SUNMatDestroy(A);         // Free the matrix memory 
  SUNContext_Free(&sunctx); // Free the SUNDIALS context 

  return (retval);
}

static int hydraulic_circuit(sunrealtype t, N_Vector y, N_Vector ydot, void *user_data)
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
  System * sysDerefPtr =  static_cast<System *>(user_data);
  // Dereference the pointer 
  System sysDeref = *sysDerefPtr;

  // Set new depenedent variable and reset equation factors
  sysDeref.ResetDiffEq(y);

  // Calculate RHS from the system
  sysDeref.CalculateAuxEqRHS();
  sysDeref.CalculateDiffEqRHS();

  // Extract RHS from system and store it in ydot
  std::vector<sunrealtype> RHS = sysDeref.GetDiffEqRHS();
  sunrealtype noOfDiffEq = sysDeref.noOfDiffEq;
  for (int ii = 0; ii < noOfDiffEq; ii++)
  {    
    Ith(ydot, ii + 1) = RHS[ii];
  };

  return (0);
}

static void PrintOutput(sunrealtype t, sunrealtype y1)
{
#if defined(SUNDIALS_EXTENDED_PRECISION)
  printf("At t = %0.4Le      y =%14.6Le  \n", t, y1);
#elif defined(SUNDIALS_DOUBLE_PRECISION)
  printf("At t = %0.4e      y =%14.6e \n", t, y1);
#else
  printf("At t = %0.4e      y =%14.6e \n", t, y1);
#endif

  return;
}

//  Check function return value
//    opt == 0 means SUNDIALS function allocates memory so check if
//             returned NULL pointer
//    opt == 1 means SUNDIALS function returns an integer value so check if
//             retval < 0
//    opt == 2 means function allocates memory so check if returned
//             NULL pointer 
static int check_retval(void *returnvalue, const char *funcname, int opt)
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
