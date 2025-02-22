#pragma once
#include "./lookuptable.h"
#include <fstream>
#include <iostream>
#include <sstream>

OrderedLookUpTable::OrderedLookUpTable(std::string name, std::string filename) : name(name), filename(filename)
{
    ReadTable();
    return;
};

void OrderedLookUpTable::ReadTable()
{
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        
        for (const std::string& str : row) {
            std::cout << str << " ";
        }
        std::cout << std::endl;
    }

    inputFile.close();
    return;

}