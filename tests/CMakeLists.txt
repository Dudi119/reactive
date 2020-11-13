project(reactiveTests CXX)
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

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_DEBUG_POSTFIX d)
    set(CORE_DEBUG_POSTFIX d)
endif()

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BUILD_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_BUILD_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_BUILD_DIR}/bin)
set(CMAKE_BINARY_DIR ${PROJECT_BUILD_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BUILD_DIR}/bin)

include_directories(. ..  ${PROJECT_DIR} ${PROJECT_DIR}/../src/cpp ${PROJECT_3RD_LOC}}/include)
link_directories(${PROJECT_3RD_LOC}/lib ${PROJECT_DIR}/../bin)

add_executable(reactiveTests Tests.cpp)
if(TEST_DEPENDECIES)
    add_dependencies(reactiveTests ${TEST_DEPENDECIES})
endif()
set(TO_LINK_LIBS gtest gtest_main reactive${CMAKE_DEBUG_POSTFIX} ${PYTHON_LIBRARIES} util dl sweetPy${CMAKE_DEBUG_POSTFIX}.so Core${CORE_DEBUG_POSTFIX}.so pthread)
target_link_libraries(reactiveTests ${TO_LINK_LIBS})
