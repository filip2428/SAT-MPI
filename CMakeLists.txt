cmake_minimum_required(VERSION 3.10)
project(MPI)

set(CMAKE_CXX_STANDARD 20)

add_executable(resolution resolution.cpp)
add_executable(dp dp.cpp)
add_executable(dpll dpll.cpp)
add_executable(cdcl cdcl.cpp)
add_executable(grasp grasp.cpp)


