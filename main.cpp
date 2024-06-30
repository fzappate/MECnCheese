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

#define PI 3.1415926535897932
#define Ith(v, i) NV_Ith_S(v, i - 1)                /* i-th vector component i=1..NEQ */
#define IJth(A, i, j) SM_ELEMENT_D(A, i - 1, j - 1) /* (i,j)-th matrix component i,j=1..NEQ */

// Problem Constants
#define NEQ 1          // number of equations
#define Y1 RCONST(1.0) // initial y components
#define RTOL RCONST(1.0e-4) // scalar relative tolerance
#define ATOL RCONST(1.0e-8) // vector absolute tolerance components
#define T0 RCONST(0.0)      // initial time
#define T1 RCONST(0.01)     // first output time
#define TMULT RCONST(10.0)  // output time factor
#define NOUT 1000           // number of output times
#define ZERO RCONST(0.0)

// Functions Called by the Solver

static int f(sunrealtype t, N_Vector y, N_Vector ydot, void *user_data);
static int mass_dumper(sunrealtype t, N_Vector y, N_Vector ydot, void *user_data);
static int hydraulic_circuit(sunrealtype t, N_Vector y, N_Vector ydot, void *user_data);

static void PrintOutput(sunrealtype t, sunrealtype y1);
static void PrintOutputToTxt(std::string fileName, sunrealtype t, sunrealtype y1, sunrealtype y2);

static int check_retval(void *returnvalue, const char *funcname, int opt);

int main()
{
  SUNContext sunctx;
  sunrealtype t, tout;
  N_Vector y;
  N_Vector abstol;
  SUNMatrix A;
  SUNLinearSolver LS;
  void *cvode_mem;
  int retval, iout;
  int retvalr;
  FILE *FID;

  y = NULL;
  abstol = NULL;
  A = NULL;
  LS = NULL;
  cvode_mem = NULL;

  Chamber chamber = Chamber("Chamber1", 7, 7);
  chamber.SetPressure(7);
  std::cout << "Chamber pressure: " << chamber.GetPressure() << std::endl;
  Orifice orifice = Orifice("Orifice1",10); 

  std::cout << "Orifice Cf: " << orifice.GetCf() << std::endl;

  // Create the SUNDIALS context
  retval = SUNContext_Create(NULL, &sunctx);
  if (check_retval(&retval, "SUNContext_Create", 1))
    return (1);

  // Initial conditions
  y = N_VNew_Serial(NEQ, sunctx);
  if (check_retval((void *)y, "N_VNew_Serial", 0))
    return (1);

  // Initialize y
  Ith(y, 1) = Y1;

  // Set the vector absolute tolerance
  abstol = N_VNew_Serial(NEQ, sunctx);
  if (check_retval((void *)abstol, "N_VNew_Serial", 0))
    return (1);

  Ith(abstol, 1) = ATOL;

  // Call CVodeCreate to create the solver memory and specify the
  // Backward Differentiation Formula
  cvode_mem = CVodeCreate(CV_BDF, sunctx);
  if (check_retval((void *)cvode_mem, "CVodeCreate", 0))
    return (1);

  // Call CVodeInit to initialize the integrator memory and specify the
  // user's right hand side function in y'=f(t,y), the initial time T0, and
  // the initial dependent variable vector y.
  retval = CVodeInit(cvode_mem, hydraulic_circuit, T0, y);
  if (check_retval(&retval, "CVodeInit", 1))
    return (1);

  // Call CVodeSVtolerances to specify the scalar relative tolerance
  // and vector absolute tolerances */
  retval = CVodeSVtolerances(cvode_mem, RTOL, abstol);
  if (check_retval(&retval, "CVodeSVtolerances", 1))
    return (1);

  // Call CVodeRootInit to specify the root function g with 2 components
  // retval = CVodeRootInit(cvode_mem, 2, g);
  // if (check_retval(&retval, "CVodeRootInit", 1)) return(1);

  // Create dense SUNMatrix for use in linear solves
  A = SUNDenseMatrix(NEQ, NEQ, sunctx);
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

  // Open file for printing statistics
  FID = fopen("hydraulic_circuit_statistics.csv", "w");

  iout = 0;
  tout = T1;
  std::string fileName = "hydraulic_circuit_results.csv";
  std::ofstream outputFile;
  outputFile.open(fileName);
  outputFile << "Writing this to a file.\n";
  outputFile << "Time,p,\n";

  while (1)
  {
    retval = CVode(cvode_mem, tout, y, &t, CV_NORMAL);
    // PrintOutput(t, Ith(y, 1)); // Print to screen
    //  PrintOutputToTxt( fileName, tout, Ith(y,1), Ith(y,2));

    outputFile << tout << "\t" << Ith(y, 1) << std::endl;

    if (check_retval(&retval, "CVode", 1))
      break;
    if (retval == CV_SUCCESS)
    {
      iout++;
      tout = tout + 0.01;
    }

    // retval = CVodePrintAllStats(cvode_mem, FID, SUN_OUTPUTFORMAT_CSV);

    if (iout == NOUT)
      break;
  }
  fclose(FID);
  outputFile.close();

  /* Print final statistics to the screen */
  printf("\nFinal Statistics:\n");
  // retval = CVodePrintAllStats(cvode_mem, stdout, SUN_OUTPUTFORMAT_TABLE);

  /* check the solution error */
  // retval = check_ans(y, t, RTOL, abstol);

  /* Free memory */
  N_VDestroy(y);            /* Free y vector */
  N_VDestroy(abstol);       /* Free abstol vector */
  CVodeFree(&cvode_mem);    /* Free CVODE memory */
  SUNLinSolFree(LS);        /* Free the linear solver memory */
  SUNMatDestroy(A);         /* Free the matrix memory */
  SUNContext_Free(&sunctx); /* Free the SUNDIALS context */

  return (retval);
}

/*
 *-------------------------------
 * Functions called by the solver
 *-------------------------------
 */

/*
 * f routine. Compute function f(t,y).
 */

static int f(sunrealtype t, N_Vector y, N_Vector ydot, void *user_data)
{
  realtype y1, y2, y3, yd1, yd3;

  y1 = Ith(y, 1);
  y2 = Ith(y, 2);
  y3 = Ith(y, 3);

  yd1 = Ith(ydot, 1) = RCONST(-0.04) * y1 + RCONST(1.0e4) * y2 * y3;
  yd3 = Ith(ydot, 3) = RCONST(3.0e7) * y2 * y2;
  Ith(ydot, 2) = -yd1 - yd3;

  return (0);
}

static int mass_dumper(sunrealtype t, N_Vector y, N_Vector ydot, void *user_data)
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
  realtype y1;
  realtype y2;
  realtype x1;
  realtype x2;

  y1 = Ith(y, 1);
  y2 = Ith(y, 2);
  x1 = Ith(y, 3);
  x2 = Ith(y, 4);

  realtype u = 10 * sin(t * 2 * PI);
  realtype m = 1;
  realtype c = 1;
  realtype k = 5;

  Ith(ydot, 1) = y2;
  Ith(ydot, 2) = (u - c * y2 - k * y1) / m;
  Ith(ydot, 3) = x2;
  Ith(ydot, 4) = (u - c * x2 - k * x1) / m;

  return (0);
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

  sunrealtype bulkMod = 1.5 * 1e9;
  sunrealtype vol1 = 1;
  sunrealtype volDer = 0;
  sunrealtype Qin1 = 0.002;
  sunrealtype Qout1 = 0.001;

  Ith(ydot, 1) = bulkMod / vol1 * (Qin1 - Qout1 - volDer);

  return (0);
}

// g routine. Compute functions g_i(t,y) for i = 0,1.
static int g(sunrealtype t, N_Vector y, sunrealtype *gout, void *user_data)
{
  realtype y1, y3;

  y1 = Ith(y, 1);
  y3 = Ith(y, 3);
  gout[0] = y1 - RCONST(0.0001);
  gout[1] = y3 - RCONST(0.01);

  return (0);
}

//* Jacobian routine. Compute J(t,y) = df/dy. *
static int Jac(sunrealtype t, N_Vector y, N_Vector fy, SUNMatrix J,
               void *user_data, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
  realtype y2, y3;

  y2 = Ith(y, 2);
  y3 = Ith(y, 3);

  IJth(J, 1, 1) = RCONST(-0.04);
  IJth(J, 1, 2) = RCONST(1.0e4) * y3;
  IJth(J, 1, 3) = RCONST(1.0e4) * y2;

  IJth(J, 2, 1) = RCONST(0.04);
  IJth(J, 2, 2) = RCONST(-1.0e4) * y3 - RCONST(6.0e7) * y2;
  IJth(J, 2, 3) = RCONST(-1.0e4) * y2;

  IJth(J, 3, 1) = ZERO;
  IJth(J, 3, 2) = RCONST(6.0e7) * y2;
  IJth(J, 3, 3) = ZERO;

  return (0);
}

/*
 *-------------------------------
 * Private helper functions
 *-------------------------------
 */

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

/*
 * Check function return value...
 *   opt == 0 means SUNDIALS function allocates memory so check if
 *            returned NULL pointer
 *   opt == 1 means SUNDIALS function returns an integer value so check if
 *            retval < 0
 *   opt == 2 means function allocates memory so check if returned
 *            NULL pointer
 */

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
