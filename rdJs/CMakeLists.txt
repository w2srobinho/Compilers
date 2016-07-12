project(rdjs CXX C)
cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(build_tests "Build all tests Project." ON)

include(setup)
include(bison_flex)
include(llvm)

#message(STATUS "##LLVM INCLUDES: ${LLVM_INCLUDE_DIRS}")
#message(STATUS "##LLVM definitions: ${LLVM_DEFINITIONS}")

# sources
add_subdirectory(src/backend)
add_subdirectory(src/frontend)

# third_party
if(build_tests)
    # add googletest suite to compile with project
    include(gtest)
    get_gtest(${THIRD_PARTY_DIR}/gtest)

    # googletest and googlemock dependencies
    add_subdirectory(${GTEST_ROOT}/googlemock)
    enable_testing()

    # tests
    add_subdirectory(tests)
endif()
