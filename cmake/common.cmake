include_guard(GLOBAL)

include(FetchContent)

# helper macro to define feature option
# with conditional default value since
# cmake doesn't support returning value
# or boolean expression outside `if()`.
macro(wolf_feature_option FEATURE_NAME DESCRIPTION)
    if (${ARGN})
        option(${FEATURE_NAME} ${DESCRIPTION} ON)
    else()
        option(${FEATURE_NAME} ${DESCRIPTION} OFF)
    endif()
endmacro()

# check the OS
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    if (WIN32)
        set(WIN64 TRUE)
    endif()
endif()

if (MSVC AND NOT WIN64)
    message(FATAL_ERROR "Only Window 64 bit is supported")
endif()

if(UNIX AND (NOT EMSCRIPTEN))
   if (APPLE)
        if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            set(IOS TRUE)
        endif()
   else()
        set(LINUX TRUE)
   endif()
endif()

if (NOT EMSCRIPTEN)
    if(ANDROID OR IOS)
        set(MOBILE ON)
        message(STATUS "target platform is mobile")
    else()
        set(DESKTOP ON)
        message(STATUS "target platform is desktop")
    endif()
endif()
