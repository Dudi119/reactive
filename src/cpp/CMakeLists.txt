project(wrappers CXX)
cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_COLOR_MAKEFILE ON)
set (CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Default build type 'Debug'")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE )
else()
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${OUTPUT_DIR})
set(CMAKE_BINARY_DIR ${OUTPUT_DIR})
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIR})

include_directories(. ..  ${PROJECT_3RD_LOC}/include ${PYTHON_INCLUDE_DIRS})
link_directories(${PROJECT_3RD_LOC}/lib)

add_library(_pyNode SHARED pyNodeWrapper.cpp PyNode.h)
target_link_libraries(_pyNode sweetPy${CMAKE_DEBUG_POSTFIX}.so libCore${CMAKE_DEBUG_POSTFIX}.so pthread )
set_target_properties(_pyNode PROPERTIES PREFIX "")
set_target_properties(_pyNode PROPERTIES DEBUG_POSTFIX "")
