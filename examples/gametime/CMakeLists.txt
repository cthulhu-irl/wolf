cmake_minimum_required(VERSION 3.20)

project(wolf_example_gametime
    LANGUAGES CXX
    VERSION 1.0
)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Quick REQUIRED)

qt_add_executable(${PROJECT_NAME}
    main.cpp
)

qt_add_qml_module(${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES
        main.qml
    SOURCES
    q_custom_render_item.hpp
    q_custom_render_item.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Quick
        wolf::wolf
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
    COMMAND Qt6::windeployqt
    ARGS $<TARGET_FILE:${PROJECT_NAME}>
)
