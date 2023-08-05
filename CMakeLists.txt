cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Default to "Release" build
SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build,
# options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) DEBUG RELEASE
# RELWITHDEBINFO MINSIZEREL DEV.")

project(
  LUST
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

# Using FunC to generate lookup tables
find_package(FunC REQUIRED)
message(STATUS "Found FunC libs: ${FunC_INCLUDE_DIR}")

#find_package(Boost)
#message(STATUS "Found boost incl: ${Boost_INCLUDE_DIR}")
#find_package(Armadillo)
#find_package(OpenMP)

#configure_file(${PROJECT_SOURCE_DIR}/cmake/config.hpp.cmake ${PROJECT_SOURCE_DIR}/src/config.hpp @ONLY)

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
