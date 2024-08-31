#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "./inputreader.h"

InputReader::InputReader(){};

void InputReader::ReadInput()
{
    std::ifstream inputFile("Input.txt"); 
  
    // Check if the file is successfully opened 
    if (!inputFile.is_open()) { 
        std::cout << "Error opening the file!" << std::endl; 
        return; 
    } 
  
    // Read each line of the file and print it to the standard output stream 
    std::string line; 
    while (getline(inputFile, line)) { 

      // Remove leading and trailing spaces
      RightTrim(line);
      LeftTrim(line);

      // Make sure the line is not a comment
      if (line[0] == '#')
      {
          continue;
      }

      // Break down the line in tokens 
      std::stringstream ss(line);
      std::string token; 
      std::vector<std::string> tokens; 
      char delimiter = ' ';

      while (getline(ss, token, delimiter)) { 
        tokens.push_back(token); 
      } 

      // Make sure that the line respects the proper format
      if(tokens.size()==3)
      {
      std::string name = tokens[0];
      std::string unit = tokens[1];
      double convFactor = ConvertUnits(unit);
      double value = std::stod(tokens[2])*convFactor;

      variableNames.push_back(name);
      variables.push_back(value);
      }
      else
      {
        return;
      }

      std::cout << line << std::endl; // Print the current line 
    } 
  
    // Close the file 
    inputFile.close(); 
    return;
};

void InputReader::RightTrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return;
}

void InputReader::LeftTrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    return;
}

// Convert input units into SI units
double InputReader::ConvertUnits(std::string unit)
{
    double convFactor = 0;
    if (unit == "m")
    {
        convFactor = 1;
    }
    else if (unit == "dm")
    {
        convFactor = 1e-1;
    }
    else if (unit == "cm")
    {
        convFactor = 1e-2;
    }
    else if (unit == "mm")
    {
        convFactor = 1e-3;
    }
    else if (unit == "m^2")
    {
        convFactor = 1;
    }
    else if (unit == "dm^2")
    {
        convFactor = 1e-2;
    }
    else if (unit == "cm^2")
    {
        convFactor = 1e-4;
    }
    else if (unit == "mm^2")
    {
        convFactor = 1e-6;
    }
    else if (unit == "m^3")
    {
        convFactor = 1;
    }
    else if (unit == "dm^3")
    {
        convFactor = 1e-3;
    }
    else if (unit == "cm^3")
    {
        convFactor = 1e-6;
    }
    else if (unit == "mm^3")
    {
        convFactor = 1e-9;
    }
    else if (unit == "Pa")
    {
        convFactor = 1;
    }
    else if (unit == "kPa")
    {
        convFactor = 1e3;
    }
    else if (unit == "MPa")
    {
        convFactor = 1e6;
    }
    else if (unit == "bar")
    {
        convFactor = 1e5;
    }
    else 
    {
        std::cout << "Unit not found." << std::endl;
    }
    return convFactor;
}

double InputReader::SearchVarInVarList(std::string var)
{
    for(int ii = 0; ii<variables.size(); ii++)
    {
        if(variableNames[ii] == var)
        {
            return variables[ii];
        }
    }

    std::cout<< "Variable " << var <<" not found." << std::endl;
    return 0;
}

double InputReader::ImportVarAsDouble(std::string varName)
{
    double res = SearchVarInVarList(varName);
    return res;
}

int InputReader::ImportVarAsInt(std::string varName)
{
    double res = SearchVarInVarList(varName); 
    int castRes = static_cast<int>(res);
    return castRes;

}

bool InputReader::ImportVarAsBool(std::string varName)
{
    double res = SearchVarInVarList(varName); 
    bool castRes = static_cast<bool>(res);
    return castRes;
}


