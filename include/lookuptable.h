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

    sunindextype noOfRow; 
    sunindextype noOfCol;

    LookUpTable(std::string name, std::string filename);

    // Read a table
    sunindextype ReadTable();

    // Read a table and print how long it takes
    sunindextype TimedReadTable();

    // Get a column of the data table 
    std::vector<sunrealtype>& GetTableColumn(sunindextype index);

    // Get interpolated value value 
    sunrealtype GetInterpolatedValue(sunindextype columnIndex, sunindextype lowerValIndex, sunrealtype coeff);
};
