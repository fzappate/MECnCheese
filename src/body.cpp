#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <nvector/nvector_serial.h> // access to serial N_Vector

#include "./body.h"

Body::Body(std::string name, sunrealtype mass) : DiffObject(name, 12)
{
    // Set initial conditions
    this->yValuesInit = std::vector<sunrealtype>(this->nDepVar, 0.0);
    this->yDotValuesInit = std::vector<sunrealtype>(this->nDepVar, 0.0);
    this->absTol = std::vector<sunrealtype>(this->nDepVar, 0.001);

    // Initialize the pointers that connect to the dependent variables in the system
    this->yValuesPnt = std::vector<sunrealtype *>(this->nDepVar, nullptr);
    this->yDotValuesPnt = std::vector<sunrealtype *>(this->nDepVar, nullptr);
    this->depVarIndexInSys = std::vector<sunindextype>(this->nDepVar, -1);

    this->mass = mass;

    return;
};

void Body::AddForce(std::string forceName, sunrealtype forceX, sunrealtype forceY, sunrealtype forceZ)
{
    this->forces.push_back(forceX);
    this->forces.push_back(forceY);
    this->forces.push_back(forceZ);
    this->forceNames.push_back(forceName);

    return;
};

void Body::CalculateRHS()
{
    *yDotValuesPnt[0] = *yValuesPnt[3];   // velX
    *yDotValuesPnt[1] = *yValuesPnt[4];   // velY
    *yDotValuesPnt[2] = *yValuesPnt[5];   // velZ
    *yDotValuesPnt[3] = 1/this->mass;   // accX
    *yDotValuesPnt[4] = 0/this->mass;   // accY
    *yDotValuesPnt[5] = 0/this->mass;   // accZ
    *yDotValuesPnt[6] = *yValuesPnt[9];   // velPhi
    *yDotValuesPnt[7] = *yValuesPnt[10];   // velTheta
    *yDotValuesPnt[8] = *yValuesPnt[11];   // velPsi 
    *yDotValuesPnt[9] = 0/this->mass;   // accPhi 
    *yDotValuesPnt[10] = 0/this->mass;  // accTheta
    *yDotValuesPnt[11] = 0/this->mass;  // accPsi 
    return;
};

void Body::ZeroParameters()
{
    return;
};

void Body::PrintHeader(std::ofstream &outputFile)
{
    if (printStruct.printPosX == 1)
    {
        outputFile << this->name << ":Position X:m, ";
    }
    if (printStruct.printPosY == 1)
    {
        outputFile << this->name << ":Position Y:m, ";
    }
    if (printStruct.printPosZ == 1)
    {
        outputFile << this->name << ":Position Z:m, ";
    }
    if (printStruct.printVelX == 1)
    {
        outputFile << this->name << ":Velocity X:m/s, ";
    }
    if (printStruct.printVelY == 1)
    {
        outputFile << this->name << ":Velocity Y:m/s, ";
    }
    if (printStruct.printVelZ == 1)
    {
        outputFile << this->name << ":Velocity Z:m/s, ";
    }
    if (printStruct.printAccX == 1)
    {
        outputFile << this->name << ":Acceleration X:m/s^2, ";
    }
    if (printStruct.printAccY == 1)
    {
        outputFile << this->name << ":Acceleration Y:m/s^2, ";
    }
    if (printStruct.printAccZ == 1)
    {
        outputFile << this->name << ":Acceleration Z:m/s^2, ";
    }
    if (printStruct.printPosPhi == 1)
    {
        outputFile << this->name << ":Angular Position Phi:rad, ";
    }
    if (printStruct.printPosTheta == 1)
    {
        outputFile << this->name << ":Angular Position Theta:rad, ";
    }
    if (printStruct.printPosPsi == 1)
    {
        outputFile << this->name << ":Angular Position Psi:rad, ";
    }
    if (printStruct.printVelPhi == 1)
    {
        outputFile << this->name << ":Angular Velocity Phi:rad/s, ";
    }
    if (printStruct.printVelTheta == 1)
    {
        outputFile << this->name << ":Angular Velocity Theta:rad/s, ";
    }
    if (printStruct.printVelPsi == 1)
    {
        outputFile << this->name << ":Angular Velocity Psi:rad/s, ";
    }
    if (printStruct.printAccPhi == 1)
    {
        outputFile << this->name << ":Angular Acceleration Phi:rad/s^2, ";
    }
    if (printStruct.printAccTheta == 1)
    {
        outputFile << this->name << ":Angular Acceleration Theta:rad/s^2, ";
    }
    if (printStruct.printAccPsi == 1)
    {
        outputFile << this->name << ":Angular Acceleration Psi:rad/s^2, ";
    }

    return;
};

void Body::PrintVariables(std::ofstream &outputFile)
{

    if (printStruct.printPosX == 1)
    {
        outputFile << *this->yValuesPnt[0] << ",";
    }
    if (printStruct.printPosY == 1)
    {
        outputFile << *this->yValuesPnt[1] << ",";
    }
    if (printStruct.printPosZ == 1)
    {
        outputFile << *this->yValuesPnt[2] << ",";
    }
    if (printStruct.printVelX == 1)
    {
        outputFile << *this->yValuesPnt[3] << ",";
    }
    if (printStruct.printVelY == 1)
    {
        outputFile << *this->yValuesPnt[4] << ",";
    }
    if (printStruct.printVelZ == 1)
    {
        outputFile << *this->yValuesPnt[5] << ",";
    }
    if (printStruct.printAccX == 1)
    {
        outputFile << *this->yDotValuesPnt[3] << ",";
    }
    if (printStruct.printAccY == 1)
    {
        outputFile << *this->yDotValuesPnt[4] << ",";
    }
    if (printStruct.printAccZ == 1)
    {
        outputFile << *this->yDotValuesPnt[5] << ",";
    }
    if (printStruct.printPosPhi == 1)
    {
        outputFile << *this->yValuesPnt[6] << ",";
    }
    if (printStruct.printPosTheta == 1)
    {
        outputFile << *this->yValuesPnt[7] << ",";
    }
    if (printStruct.printPosPsi == 1)
    {
        outputFile << *this->yValuesPnt[8] << ",";
    }
    if (printStruct.printVelPhi == 1)
    {
        outputFile << *this->yValuesPnt[9] << ",";
    }
    if (printStruct.printVelTheta == 1)
    {
        outputFile << *this->yValuesPnt[10] << ",";
    }
    if (printStruct.printVelPsi == 1)
    {
        outputFile << *this->yValuesPnt[11] << ",";
    }
    if (printStruct.printAccPhi == 1)
    {
        outputFile << *this->yDotValuesPnt[9] << ",";
    }
    if (printStruct.printAccTheta == 1)
    {
        outputFile << *this->yDotValuesPnt[10] << ",";
    }
    if (printStruct.printAccPsi == 1)
    {
        outputFile << *this->yDotValuesPnt[11] << ",";
    }

    return;
};

// Class ConstVelBody
ConstVelBody::ConstVelBody(std::string name, sunrealtype velX, sunrealtype velY, sunrealtype velZ) : Body(name, mass)
{
    // Override the initial velocity of the body
    this->yValuesInit[3] = velX;
    this->yValuesInit[4] = velY;
    this->yValuesInit[5] = velZ;

    return;
};

void ConstVelBody::CalculateRHS()
{
    *yDotValuesPnt[0] = yValuesInit[3];   // velX
    *yDotValuesPnt[1] = yValuesInit[4];   // velY
    *yDotValuesPnt[2] = yValuesInit[5];   // velZ
    *yDotValuesPnt[3] = 0/this->mass;   // accX
    *yDotValuesPnt[4] = 0/this->mass;   // accY
    *yDotValuesPnt[5] = 0/this->mass;   // accZ
    *yDotValuesPnt[6] = yValuesInit[9];   // velPhi
    *yDotValuesPnt[7] = yValuesInit[10];   // velTheta
    *yDotValuesPnt[8] = yValuesInit[11];   // velPsi 
    *yDotValuesPnt[9] = 0/this->mass;   // accPhi 
    *yDotValuesPnt[10] = 0/this->mass;  // accTheta
    *yDotValuesPnt[11] = 0/this->mass;  // accPsi 
    return;
};

// Class ConstRotVelBody
ConstRotVelBody::ConstRotVelBody(std::string name, sunrealtype velPhi, sunrealtype velTheta, sunrealtype velPsi) : Body(name, mass)
{
    // Override the initial rotational velocity of the body
    this->yValuesInit[9] = velPhi;
    this->yValuesInit[10] = velTheta;
    this->yValuesInit[11] = velPsi;

    return;
};

void ConstRotVelBody::CalculateRHS()
{
    *yDotValuesPnt[0] = yValuesInit[3];   // velX
    *yDotValuesPnt[1] = yValuesInit[4];   // velY
    *yDotValuesPnt[2] = yValuesInit[5];   // velZ
    *yDotValuesPnt[3] = 0/this->mass;   // accX
    *yDotValuesPnt[4] = 0/this->mass;   // accY
    *yDotValuesPnt[5] = 0/this->mass;   // accZ
    *yDotValuesPnt[6] = yValuesInit[9];   // velPhi
    *yDotValuesPnt[7] = yValuesInit[10];   // velTheta
    *yDotValuesPnt[8] = yValuesInit[11];   // velPsi 
    *yDotValuesPnt[9] = 0/this->mass;   // accPhi 
    *yDotValuesPnt[10] = 0/this->mass;  // accTheta
    *yDotValuesPnt[11] = 0/this->mass;  // accPsi 
    return;
};

