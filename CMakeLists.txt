cmake_minimum_required(VERSION 4.1.1)
project(itsmine VERSION 0.1.0 LANGUAGES CXX)

add_executable(itsmine src/main.cpp)

# Tests
add_executable(test_params tests/test_params.cpp)
target_compile_features(test_params PRIVATE cxx_std_20)

add_test(NAME params_test COMMAND test_params)

target_compile_features(itsmine PRIVATE cxx_std_20)

if (MSVC)
  target_compile_options(itsmine PRIVATE /W4 /permissive- /fsanitize=address)
  target_link_options(itsmine PRIVATE /fsanitize=address)
  target_compile_options(test_params PRIVATE /fsanitize=address)
  target_link_options(test_params PRIVATE /fsanitize=address)
else()
  target_compile_options(itsmine PRIVATE -Wall -Wextra -Wpedantic -Werror -fsanitize=address)
  target_link_options(itsmine PRIVATE -fsanitize=address)
  target_compile_options(test_params PRIVATE -fsanitize=address)
  target_link_options(test_params PRIVATE -fsanitize=address)
endif()

# Add library for Params implementation and link it to executables
add_library(params_impl STATIC src/params.cpp)
target_include_directories(params_impl PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(itsmine PRIVATE params_impl)
target_link_libraries(test_params PRIVATE params_impl)

include(CTest)
enable_testing()
