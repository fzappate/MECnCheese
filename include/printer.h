#pragma once
#include <fstream>
#include <iostream>
#include <string>

#include "./system.h"

using namespace std;

class Printer
{
protected:
    std::string filename = "ResultsPrinter.csv";
    std::ofstream outputFile;
    System sys;

public:
    Printer();

    Printer(System sys);

    void OpenFile();

    void PrintResultsHeader();

    void PrintResults(double time);
};