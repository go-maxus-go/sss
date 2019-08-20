cmake_minimum_required (VERSION 3.10)

set(TARGET_NAME tests)

project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

include_directories("include")
include_directories("3rdparty/json/include")
include_directories("3rdparty/catch2/single_include")
include_directories("3rdparty/preprocessor/include")

file(GLOB_RECURSE SRC_FILES include/*.h include/*.hpp include/*.cpp)
file(GLOB_RECURSE TESTS_FILES tests/*.h tests/*.hpp tests/*.cpp)

add_executable(${TARGET_NAME} ${SRC_FILES} ${TESTS_FILES})
