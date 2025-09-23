cmake_minimum_required(VERSION 3.10.0)
project(itsmine VERSION 0.1.0 LANGUAGES CXX)

add_executable(itsmine src/main.cpp)

include(CTest)
enable_testing()

