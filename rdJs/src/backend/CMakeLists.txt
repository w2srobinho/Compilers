project(backend)

FILE(GLOB HEADER_FILES "*.h")
FILE(GLOB SOURCE_FILES "*.cpp")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.
include_directories(${LLVM_INCLUDE_DIRS})

add_library(backend
        ${SOURCE_FILES}
        ${HEADER_FILES}
        )

target_link_libraries(backend ${llvm_libs})
