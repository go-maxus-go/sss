cmake_minimum_required (VERSION 3.10)

project(tests)

include_directories("include")
include_directories("3rdparty/json/include")
include_directories("3rdparty/preprocessor/include")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES include/*.h include/*.hpp include/*.cpp)

add_executable(tests main.cpp ${SRC_FILES})
