project(google_sample_tests CXX C)

include_directories(
    ${gtest_SOURCE_DIR}/include
    ${google_sample_SOURCE_DIR}
)

set(SOURCE_FILES
    sample1_unittest.cc
)

add_executable(google_sample_tests    
    ${SOURCE_FILES}
)

target_link_libraries(google_sample_tests
    back_end
    gtest_main
)

add_test(NAME google_sample_tests
         WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIR}
         COMMAND google_sample_tests)
