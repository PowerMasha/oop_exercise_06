cmake_minimum_required (VERSION 3.5)

project(lab6)

add_executable(oop_exercise_06
  main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set_target_properties(oop_exercise_06 PROPERTIES CXX_STANDART 14 CXX_STANDART_REQUIRED ON)
