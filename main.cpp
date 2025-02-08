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
  double inletChamber_Pressure = input.ImportVarAsDouble("inletChamber_Pressure");
  double inletChamber_Volume = input.ImportVarAsDouble("inletChamber_Volume");
  double variableChamber_Pressure = input.ImportVarAsDouble("variableChamber_Pressure");
  double variableChamber_Volume = input.ImportVarAsDouble("variableChamber_Volume");
  double outletChamber_Pressure = input.ImportVarAsDouble("outletChamber_Pressure");
  double outletChamber_Volume = input.ImportVarAsDouble("outletChamber_Volume");
  double inletPort_Area = input.ImportVarAsDouble("inletPort_Area");
  double inletOrif_Area = input.ImportVarAsDouble("inletOrif_Area");
  double outletOrif_Area = input.ImportVarAsDouble("outletOrif_Area");
  double outletPort_Area = input.ImportVarAsDouble("outletPort_Area");

  // Create system
  System sys = System();

  InfChamber HPChamber = InfChamber("HPChamber", 
                                    HPChamber_Pressure);
  sys.AddEquation(HPChamber);

  InfChamber LPChamber = InfChamber("LPChamber", 
                                    LPChamber_Pressure);
  sys.AddEquation(LPChamber);

  ConstChamber inletChamber = ConstChamber( "inletChamber", 
                                        inletChamber_Pressure, 
                                        inletChamber_Volume);
  sys.AddEquation(inletChamber);

  ConstChamber variableChamber = ConstChamber( "variableChamber", 
                                        inletChamber_Pressure, 
                                        inletChamber_Volume);
  sys.AddEquation(variableChamber);

  ConstChamber outletChamber = ConstChamber("outletChamber", 
                                        outletChamber_Pressure, 
                                        outletChamber_Volume);
  sys.AddEquation(outletChamber);

  Orifice inletPort = Orifice("inletPort", 
                            inletPort_Area, 
                            HPChamber, 
                            inletChamber);
  sys.AddEquation(inletPort);

  Orifice inletOrif = Orifice("inletOrif", 
                            inletOrif_Area, 
                            inletChamber, 
                            variableChamber);
  sys.AddEquation(inletOrif);

  Orifice outletOrif = Orifice("outletOrif", 
                              outletOrif_Area, 
                              variableChamber, 
                              outletChamber);
  sys.AddEquation(outletOrif);

  Orifice outletPort = Orifice("outletPort", 
                              outletPort_Area, 
                              outletChamber, 
                              LPChamber);
  sys.AddEquation(outletPort);

  // Move sys dependent variables into N_Vector
  sys.ConnectYToDepVar();
  
  // Initialize system 
  Solver solver = Solver(0.01,1.0);

  // Solve system
  int retVal = solver.SolveSystem(sys);

  return retVal;
  
}

