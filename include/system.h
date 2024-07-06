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


// ===== WHAT IS I WANT TO DO IT IN CPP =====
// In C++, you can use modern practices to achieve a similar effect while improving readability and safety. 
// Here’s how you could refactor the code using C++ features:
// Assuming y is a pointer to an object of some class type that has a member content, and content is of 
// type N_VectorContent_Serial (possibly a struct), you can use static_cast for type casting and smart 
// pointers for memory management. Here's an example:
// ----- Example: -----
// // Assuming y is a pointer to an object of some class type
// // and content is a member of type N_VectorContent_Serial
// " auto content = y->content; " // Assuming y->content is already of type N_VectorContent_Serial
// // Use static_cast to cast content to N_VectorContent_Serial if needed
// // Assuming data is a pointer to double within N_VectorContent_Serial
// " double *ttest = static_cast<double *>(content.data); "
// // Finally, dereference the pointer to access its content
// " double value = *ttest; // Dereferencing ttest to get the double value "
// ----- Explanation: -----
// Auto Keyword:
// auto content = y->content;: Using auto allows the compiler to deduce the type of content based on y->content. 
// This assumes that y->content is already of type N_VectorContent_Serial.
// Static Cast:
// double *ttest = static_cast<double *>(content.data);
// : static_cast is used for type casting in C++. It's safer than C-style casting 
// ((N_VectorContent_Serial)(y->content)) because it performs type checking at compile-time and ensures type 
// safety.
// ----- Pointer Initialization: -----
// double *ttest: Declares ttest as a pointer to double, which will point to the same location as content.data.
// ----- Advantages of C++ Approach: ----- 
// Type Safety: Using static_cast ensures type safety and makes the code more readable by explicitly stating the intended type conversion.
// Modern Practices: Utilizing auto for type deduction and static_cast for type casting aligns with modern C++ best practices.
// Readability: The use of auto and static_cast makes the code clearer and easier to understand compared to C-style casts.
// Note:
// Ensure that y->content is indeed of type N_VectorContent_Serial or can be safely cast to it using static_cast. This assumes you have control over the types and can verify the safety of the cast at compile-time.
// By leveraging these C++ features, you can write clearer, safer, and more maintainable code compared to traditional C-style casting and pointer operations.

class System{

    int noOfDiffEq = 0;
    int noOfAuxEq = 0;
    std::vector<Equation> auxEquations;
    std::vector<Equation> diffEquations;
    std::vector<sunrealtype> initConditions;
    N_Vector N_VectInitConditions;
    SUNContext &sunctx;

    
    void AddDiffEqCount();
    void AddLinEqCount();


public:

    System(SUNContext &sunctx);

    void AddEquation(Equation equation);

    void AddSunContext(SUNContext &sunctx);

    N_Vector GetInitCondition();

    N_Vector CalculateSystemRHS(sunrealtype t, N_Vector y, N_Vector ydot, void *user_data);

};