
if (WOLF_SYSTEM_LZ4)
    if (EMSCRIPTEN)
        message(FATAL_ERROR "the wasm32 target is not supported for WOLF_SYSTEM_LZ4")
    endif()

    set(WOLF_SYSTEM_LZ4_HEADERS w_lz4.hpp)
    set(WOLF_SYSTEM_LZ4_SOURCES w_lz4.cpp)
    target_sources(${PROJECT_NAME}
        PRIVATE
            ${WOLF_SYSTEM_LZ4_HEADERS}
            ${WOLF_SYSTEM_LZ4_SOURCES}
    )

    vcpkg_install(lz4)
    find_package(lz4 CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC lz4::lz4)
endif()

if (WOLF_SYSTEM_LZMA)
    if (EMSCRIPTEN)
          message(FATAL_ERROR "the wasm32 target is not supported for WOLF_SYSTEM_LZMA")
    endif()

    set(WOLF_SYSTEM_LZMA_HEADERS w_lzma.hpp)
    set(WOLF_SYSTEM_LZMA_SOURCES w_lzma.cpp)

    message("fetching https://github.com/WolfEngine/lzma.git")
    FetchContent_Declare(
        lzma
        GIT_REPOSITORY https://github.com/WolfEngine/lzma.git
        GIT_TAG        main
        GIT_SHALLOW    ON
    )
    set(FETCHCONTENT_QUIET OFF)
    FetchContent_MakeAvailable(lzma)

    target_link_libraries(${PROJECT_NAME} PUBLIC lzma)
endif()
