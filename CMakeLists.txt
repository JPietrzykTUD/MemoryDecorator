cmake_minimum_required(VERSION 2.8)
project(MemoryDecorator)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions("-DDEBUG=True")
    message("Debug build.")
    set(OPTIMIZER_FLAGS "-g -O0")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    message("Release build.")
    add_definitions("-DRELEASE=True")
    #    set(OPTIMIZER_FLAGS "-O3")
ELSE()
    message("Some other build type.")
ENDIF()

set(CMAKE_CXX_STANDARD 17)
include_directories( ${PROJECT_SOURCE_DIR}/include)

add_executable(MemoryDecorator main.cpp)