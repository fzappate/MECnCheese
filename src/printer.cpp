#pragma once
#include "./object.h"
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
    outputFile << "Simulation:Time:s, ";
    int noOfDiffEquations = sys.GetNoOfDiffEq();
    std::vector<DiffObject*> diffEquations = sys.GetDiffObjects();
    for (int ii = 0; ii < noOfDiffEquations; ii++)
    {
        DiffObject &tempEq = *diffEquations[ii];
        tempEq.PrintHeader(outputFile);
    };


    int noOfAuxEquations = sys.GetNoOfAuxEq();
    std::vector<NonDiffObject*> auxEquations = sys.GetNonDiffObjects();
    for (int ii = 0; ii < noOfAuxEquations; ii++)
    {
        NonDiffObject &tempEq = *auxEquations[ii];
        tempEq.PrintHeader(outputFile);
    };

    outputFile << std::endl;
    
    return; 

}

void Printer::PrintResults(double time)
{
    outputFile << time << ",";
    int noOfDiffEquations = sys.GetNoOfDiffEq();
    std::vector<DiffObject*> diffObjects = sys.GetDiffObjects();
    for (int ii = 0; ii < noOfDiffEquations; ii++)
    {
        DiffObject &tempEq = *diffObjects[ii];
        tempEq.PrintVariables(outputFile);
    };


    int noOfAuxEquations = sys.GetNoOfAuxEq();
    std::vector<NonDiffObject*> auxEquations = sys.GetNonDiffObjects();
    for (int ii = 0; ii < noOfAuxEquations; ii++)
    {
        NonDiffObject &tempEq = *auxEquations[ii];
        tempEq.PrintVariables(outputFile);
    };

    outputFile << std::endl;

    return; 
}