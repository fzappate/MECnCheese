#pragma once

// Class Chamber
class Chamber
{

    std::string name;
    double volume;
    double pressure;

public:
    // Default constructor
    Chamber();
    // Member intializer constructor
    Chamber(std::string name, double vol, double press);

    // Set the chamber pressure
    void SetPressure(double pressure);
    // Get the chamber pressure
    double GetPressure();
};