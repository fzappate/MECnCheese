#pragma once

#include <string>
#include <vector>
#include "./equation.h"

class Chamber : public Equation 
{
    protected:
    double bulkMod = 1.8*1.0E9;
    double pressure = 1; 
    double volume = 1;
    double volDer = 0;
    double dpdt = 0;
    double flowSum = 0;
    double aTol = 1;
    double rTol = 0.001;
    
    std::vector<double> flowIn;
    std::vector<std::string> flowInNames;

    public:

    Chamber( std::string name, double pressure);

    void SetPressure(double pressure);

    double GetPressure();

    double GetAbsTol();

    double GetRelTol();

    void SetIsDifferential(bool isDifferential) override;

    bool GetIsDifferential() override;

    virtual void AddFlowIn(std::string name, double flow);

    void UpdateDepVar(double pressure) override;

    void ZeroParameters() override;

    void CalculateRHS() override;

    double GetRHS() override;

    double GetInitialCondition() override;

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

    double volume;
    double volDer = 0;;

public:
    
    ConstChamber(std::string name, double pressure, double volume);

    

};

