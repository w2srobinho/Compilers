if(_GIT_INCLUDED)
    return()
endif()
set(_GIT_INCLUDED TRUE)

find_package(Git)
if(NOT GIT_FOUND)
    message(FATAL_ERROR "git not found on environment path")
endif()

# GIT(ARGS): Macro for calling the git command. Arguments are passed directly
#            to the command. Keyword arguments intended for execute_process()
#            may be used.
macro(GIT)
    execute_process(COMMAND ${GIT_EXECUTABLE} ${ARGN} RESULT_VARIABLE GIT_RESULT)
endmacro()

#GIT_FATAL(MESSAGE ARGS): Same as GIT(ARGS), but fatal on failure. A message
#                         needs to be passed as the first argument.
macro(GIT_FATAL MESSAGE)
    GIT(${ARGN})
    if(NOT (GIT_RESULT EQUAL 0))
        message(FATAL_ERROR "${MESSAGE}")
    endif()
endmacro()

# git_clone(branch remote output_dir ARGN): Function to clone repository. 
#                                           A branch remote and output_dir 
#                                           needs to be passed.
function(git_clone branch remote output_dir)
    GIT(clone --branch ${branch} ${remote} ${output_dir} ${ARGN})
    if(NOT (GIT_RESULT EQUAL 0))
        message(FATAL_ERROR "Could not clone '" ${remote} "' repository.")
    endif()
endfunction()
