project(frontend)

BISON_TARGET(RdJsParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
FLEX_TARGET(RdJsScanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(RdJsScanner RdJsParser)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${backend_SOURCE_DIR}
)

add_executable(rdjs ${BISON_RdJsParser_OUTPUTS} ${FLEX_RdJsScanner_OUTPUTS} main.cpp)
target_link_libraries(rdjs backend)
