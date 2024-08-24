#pragma once

#include <vector>

#include <cvode/cvode.h>               // prototypes for CVODE fcts., consts.
#include <nvector/nvector_serial.h>    // access to serial N_Vector

#include "equation.h"

// ===== WHAT IS BEHIND #define Ith(v, i) =====
// ----- double * ttest = ( ( (N_VectorContent_Serial)(y->content) )->data );
// double *ttest: This declares ttest as a pointer to a double. 
// It means ttest will hold the memory address of a double type variable.
// ----- Understanding the Expression: ----- 
// y->content: This accesses the content member of the struct or object pointed to by y.
// (N_VectorContent_Serial)(y->content): This casts y->content to type N_VectorContent_Serial. 
// It assumes that y->content points to or is of a type that can be interpreted as N_VectorContent_Serial.
// ----- Accessing Struct Members: ----- 
// ((N_VectorContent_Serial)(y->content))->data: Once y->content is cast to N_VectorContent_Serial, 
// the data member of this structure is accessed. data presumably holds a pointer to an array or a block of 
// memory.
// ----- Final Assignment: ----- 
// double *ttest = ...: This assigns the value of ((N_VectorContent_Serial)(y->content))->data to ttest. 
// Since data is a pointer to double, ttest now points to the same memory location as data.
// ----- Explanation: ----- 
// y seems to be a pointer to some structure or object that has a member content.
// y->content is accessed first, and it's assumed to be of type N_VectorContent_Serial.
// N_VectorContent_Serial is then used to access its member data, which is a pointer to double.
// The pointer ttest is then initialized to point to the same location as data, effectively allowing ttest
// to access the same array or memory block that data points to.
// ----- In summary: ----- 
// The code snippet assigns the pointer ttest to point to the data stored in the data member of the 
// structure pointed to by y->content, after casting y->content to type N_VectorContent_Serial. This 
// assumes y->content points to a structure with a data member that holds a pointer to double.


class System{

    int noOfAuxEq = 0;
    std::vector<Equation*> auxEquations;
    std::vector<Equation*> diffEquations;
    std::vector<sunrealtype> initConditions;
    N_Vector N_VectInitConditions;
    SUNContext &sunctx;
    
    void AddDiffEqCount();
    void AddLinEqCount();

public:

    int noOfDiffEq = 0;

    double relTol = 0.001;

    System(SUNContext &sunctx);

    void AddEquation(Equation& equation);

    void AddSUNContext(SUNContext &sunctx);

    SUNContext GetSUNContext();

    N_Vector GetInitCondition();

    N_Vector GetEqAbsTol();

    double GetRelTol();

    void CalculateDiffEqRHS();

    void CalculateAuxEqRHS();

    std::vector<double> GetDiffEqRHS();

    void ResetDiffEq(N_Vector y);

};