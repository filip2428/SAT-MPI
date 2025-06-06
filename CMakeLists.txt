cmake_minimum_required(VERSION 3.10)
project(MPI)

set(CMAKE_CXX_STANDARD 20)

add_executable(resolution resolution.cpp)
add_executable(dp dp.cpp)
add_executable(dpll dpll.cpp)
add_executable(cdcl cdcl.cpp)
add_executable(grasp grasp.cpp)

if (WIN32)
    set(PLATFORM_LIBS psapi)
else()
    set(PLATFORM_LIBS "")
endif()

target_link_libraries(resolution PRIVATE ${PLATFORM_LIBS})
target_link_libraries(dp PRIVATE ${PLATFORM_LIBS})
target_link_libraries(dpll PRIVATE ${PLATFORM_LIBS})
target_link_libraries(cdcl PRIVATE ${PLATFORM_LIBS})
target_link_libraries(grasp PRIVATE ${PLATFORM_LIBS})
