
#include "./orifice.h"
#include "./chamber.h"
#include "./system.h"
#include "./solver.h"
#include "./inputreader.h"
#include "./body.h"
#include "./axialpistonmachinebody.h"
#include "./axialpistonmachinecv.h"

sunrealtype pi = 3.14159265358979323846;

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

  sunrealtype casingRotVel = 1; // RPS
  sunrealtype casingRotVelRad = casingRotVel * 2 * pi; // rad/s
  AxialPistonMachineCasing casing = AxialPistonMachineCasing("Casing", casingRotVelRad);
  sys.AddObject(casing);

  sunrealtype swashPlateAngle = 20; // deg
  sunrealtype swashPlateAngleRad = swashPlateAngle * pi / 180; // rad
  AxialPistonMachineSwashPlate swashPlate = AxialPistonMachineSwashPlate("SwashPlate", swashPlateAngleRad);
  sys.AddObject(swashPlate);

  InfChamber HPChamber = InfChamber("HPChamber",
                                    HPChamber_Pressure);
  sys.AddObject(HPChamber);

  InfChamber LPChamber = InfChamber("LPChamber",
                                    LPChamber_Pressure);
  sys.AddObject(LPChamber);

  ConstChamber inletChamber = ConstChamber("inletChamber",
                                           inletChamber_Pressure,
                                           inletChamber_Volume);
  sys.AddObject(inletChamber);

  AxialPistonMachineControlVolume variableChamber = AxialPistonMachineControlVolume("variableChamber",
                                                                                    casing,
                                                                                    0.0,
                                                                                    0.01, 
                                                                                    inletChamber_Pressure,
                                                                                    inletChamber_Volume);
  sys.AddObject(variableChamber);

  ConstChamber outletChamber = ConstChamber("outletChamber",
                                            outletChamber_Pressure,
                                            outletChamber_Volume);
  sys.AddObject(outletChamber);

  Orifice inletPort = Orifice("inletPort",
                              inletPort_Area,
                              HPChamber,
                              inletChamber);
  sys.AddObject(inletPort);

  Orifice inletOrif = Orifice("inletOrif",
                              inletOrif_Area,
                              inletChamber,
                              variableChamber);
  sys.AddObject(inletOrif);

  Orifice outletOrif = Orifice("outletOrif",
                               outletOrif_Area,
                               variableChamber,
                               outletChamber);
  sys.AddObject(outletOrif);

  Orifice outletPort = Orifice("outletPort",
                               outletPort_Area,
                               outletChamber,
                               LPChamber);
  sys.AddObject(outletPort);

  // Move sys dependent variables into N_Vector
  sys.ConnectYToDepVar();

  // Initialize system
  Solver solver = Solver(0.01, 1.0);

  // Solve system
  int retVal = solver.SolveSystem(sys);

  return retVal;
}
