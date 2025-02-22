#pragma once
#include <vector>
#include <string>
#include "./sundials.h"

class LookUpTable
{
private:
    std::string name;
    std::string filename;
    std::vector<std::string> headers;
    std::vector<std::vector<sunrealtype>> data;

public:
    LookUpTable(std::string name, std::string filename);

    // Read a table
    sunindextype ReadTable();

    // Read a table and print how long it takes
    sunindextype TimedReadTable();
};
