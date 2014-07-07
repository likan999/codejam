cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(2008/Round1B/CropTriangles)

add_solver_executable(2008-Round1B-CropTriangles Solution.cpp)

add_solver_test(2008-Round1B-CropTriangles Example example.in example.out int)
add_solver_test(2008-Round1B-CropTriangles Small A-small-practice.in A-small-practice.out int)
add_solver_test(2008-Round1B-CropTriangles Large A-large-practice.in A-large-practice.out int)
