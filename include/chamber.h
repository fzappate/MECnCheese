#pragma once

#include <fstream>
#include <string>
#include <vector>



#include "./object.h"

class Chamber : public DiffObject
{
protected:
    sunrealtype bulkMod = 1.8 * 1.0E9;
    double initPressure;
    double volume = 0;
    double volDer = 0;
    double dpdt = 0;
    double flowSum = 0;
    double aTol = 1;

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
    Chamber(std::string name, double pressure);

    // void SetPressure(double pressure);

    double GetPressure();

    virtual void AddFlowIn(std::string name, double flow);

    // Base class methods

    void CalculateRHS() override;

    void ZeroParameters() override;

    void PrintHeader(std::ofstream &outputFile) override;

    void PrintVariables(std::ofstream &outputFile) override;
};

// Class InfChamber
class InfChamber : public Chamber
{
public:
    InfChamber(std::string name, double pressure);

    void AddFlowIn(std::string name, double flow) override;

    void CalculateRHS() override;
};

// Class Const Chamber
class ConstChamber : public Chamber
{
public:
    ConstChamber(std::string name, double pressure, double volume);

    void CalculateRHS() override;
};

// Class Variable Chamber
class VariableChamber : public Chamber
{
    public:
    VariableChamber(std::string name, double initPressure, double initVolume);

    void CalculateRHS() override;

    void SetVolumeDer(double volumeDer);
};

