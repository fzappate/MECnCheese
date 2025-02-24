#include "./axialpistonmachineconnection.h"
#include <iostream>
#include <cmath>
#include <numbers>  // Required for std::numbers::pi

// sunrealtype pi = 3.14159265358979323846;

AxialPistonMachineValvePlateConnection::AxialPistonMachineValvePlateConnection(std::string name,
    AxialPistonMachineControlVolume& pistonControlVolume,
    Chamber& downCh,
    LookUpTable& areaTable,
    sunindextype angleIndex,
    sunindextype areaIndex,
    sunindextype hdIndex) : Orifice(name, pistonControlVolume, downCh),
    pistonControlVolume(pistonControlVolume),
    areaTable(areaTable),
    angleIndex(angleIndex),
    areaIndex(areaIndex),
    hdIndex(hdIndex)
{
    return;
};

void AxialPistonMachineValvePlateConnection::CalculateRHS()
{
    // Find index and weights in the table 
    this->FindInterpolationIndexes();

    // Get interpolated area, diameter, 
    this->SetArea(areaTable.GetInterpolatedValue(this->areaIndex, this->interpLowerIndex, this->interpCoeff));

    // Calculate flowrate 
    this->CalculateFlowrate();

};

void AxialPistonMachineValvePlateConnection::CalculateArea()
{
    //
    sunrealtype a = 9;
    return;
};

void AxialPistonMachineValvePlateConnection::FindInterpolationIndexes()
{
    // FIND INDEX OF THE ANGLE
       // Get the piston angle
    sunrealtype pistonAngle = this->pistonControlVolume.GetControlVolumeAngPosition();
    // Find the relative piston angle 
    pistonAngle = std::fmod(pistonAngle, 2 * 3.14159265358979323846);

    // Get the characteristics of the table
    sunindextype noOfRow = this->areaTable.noOfRow;
    sunindextype noOfCol = this->areaTable.noOfCol;


    std::vector<sunrealtype> tableAngle = this->areaTable.GetTableColumn(this->angleIndex);

    // Get the exact interpolation weights 
    sunrealtype lowerAngle;
    sunrealtype upperAngle;

    for (int ii = 0; ii < noOfRow; ii++)
    {
        if (tableAngle[ii] >= pistonAngle)
        {
            if (ii==0)
            {
                this->interpLowerIndex = ii;
                this->interpCoeff = 0;
            }
            else
            {
                this->interpLowerIndex = ii - 1;
                lowerAngle = tableAngle[ii - 1];
                upperAngle = tableAngle[ii];
                this->interpCoeff = (pistonAngle - lowerAngle) / (upperAngle - lowerAngle);
            }
            std::cout << "Angle index: " << ii << std::endl;
            break;
        }

    };

};