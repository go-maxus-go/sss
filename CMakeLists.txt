cmake_minimum_required (VERSION 3.10)

project(test)

include_directories("include")
include_directories("3rdparty/json/include")
include_directories("3rdparty/preprocessor/include")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test main.cpp)