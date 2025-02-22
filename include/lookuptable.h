#pragma once
#include <vector>
#include <string>

class OrderedLookUpTable
{
private:
std::string name;
std::string filename;


public:
OrderedLookUpTable(std::string name, std::string filename);

void ReadTable();
};
