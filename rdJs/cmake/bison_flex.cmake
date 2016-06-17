if(_BISON_FLEX_INCLUDED)
    return()
endif()

set(_BISON_FLEX_INCLUDED TRUE)

cmake_minimum_required(VERSION 2.8)

find_package(BISON)
if (NOT BISON_FOUND)
    message(FATAL_ERROR "Bison not found on environment path.")
endif()

find_package(FLEX)
if (NOT FLEX_FOUND)
    message(FATAL_ERROR "Flex not found on environment path.")
endif()

# add_definitions(-D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)
