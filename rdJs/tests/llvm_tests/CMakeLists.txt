project(llvm_tests CXX C)

set(SOURCE_FILES
    single_main.cpp
    two_functions.cpp
)

include_directories(${LLVM_INCLUDE_DIRS})

add_executable(llvm_tests
    ${SOURCE_FILES}
)

target_link_libraries(llvm_tests
    ${llvm_libs}
)

#add_test(NAME some_tests
#         WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIR}
#         COMMAND some_tests)
