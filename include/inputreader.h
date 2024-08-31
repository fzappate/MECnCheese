#pragma once
#include <vector>
#include <string>

class InputReader
{
    private:

    std::vector<double> variables;
    std::vector<std::string> variableNames;
    
    public:

    InputReader();

    void ReadInput();

    void RightTrim(std::string &s);

    void LeftTrim(std::string &s);

    double ConvertUnits(std::string unit);

    double SearchVarInVarList(std::string var);

    double ImportVarAsDouble(std::string var);

    int ImportVarAsInt(std::string var);

    bool ImportVarAsBool(std::string var);

};