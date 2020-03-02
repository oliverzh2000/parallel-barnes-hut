cmake_minimum_required(VERSION 3.10)
project(bh_sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Ofast -g -march=native -pthread")
include_directories(.)

add_executable(nbody
        base/main.cpp
        base/model.cpp
        base/nbody_sim.cpp
        base/star.cpp
        base/vec3d.cpp
#        force_calc/abstract_octree.cpp
        force_calc/breadth_first_octree.cpp
#        force_calc/flat_octree.cpp
        force_calc/force_calc.cpp
        force_calc/force_calc_all_pairs.cpp
        force_calc/force_calc_barnes_hut.cpp
        force_calc/force_calc_barnes_hut_parallel.cpp
        force_calc/octree.cpp
        integrator/integrator.cpp
        integrator/integrator_euler.cpp
        integrator/integrator_leapfrog.cpp
        utils/stopwatch.cpp
        )
