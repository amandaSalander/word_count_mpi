cmake_minimum_required(VERSION 3.12)
project(hello_mpi)

set(CMAKE_CXX_STANDARD 17)

add_executable(hello_mpi main.cpp Map.cpp Map.h Reduce.cpp Reduce.h Combiner.cpp Combiner.h)