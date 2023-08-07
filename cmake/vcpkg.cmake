include_guard(GLOBAL)

if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "VCPKG_ROOT environment variable was not set.")
endif()

if (WIN64)
    set(VCPKG_PROGRAM "$ENV{VCPKG_ROOT}/vcpkg.exe")
else()
    set(VCPKG_PROGRAM "$ENV{VCPKG_ROOT}/vcpkg")
endif()

function(vcpkg_install PACKAGE_NAME_ARGS)
    foreach(ARG IN LISTS ARGV)
        message(STATUS "installing ${ARG} via vcpkg")
        execute_process(COMMAND ${VCPKG_PROGRAM} install ${ARG} --triplet=${VCPKG_TARGET_TRIPLET})
    endforeach()
endfunction()
