#pragma once

#include "./object.h"

class Body : public DiffObject
{
protected:
    // X: object X coordinate in the parent reference system
    // Y: object Y coordinate in the parent reference system
    // Z: object Z coordinate in the parent reference system
    // Phi: object angle around the X axis of the parent reference system
    // Theta: object angle around the Y axis of the parent reference system
    // Psi: object angle around the Z axis of the parent reference system
    // jj is the number of equations before the body object equations
    //
    // posX: stored in N_Vector y[jj+0]
    // posY: stored in N_Vector y[jj+1]
    // posZ: stored in N_Vector y[jj+2]
    // velX: stored in N_Vector y[jj+3]
    // velY: stored in N_Vector y[jj+4]
    // velZ: stored in N_Vector y[jj+5]
    // accX: stored in N_Vector ydot[jj+3]
    // accY: stored in N_Vector ydot[jj+4]
    // accZ: stored in N_Vector ydot[jj+5]
    //
    // posPhi:  stored in N_Vector y[jj+6]
    // posTheta:stored in N_Vector y[jj+7]
    // posPsi:  stored in N_Vector y[jj+8]
    // velPhi:  stored in N_Vector y[jj+9]
    // velTheta:stored in N_Vector y[jj+10]
    // velPsi:  stored in N_Vector y[jj+11]
    // accPhi:  stored in N_Vector ydot[jj+9]
    // accTheta:stored in N_Vector ydot[jj+10]
    // accPsi:  stored in N_Vector ydot[jj+11]

    sunrealtype mass;
    sunrealtype Ixx;
    sunrealtype Iyy;
    sunrealtype Izz;

    std::vector<sunrealtype> forces;
    std::vector<std::string> forceNames;

    struct PrintStruct
    {
        bool printPosX = 1;
        bool printPosY = 1;
        bool printPosZ = 1;
        bool printVelX = 1;
        bool printVelY = 1;
        bool printVelZ = 1;
        bool printAccX = 1;
        bool printAccY = 1;
        bool printAccZ = 1;
        bool printPosPhi = 1;
        bool printPosTheta = 1;
        bool printPosPsi = 1;
        bool printVelPhi = 1;
        bool printVelTheta = 1;
        bool printVelPsi = 1;
        bool printAccPhi = 1;
        bool printAccTheta = 1;
        bool printAccPsi = 1;
    };
    PrintStruct printStruct;

public:
    // Default constructor
    Body();

    // Constructor
    Body(std::string name, sunrealtype mass);

    // Add force to the object
    void AddForce(std::string forceName, sunrealtype forceX, sunrealtype forceY, sunrealtype forceZ);

    // Calculate the right-hand side of the Body object equations
    void CalculateRHS();

    // Zero the forces applied to the Body object
    void ZeroParameters();

    sunrealtype GetPosPsi();

    // Print the header of the Body object variable in the output file
    void PrintHeader(std::ofstream &outputFile);

    // Print the variables of the Body object in the output file
    void PrintVariables(std::ofstream &outputFile);
};

class ConstVelBody : public Body
{
public:
    // Constructor
    ConstVelBody(std::string name, sunrealtype velX, sunrealtype velY, sunrealtype velZ);

    // Calculate the right-hand side of the Body object equations
    void CalculateRHS() override;
};

class ConstRotVelBody : public Body
{
public:
    // Constructor
    ConstRotVelBody(std::string name, sunrealtype velX, sunrealtype velY, sunrealtype velZ);

    // Calculate the right-hand side of the Body object equations
    void CalculateRHS() override;
};
