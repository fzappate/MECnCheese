# -----------------------------------------------------------------
# Programmer(s): Radu Serban @ LLNL
#                David J. Gardner @ LLNL
# -----------------------------------------------------------------
# SUNDIALS Copyright Start
# Copyright (c) 2002-2023, Lawrence Livermore National Security
# and Southern Methodist University.
# All rights reserved.
#
# See the top-level LICENSE and NOTICE files for details.
#
# SPDX-License-Identifier: BSD-3-Clause
# SUNDIALS Copyright End
# -----------------------------------------------------------------
# CMakeLists.txt for CVODE C serial examples.
#
# This file is generated from a template using variables
# set at configuration time. It can be used as a template for
# other user CMakeLists configuration files.
# -----------------------------------------------------------------

# Set the minimum required cmake version
cmake_minimum_required(VERSION 3.29.0-rc1)

# Set cache variables for compilers and flags
set(CMAKE_C_COMPILER
  C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.37.32822/bin/Hostx64/x64/cl.exe
  CACHE FILEPATH "C compiler")

set(CMAKE_C_FLAGS
  "/DWIN32 /D_WINDOWS /W3"
  CACHE STRING "C compiler flags")

if("99")
  set(CMAKE_C_STANDARD "99"
    CACHE STRING "C standard")
endif()

# Specify project name and languages
project(MECAndCheese CXX)

# set_target_properties(MECAndCheese PROPERTIES LINKER_LANGUAGE CXX)

# Enable testing
include(CTest)

# ------------------------------------------------------------------------------

# Specify the path to SUNDIALS header files
set(SUNDIALS_INCLUDE_DIR
  C:/Software/SUNDIALS/SUNDIALS/include
  CACHE PATH "Location of SUNDIALS header files")

# Specify the path to SUNDIALS libraries
set(SUNDIALS_LIBRARY_DIR
  C:/Software/SUNDIALS/SUNDIALS/lib
  CACHE PATH "Location of SUNDIALS libraries")

# Find the SUNDIALS libraries
find_library(SUNDIALS_SOLVER_LIB
  sundials_cvode ${SUNDIALS_LIBRARY_DIR}
  DOC "CVODE library")

find_library(SUNDIALS_NVEC_LIB
  sundials_nvecserial ${SUNDIALS_LIBRARY_DIR}
  DOC "NVECTOR_SERIAL library")

find_library(SUNDIALS_MANYVEC_LIB
  sundials_nvecmanyvector ${SUNDIALS_LIBRARY_DIR}
  DOC "NVECTOR_MANYVECTOR library")

set(SUNDIALS_EXTRA_LIBS  
  CACHE STRING 
  "Additional libraries")

# List of SUNDIALS libraries
set(SUNDIALS_LIBRARIES
  -L${SUNDIALS_LIBRARY_DIR}
  ${SUNDIALS_SOLVER_LIB}
  ${SUNDIALS_NVEC_LIB}
  ${SUNDIALS_MANYVEC_LIB}
  ${SUNDIALS_EXTRA_LIBS})



# Specify the path to MEC libraries NOT USED YET
# set(MEC_SOURCE_DIR
# C:/Software/MACnCheese/src/*.cpp
#  CACHE PATH "Location of source files")
 set(MEC_SOURCE_DIR
 ./src/chamber.cpp
 CACHE PATH "Location of source files")

include(src/CMakeList.txt)

#  set(MEC_SOURCE_FILES
#  ./src/chamber.cpp
#  ./src/orifice.cpp
#  ./src/body.cpp
#  CACHE PATH "Location of source files")

 set(MEC_INCLUDES
  -L${MEC_INCLUDE_DIR}
  ${MEC_SOURCE_DIR})

# Set the relative path to the project include directory
include_directories(./include/)



# ------------------------------------------------------------------------------

# Set the names of the executable to be built and their dependencies
set(exeName MEC)

# example source files
# add_executable(${exeName} main.cpp ./src/chamber.cpp ./src/orifice.cpp)
add_executable(${exeName} main.cpp ${MEC_SOURCES})
# add_executable(${exeName} main.cpp ${MEC_SOURCE_FILES})
# add_executable(${exeName} main.cpp ${MEC_SOURCE_DIR})

# directories to include
target_sources(${exeName} PRIVATE ${MEC_SOURCE_DIR})

# directories to include
target_include_directories(${exeName} PRIVATE ${SUNDIALS_INCLUDE_DIR})

# libraries to link against
target_link_libraries(${exeName} ${SUNDIALS_LIBRARIES})

# NOT IMPLEMENTED: add the example to ctest 
# add_test(NAME ${exeName} COMMAND ${exeName})