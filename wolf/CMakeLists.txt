# use BUILD_SHARED_LIB to set SHARED or STATIC build.
add_library(${PROJECT_NAME} "")
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE wolf.hpp wolf.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# common options and definitons

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3 /bigobj)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC NDEBUG)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC
    BOOST_ASIO_NO_DEPRECATED
    BOOST_ASIO_HAS_CO_AWAIT
    BOOST_ASIO_HAS_STD_COROUTINE 
)
if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        WIN64
        _WIN32_WINNT=0x0601 # Windows 7
    )
elseif(EMSCRIPTEN)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EMSCRIPTEN)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NEED_XLOCALE_H=1)
endif()

# primary dependencies

vcpkg_install(ms-gsl)
find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Microsoft.GSL::GSL)

vcpkg_install(boost-leaf boost-signals2 boost-asio)
find_package(Boost ${BOOST_VERSION} REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost)

if (NOT MSVC AND NOT EMSCRIPTEN)
    vcpkg_install(fmt)
    find_package(fmt REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt-header-only)
endif()

# modules

add_subdirectory(system)
add_subdirectory(media)
add_subdirectory(stream)
# TODO(PooyaEimandar): add_subdirectory(ml)
