if(_SETUP_INCLUDED)
    return()
endif()
set(_SETUP_INCLUDED TRUE)

cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    # add c++11 support
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
if (NOT EXISTS ${THIRD_PARTY_DIR})
    file(MAKE_DIRECTORY ${THIRD_PARTY_DIR})
endif()
