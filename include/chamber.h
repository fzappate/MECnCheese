#pragma once

#include <fstream>
#include <string>
#include <vector>

#include <nvector/nvector_serial.h> // access to serial N_Vector

#include "./object.h"



class Chamber : public DiffObject 
{
    protected:
    int noOfEq = 1;
    double bulkMod = 1.8*1.0E9;
    double pressure; 
    double volume;
    double volDer;
    double dpdt;
    double flowSum = 0;
    double aTol = 1;
    double rTol = 0.001;
    
    std::vector<double> flowIn;
    std::vector<std::string> flowInNames;

    struct PrintStruct
    {
        bool printBulkMod = 1;
        bool printPress = 1;
        bool printVolume = 1;
        bool printVolDer = 1;
        bool printFlowSum = 1;
    };
    PrintStruct printStruct;

    public:

    Chamber( std::string name, double pressure);

    // void SetPressure(double pressure);

    double GetPressure();

    virtual void AddFlowIn(std::string name, double flow);

    // Base class methods

    void CalculateRHS() override;

    void ZeroParameters() override;

    void PrintHeader(std::ofstream& outputFile) override;

    void PrintVariables(std::ofstream& outputFile) override;

};

// Class InfChamber
class InfChamber : public Chamber
{
public:

    InfChamber(std::string name,double pressure);
    
    void AddFlowIn(std::string name, double flow) override;

    void CalculateRHS() override;
};

// Class Chamber
class ConstChamber : public Chamber
{


public:
    
    ConstChamber(std::string name, double pressure, double volume);   

    void CalculateRHS() override;
    

};

