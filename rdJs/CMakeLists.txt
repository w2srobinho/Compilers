project(rdjs CXX C)
cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(build_tests "Build all tests Project." OFF)

include(setup)
include(bison_flex)

# sources
add_subdirectory(src/back_end)
add_subdirectory(src/front_end)

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
