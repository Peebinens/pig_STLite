cmake_minimum_required(VERSION 3.22)
project(pig_list)

set(CMAKE_CXX_STANDARD 17)

add_executable(pig_list exceptions.hpp utility.hpp algorithm.hpp list.hpp one.cpp)
