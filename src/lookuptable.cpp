#pragma once
#include "./lookuptable.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>

LookUpTable::LookUpTable(std::string name, std::string filename) : name(name), filename(filename)
{
    return;
};

sunindextype LookUpTable::ReadTable()
{

    std::ifstream inputFile(this->filename);

    if (!inputFile.is_open())
    {
        std::cout << "Error opening file: " << filename << std::endl;
        return 1;
    }

    sunbooleantype loadedHeader = SUNFALSE;
    sunindextype lineCounter = 0;
    sunindextype columnCounter = 0;

    std::string line;
    while (std::getline(inputFile, line))
    {
        std::stringstream ss(line);
        std::string cell;

        if (loadedHeader == SUNFALSE) // Load the header
        {
            while (std::getline(ss, cell, ','))
            {
                this->headers.push_back(cell);
            }
            // Resize the data vector to the number of columns
            this->data.resize(this->headers.size());
            // Set the flag to true to prevent headers reload
            loadedHeader = SUNTRUE;
        }
        else
        {
            sunindextype columnCounter = 0;
            while (std::getline(ss, cell, ','))
            {
                sunrealtype cellValue = std::stod(cell);
                this->data[columnCounter].push_back(cellValue);

                columnCounter++;
            }
            lineCounter++;
        }
    }

    // Save the number of rows and columns
    this->noOfCol = columnCounter;
    this->noOfRow = lineCounter;

    inputFile.close();



    return 0;
};

sunindextype LookUpTable::TimedReadTable()
{
    auto startTimer = std::chrono::high_resolution_clock::now();

    sunindextype readTableSuccess = this->ReadTable();

    auto nowTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - startTimer);
    std::cout << "Table " << this->name << " read in: " << elapsed.count() << " ms" << std::endl;

    return readTableSuccess;
};

std::vector<sunrealtype>& LookUpTable::GetTableColumn(sunindextype index)
{
    return this->data[index];
};

sunrealtype LookUpTable::GetInterpolatedValue(  sunindextype columnIndex, 
                                                sunindextype lowerValIndex,     
                                                sunrealtype coeff)
{
    return this->data[columnIndex][lowerValIndex] + coeff*(this->data[columnIndex][lowerValIndex+1] - this->data[columnIndex][lowerValIndex]);
};