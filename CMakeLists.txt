cmake_minimum_required(VERSION 3.13)
project(bh_sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Ofast -g")

add_executable(3d-barnes-hut
        main.cpp
        model.cpp
        model.h
        nbody_sim.cpp
        nbody_sim.h
        integrator/integrator.cpp
        integrator/integrator.h
        vec3d.cpp vec3d.h
        force_calc/force_calc.cpp
        force_calc/force_calc.h
        star.cpp star.h
        integrator/integrator_euler.cpp
        integrator/integrator_euler.h
        force_calc/force_calc_all_pairs.cpp
        force_calc/force_calc_all_pairs.h
        force_calc/oct_tree.cpp
        force_calc/oct_tree.h
        force_calc/force_calc_barnes_hut.cpp
        force_calc/force_calc_barnes_hut.h
        integrator/integrator_leapfrog.cpp
        integrator/integrator_leapfrog.h
        force_calc/force_calc_barnes_hut_parallel.cpp
        force_calc/force_calc_barnes_hut_parallel.h)
