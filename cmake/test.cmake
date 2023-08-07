enable_testing()

vcpkg_install(boost-test)
find_package(Boost ${BOOST_VERSION} REQUIRED)

set(TEST_PROJECT_NAME wolf_tests)
add_executable(${TEST_PROJECT_NAME})

target_sources(${TEST_PROJECT_NAME} PRIVATE wolf/tests.cpp)

target_compile_features(${TEST_PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_definitions(${TEST_PROJECT_NAME} PRIVATE WOLF_TEST)

target_link_libraries(${TEST_PROJECT_NAME}
    PRIVATE
        ${PROJECT_NAME}
        Boost::boost
)

add_subdirectory(wolf/system/tests)
add_subdirectory(wolf/media/tests)
add_subdirectory(wolf/stream/tests)
#add_subdirectory(wolf/ml/tests)

include(CTest)
add_test(NAME ${TEST_PROJECT_NAME} COMMAND ${TEST_PROJECT_NAME})

set_tests_properties(${TEST_PROJECT_NAME}
    PROPERTIES
        ENVIRONMENT WOLF_CONTENT_PATH="${PROJECT_SOURCE_DIR}/content"
)

# copy runtime dll files to the same directory as the executable.
if(WIN64)
    add_custom_command(TARGET ${TEST_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TEST_PROJECT_NAME}>  # Just so there would be enough arguments without DLLs
                                        $<TARGET_RUNTIME_DLLS:${TEST_PROJECT_NAME}>  # Linked DLLs
                                        $<TARGET_FILE_DIR:${TEST_PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()