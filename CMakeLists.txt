cmake_minimum_required(VERSION 3.10)
message("Starting")
set(CMAKE_C_COMPILER "pgcc")
set(CMAKE_CXX_COMPILER "pgc++")
project(Paralell_FFT)

# Find OpenCV package
find_package(OpenCV REQUIRED)
find_package(OpenACC REQUIRED)
# Add compiler warning flags
message("pgc compiler found")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenACC_CXX_FLAGS} -Wall -Wextra -pedantic -g -acc")

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("/usr/include/c++/11")

# Collect source files
file(GLOB SOURCES *.cpp)

# Exclude files ending with '.ignore.cpp'
list(FILTER SOURCES EXCLUDE REGEX ".*\\.ignore\\.cpp$")

# Create executable
add_executable(Main ${SOURCES})

# Link OpenCV libraries
target_link_libraries(Main ${OpenCV_LIBS})

