cmake_minimum_required(VERSION 3.13)
project(bh_sim)

set(CMAKE_CXX_STANDARD 17)

add_executable(3d-barnes-hut main.cpp Model.cpp Model.h)
