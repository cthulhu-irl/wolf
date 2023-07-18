vcpkg_install_force(boost-test)
find_package(Boost 1.82 REQUIRED)

set(TEST_PROJECT_NAME wolf_tests)
add_executable(${TEST_PROJECT_NAME})

target_sources(${TEST_PROJECT_NAME} PRIVATE common.hpp main.cpp)

target_compile_features(${TEST_PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_definitions(${TEST_PROJECT_NAME} PRIVATE WOLF_TEST)

target_link_libraries(${TEST_PROJECT_NAME}
    PRIVATE
        ${PROJECT_NAME}
        Boost::boost
)

add_subdirectory(system)
add_subdirectory(media)
add_subdirectory(stream)
# TODO add_subdirectory(ml)

include(CTest)
add_test(NAME ${TEST_PROJECT_NAME} COMMAND ${TEST_PROJECT_NAME})

set_tests_properties(${TEST_PROJECT_NAME}
    PROPERTIES
        ENVIRONMENT WOLF_CONTENT_PATH="${PROJECT_SOURCE_DIR}/content"
)

# copy runtime dll files to the same directory as the executable.
if(WIN32)
    add_custom_command(TARGET ${TEST_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TEST_PROJECT_NAME}>  # Just so there would be enough arguments without DLLs
                                         $<TARGET_RUNTIME_DLLS:${TEST_PROJECT_NAME}>  # Linked DLLs
                                         $<TARGET_FILE_DIR:${TEST_PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()
