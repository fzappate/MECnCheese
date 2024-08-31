#include "./orifice.h"
#include "./chamber.h"
#include "./equation.h"
#include "./system.h"
#include "./solver.h"
#include "./inputreader.h"

int main()
{
  // Inputs
  InputReader input = InputReader("Input.md");
  double HPChamber_Pressure = input.ImportVarAsDouble("HPChamber_Pressure");
  double LPChamber_Pressure = input.ImportVarAsDouble("LPChamber_Pressure");
  double chamber1_Pressure = input.ImportVarAsDouble("chamber1_Pressure");
  double chamber1_Volume = input.ImportVarAsDouble("chamber1_Volume");
  double chamber2_Pressure = input.ImportVarAsDouble("chamber2_Pressure");
  double chamber2_Volume = input.ImportVarAsDouble("chamber2_Volume");
  double upOrifArea = input.ImportVarAsDouble("upOrifArea");
  double midOrifArea = input.ImportVarAsDouble("midOrifArea");
  double downOrifArea = input.ImportVarAsDouble("downOrifArea");

  // Create system
  System sys = System();

  InfChamber HPChamber = InfChamber("HPChamber", 
                                    HPChamber_Pressure);
  sys.AddEquation(HPChamber);

  InfChamber LPChamber = InfChamber("LPChamber", 
                                    LPChamber_Pressure);
  sys.AddEquation(LPChamber);

  ConstChamber chamber1 = ConstChamber( "MidCh1", 
                                        chamber1_Pressure, 
                                        chamber1_Volume);
  sys.AddEquation(chamber1);

  ConstChamber chamber2 = ConstChamber("MidCh2", 
                                        chamber2_Pressure, 
                                        chamber2_Volume);
  sys.AddEquation(chamber2);

  Orifice midOrif = Orifice("UpOrif", 
                            midOrifArea, 
                            chamber1, 
                            chamber2);
  sys.AddEquation(midOrif);

  Orifice upOrif = Orifice("UpOrif", 
                            upOrifArea, 
                            HPChamber, 
                            chamber1);
  sys.AddEquation(upOrif);

  Orifice downOrif = Orifice("DownOrif", 
                              downOrifArea, 
                              chamber2, 
                              LPChamber);
  sys.AddEquation(downOrif);

  // Solve system 
  Solver solver = Solver(0.01,1.0);
  int retVal = solver.SolveSystem(sys);

  return retVal;
  
}

