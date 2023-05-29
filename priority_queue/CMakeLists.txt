cmake_minimum_required(VERSION 3.16)
project(priority_queue)

set(CMAKE_CXX_STANDARD 17)

add_executable(priority_queue
        exceptions.hpp priority_queue.hpp utility.hpp one.cpp)
