#include "./orifice.h"
#include "./chamber.h"
#include "./equation.h"
#include "./system.h"
#include "./solver.h"
#include "./inputreader.h"

int main()
{
  // Inputs
  InputReader input = InputReader();
  input.ReadInput();
  int HPChamber_Pressure = input.ImportVarAsInt("HPChamber");

  // Create system
  System sys = System();

  InfChamber HPChamber = InfChamber("HPChamber", HPChamber_Pressure);
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

  // Solve system 
  Solver solver = Solver(0.01,1.0);
  int retVal = solver.SolveSystem(sys);
  
  return retVal;
  
}

