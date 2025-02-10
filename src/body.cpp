#pragma once

#include <nvector/nvector_serial.h> // access to serial N_Vector

#include "./body.h"

Body::Body(std::string name) : DiffObject(name, 12)
{
    // Set initial conditions
    this->yValuesInit = std::vector<sunrealtype>(this->nDepVar, 0.0);
    this->yDotValuesInit = std::vector<sunrealtype>(this->nDepVar, 0.0);
    this->absTol = std::vector<sunrealtype>(this->nDepVar, 0.001);

    // Initialize the pointers that connect to the dependent variables in the system
    this->yValuesPnt = std::vector<sunrealtype *>(this->nDepVar, nullptr);
    this->yDotValuesPnt = std::vector<sunrealtype *>(this->nDepVar, nullptr);
    this->depVarIndexInSys = std::vector<sunindextype>(this->nDepVar, -1);

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
    *yDotValuesPnt[0] = 0/this->mass;   // posX
    *yDotValuesPnt[1] = 0/this->mass;   // posY
    *yDotValuesPnt[2] = 0/this->mass;   // posZ
    *yDotValuesPnt[3] = 0/this->mass;   // velX
    *yDotValuesPnt[4] = 0/this->mass;   // velY
    *yDotValuesPnt[5] = 0/this->mass;   // velZ
    *yDotValuesPnt[6] = 0/this->mass;   // posPhi
    *yDotValuesPnt[7] = 0/this->mass;   // posTheta
    *yDotValuesPnt[8] = 0/this->mass;   // posPsi 
    *yDotValuesPnt[9] = 0/this->mass;   // velPhi 
    *yDotValuesPnt[10] = 0/this->mass;  // velTheta
    *yDotValuesPnt[11] = 0/this->mass;  // velPsi 
    return;
};

void Body::ZeroParameters()
{
    return;
};

void Body::PrintHeader(std::ofstream &outputFile)
{
    return;
};

void Body::PrintVariables(std::ofstream &outputFile)
{
    return;
};

