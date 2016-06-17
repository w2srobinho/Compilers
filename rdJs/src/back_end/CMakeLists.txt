project(back_end)

FILE(GLOB HEADER_FILES "*.h")
FILE(GLOB SOURCE_FILES "*.cpp")

add_library(back_end
    ${SOURCE_FILES} 
    ${HEADER_FILES}
)
