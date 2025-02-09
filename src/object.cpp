#include <string>
#include <iostream>
#include "./object.h"

Object::Object(std::string name) : name(name) {};

DiffObject::DiffObject(std::string name) : Object(name) {};

sunrealtype DiffObject::GetAbsTol(sunindextype index)
{
    return this->absTol[index];
};

void DiffObject::SetDepVarIndex(sunindextype objDepVarIndex, sunindextype sysDepVarIndex)
{
    this->depVarIndexInSys[objDepVarIndex] = sysDepVarIndex;
    return;
};

sunrealtype DiffObject::GetYValuesInit(sunindextype index)
{
    return this->yValuesInit[index];    
};

void DiffObject::SetYValuesPnt(sunindextype index, sunrealtype* yValuePnt)
{
    this->yValuesPnt[index] = yValuePnt;
    return;
};

void DiffObject::SetYDotValuesPnt(sunindextype index, sunrealtype* yDotValuePnt)
{
    this->yDotValuesPnt[index] = yDotValuePnt;
    return;
};

sunindextype DiffObject::GetDepVarIndex(sunindextype index)
{
    return this->depVarIndexInSys[index];
};

sunindextype DiffObject::GetNoOfDepVar()
{
    return this->nDepVar;
};

NonDiffObject::NonDiffObject(std::string name) : Object(name) {};