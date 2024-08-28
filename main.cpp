#include <math.h>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "./orifice.h"
#include "./chamber.h"
#include "./equation.h"
#include "./system.h"
#include "./solver.h"

// Define handy macros
#define PI 3.1415926535897932
#define Ith(v, i) NV_Ith_S(v, i - 1)                /* i-th vector component i=1..NEQ */
#define IJth(A, i, j) SM_ELEMENT_D(A, i - 1, j - 1) /* (i,j)-th matrix component i,j=1..NEQ */

int main()
{

  // Create system
  System sys = System();

  InfChamber HPChamber = InfChamber("HPChamber", 20*1e5);
  sys.AddEquation(HPChamber);

  InfChamber LPChamber = InfChamber("LPChamber", 1*1e5);
  sys.AddEquation(LPChamber);

  ConstChamber chamber1 = ConstChamber("MidCh1", 2*1e5, 10);
  sys.AddEquation(chamber1);

  ConstChamber chamber2 = ConstChamber("MidCh2", 2*1e5, 10);
  sys.AddEquation(chamber2);

  Orifice midOrif = Orifice("UpOrif", 5*1e-6, chamber1, chamber2);
  sys.AddEquation(midOrif);

  Orifice upOrif = Orifice("UpOrif", 5*1e-6, HPChamber, chamber1);
  sys.AddEquation(upOrif);

  Orifice downOrif = Orifice("DownOrif", 5*1e-6, chamber2, LPChamber);
  sys.AddEquation(downOrif);

  
  Solver solver = Solver(0.01,1.0);
  int retVal = solver.SolveSystem(sys);

  return retVal;
  
}

