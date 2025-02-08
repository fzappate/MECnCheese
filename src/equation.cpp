#include <string>
#include <iostream>
#include "./equation.h"

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

NonDiffObject::NonDiffObject(std::string name) : Object(name) {};