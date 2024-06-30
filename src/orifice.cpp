#include <math.h>
#include <string>
#include <corecrt_math_defines.h>

#include "../include/orifice.h"

// Constructors
Orifice::Orifice(){};

Orifice::Orifice(double area):area(area){};

Orifice::Orifice(std::string name, double area) : name(name), area(area)
{
    // Assumer circular orifice
    double d = sqrt(area * 4 / M_PI);
    hydrDiam = 4 * area / perim;
    Cf = 0.678;
    return;
};

double Orifice::GetCf()
{
    return Cf;
};
