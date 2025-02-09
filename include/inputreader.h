#pragma once
#include <vector>
#include <string>

class InputReader
{
private:
    std::string filename = "Inputs.md";
    std::vector<double> variables;
    std::vector<std::string> variableNames;

public:
    InputReader();

    InputReader(std::string filename);

    void ReadInput();

    void RightTrim(std::string &s);

    void LeftTrim(std::string &s);

    void RemoveCharacter(std::string &str, char c);

    double ConvertUnits(std::string unit);

    double SearchVarInVarList(std::string var);

    double ImportVarAsDouble(std::string var);

    int ImportVarAsInt(std::string var);

    bool ImportVarAsBool(std::string var);
};