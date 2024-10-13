#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <corecrt_math_defines.h>

#include "./equation.h"
#include "./orifice.h"
#include "./chamber.h"

// Constructors
Orifice::Orifice(std::string name, double area, Chamber &upCh, Chamber &downCh) : Equation(name), area(area), upChamber(upCh), downChamber(downCh)
{
    
    isDifferential = false;

    return;
};

void Orifice::SetIsDifferential(bool isDifferential)
{
    this->isDifferential = false;
    return;
}

bool Orifice::GetIsDifferential()
{
    return this->isDifferential;
}

void Orifice::SetArea(double area)
{
    this->area = area;
    return;
};

double Orifice::GetArea()
{
    return this->area;
};

void Orifice::SetCf(double Cf)
{
    this->Cf = Cf;
    return;
};

double Orifice::GetCf()
{
    return this->Cf;
};

void Orifice::SetFlowrate(double flowrate)
{
    this->flowrate = flowrate;
    return;
};

double Orifice::GetFlowrate()
{
    return this->flowrate;
};

void Orifice::SetUpChamber(Chamber &upChamber)
{
    this->upChamber = upChamber;
    return;
};

Chamber Orifice::GetUpChamber()
{
    return this->upChamber;
};

void Orifice::SetDownChamber(Chamber &downChamber)
{
    this->downChamber = downChamber;
    return;
};

Chamber Orifice::GetDownChamber()
{
    return this->downChamber;
};

void Orifice::CalculateFlowrate()
{
    Cf = GetCf();
    area = GetArea();
    Chamber &upCh = this->upChamber;
    Chamber &downCh = this->downChamber;
    double upPress = upCh.GetPressure();
    double downPress = downCh.GetPressure();
    
    double isDpNeg = signbit(-upPress + downPress);
    int sign;
    if (isDpNeg == 0){
        sign = -1;
    }
    else{
        sign = 1;
    }

    flowrate = sign * Cf * area * sqrt(abs(upPress - downPress));

    UpdateChambersFlow();
    return;
};

void Orifice::UpdateChambersFlow(){
    // Flow entering a chamber is negative
    upChamber.AddFlowIn(name,-flowrate);
    downChamber.AddFlowIn(name,flowrate);
    return;
};

void Orifice::UpdateDepVar(double depVar)
{
    return;
};

void Orifice::ZeroParameters()
{
    return;
};

void Orifice::CalculateRHS()
{
    // Calculate and update the flowrate
    this->CalculateFlowrate();

    return;
};

double Orifice::GetRHS()
{
    std::cout << "Orifice GetRHS" << std::endl;
    return this->flowrate;
};

double Orifice::GetAbsTol()
{
    return 0.0;
}

double Orifice::GetRelTol()
{
    return 0.0;
}

double Orifice::GetInitialCondition()
{
    return this->flowrate;

};

void Orifice::PrintHeader(std::ofstream& outputFile)
{
    if(printStruct.printCf == 1)
    {
    outputFile << name << ":Cf:-, ";
    }
    if(printStruct.printArea == 1)
    {
        outputFile << name << ":Area:m^2, ";
    }
    if(printStruct.printFlowrate == 1)
    {
        outputFile << name << ":Flowrate:m^3/s, ";
    }


    return;
}

void Orifice::PrintVariables(std::ofstream& outputFile)
{
    if(printStruct.printCf == 1)
    {
    outputFile << Cf << ",";
    }
    if(printStruct.printArea == 1)
    {
        outputFile << area << ",";
    }
    if(printStruct.printFlowrate == 1)
    {
        outputFile << flowrate << ",";
    }

    return;
}   