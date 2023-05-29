cmake_minimum_required(VERSION 3.22)
project(pig_vector)

set(CMAKE_CXX_STANDARD 17)

add_executable(pig_vector zfy.hpp exceptions.hpp utility.hpp two.cpp)
