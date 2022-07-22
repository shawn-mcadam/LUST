cmake_minimum_required(VERSION 3.19)

# Default to "Release" build
SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build,
# options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) DEBUG RELEASE
# RELWITHDEBINFO MINSIZEREL DEV.")

project(
  LUST
  LANGUAGES CXX)

# Honestly I would like to play with c++20's modules so that standard might be nice
set(CMAKE_CXX_STANDARD 20) # c++20 is a cmake version 3.12 feature

# Using FunC to generate lookup tables
find_package(FunC)
#message(STATUS "Found FunC incl: $}") TODO make a FunC_INCLUDE_DIR variable

# Generating LUTs from Boosts suite of special functions
find_package(Boost REQUIRED)
message(STATUS "Found boost incl: ${Boost_INCLUDE_DIR}")

find_package(OpenMP)

# Primary source subdirectory
add_subdirectory("src")

# Examples subdirectory
add_subdirectory("examples")


# Summarize compilation flags for quick review on configuration
macro(print_variable var)
  message(STATUS "${var}: ${${var}}")
endmacro()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "--- Compile and link flags ------------------")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
string(TOLOWER "${CMAKE_BUILD_TYPE}" build_lower)
if(build_lower MATCHES release)
  print_variable(CMAKE_CXX_FLAGS_RELEASE)
elseif(build_lower MATCHES debug)
  print_variable(CMAKE_CXX_FLAGS_DEBUG)
elseif(build_lower MATCHES relwithdebinfo)
  print_variable(CMAKE_CXX_FLAGS_RELWITHDEBINFO)
elseif(build_lower MATCHES minsizerel)
  print_variable(CMAKE_CXX_FLAGS_MINSIZEREL)
endif()

message(STATUS "---------------------------------------------")
