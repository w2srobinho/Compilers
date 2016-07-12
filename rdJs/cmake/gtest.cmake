if(_GTEST_INCLUDED)
    return()
endif()

set(_GTEST_INCLUDED TRUE)
find_package(Threads REQUIRED)

# get_gtest (output_dir ARGN): Function to take a googletest dependecy.
#                              A output_dir needs to be passed.
#                              GTEST_ROOT_DIR Set path to googletest root directory
function(get_gtest output_dir)
    set(GTEST_REMOTE https://github.com/google/googletest)
    set(GTEST_ROOT "${output_dir}" CACHE PATH "Path to googletest root directory")

    if(NOT EXISTS ${output_dir})
        include(git)
        git_clone(master ${GTEST_REMOTE} ${output_dir} ERROR_QUIET)
    endif()
endfunction()
