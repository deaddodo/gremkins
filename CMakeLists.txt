cmake_minimum_required(VERSION 3.10)
project(gremlin C)

set(CMAKE_C_STANDARD 11)

add_executable(gremlin main.c engine/gremlins.c shared/iter.c engine/map.c engine/map.h)