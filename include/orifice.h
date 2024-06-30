#pragma once

#include <string>
#include "./chamber.h"

// Class Orifice
class Orifice
{

    std::string name;
    double area;
    double perim;
    double diam;
    double hydrDiam;
    double Cf;
    Chamber upCh;
    Chamber downCh;

public:
    // Default constructor
    Orifice();
    // Member initializer constructor 
    Orifice(double area);
    // Member initializer constructor
    Orifice(std::string name, double area);

    double GetCf();
};
