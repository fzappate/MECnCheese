#include <string>
#include "../include/chamber.h"

// Constructors
Chamber::Chamber(){};

Chamber::Chamber(std::string, double vol, double press) : name(name), volume(vol), pressure(press){};

// Getters and setters
void Chamber::SetPressure(double press)
{
    pressure = press;

    return;
};

double Chamber::GetPressure()
{
    return pressure;
};