cmake_minimum_required(VERSION 3.10)
project(airline)

set(CMAKE_CXX_STANDARD 11)
include_directories(include)

add_executable(airline main.cpp include/flight.h include/plane.h  include/datetime.h include/service.h src/flight.cpp src/plane.cpp src/service.cpp src/datetime.cpp)
