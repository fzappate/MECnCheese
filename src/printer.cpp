#pragma once
#include "./equation.h"
#include "./printer.h"

Printer::Printer(){};

Printer::Printer(System sys) : sys(sys)
{ 
    return;
};

void Printer::OpenFile()
{
    outputFile.open(filename);
    return;
}

void Printer::PrintResultsHeader()
{
    int noOfDiffEquations = sys.GetNoOfDiffEq();
    std::vector<Equation*> diffEquations = sys.GetDiffEquations();
    for (int ii = 0; ii < noOfDiffEquations; ii++)
    {
        Equation &tempEq = *diffEquations[ii];
        tempEq.PrintHeader(outputFile);
    };


    int noOfAuxEquations = sys.GetNoOfAuxEq();
    std::vector<Equation*> auxEquations = sys.GetAuxEquations();
    for (int ii = 0; ii < noOfAuxEquations; ii++)
    {
        Equation &tempEq = *auxEquations[ii];
        tempEq.PrintHeader(outputFile);
    };

    outputFile << std::endl;
    
    return; 

}

void Printer::PrintResults()
{
    int noOfDiffEquations = sys.GetNoOfDiffEq();
    std::vector<Equation*> diffEquations = sys.GetDiffEquations();
    for (int ii = 0; ii < noOfDiffEquations; ii++)
    {
        Equation &tempEq = *diffEquations[ii];
        tempEq.PrintVariables(outputFile);
    };


    int noOfAuxEquations = sys.GetNoOfAuxEq();
    std::vector<Equation*> auxEquations = sys.GetAuxEquations();
    for (int ii = 0; ii < noOfAuxEquations; ii++)
    {
        Equation &tempEq = *auxEquations[ii];
        tempEq.PrintVariables(outputFile);
    };

    outputFile << std::endl;

    return; 
}