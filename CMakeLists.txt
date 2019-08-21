cmake_minimum_required (VERSION 3.10)

set(TARGET_NAME tests)
project(${TARGET_NAME})

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set pedantic errors
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# Include paths
include_directories("include")
include_directories("3rdparty/json/include")
include_directories("3rdparty/catch2/single_include")
include_directories("3rdparty/preprocessor/include")

# Include files recursive
file(GLOB_RECURSE SRC_FILES
    include/*.h
    include/*.hpp
    include/*.cpp
    tests/*.h
    tests/*.hpp
    tests/*.cpp
)

# Add files to executable
add_executable(${TARGET_NAME} ${SRC_FILES})

# Set filter hierarchy according to file folders
if (MSVC)
    foreach(source IN LISTS SRC_FILES)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" source ${source})
        get_filename_component(source_path "${source}" PATH)
        string(REPLACE "/" "\\" source_path_msvc "${source_path}")
        source_group("${source_path_msvc}" FILES "${source}")
    endforeach()
endif()
