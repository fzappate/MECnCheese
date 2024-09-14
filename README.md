### Dedicate
This work is dedicated to all the "midnight scientists"—those with big dreams but little time to devote to the fine art of computational modeling.

### Introduction
SUNDIALS is an incredibly powerful suite of solvers for nonlinear and differential/algebraic equations. Despite its impressive capabilities and support for both C and Fortran, the code can be complex and difficult for non-expert programmers to modify. The provided examples guide users through the setup of simple systems of equations, often written entirely in one function. However, setting up and modifying large systems of equations can be a painful and error-prone process. While this may not be an issue for elite scientist-programmers, it presents a substantial obstacle for others, significantly reducing the usability of such libraries.

The project contained in this repository offers inspiration to those facing these challenges, proposing a solution based on object-oriented programming. By adopting an object-oriented approach, CVODES, a library within SUNDIALS, can be elegantly wrapped in an API tailored to the needs of a specific project. The code introduces functions that allow users to interact with the program through reading and writing .txt files, extending its capabilities even to those with no coding experience. In this project, each equation is represented as an object, transforming a system of equations into a system of interacting objects. This approach makes the problem setup and modification process more intuitive, drastically reduces code repetition, and increases flexibility and modularity.