cmake_minimum_required(VERSION 3.10.0)
project(itsmine VERSION 0.1.0 LANGUAGES CXX)

add_executable(itsmine src/main.cpp)

target_compile_features(itsmine PRIVATE cxx_std_20)

if (MSVC)
  target_compile_options(itsmine PRIVATE /W4 /permissive-)
else()
  target_compile_options(itsmine PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

include(CTest)
enable_testing()

