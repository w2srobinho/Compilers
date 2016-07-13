if(_LLVM_INCLUDED)
    return()
endif()
set(_LLVM_INCLUDED TRUE)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.
add_definitions(${LLVM_DEFINITIONS})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libraries(llvm_libs core native jit)
