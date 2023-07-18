set(WOLF_SYSTEM_COMMON_HEADERS
    w_overloaded.hpp
    w_flags.hpp
    w_trace.hpp
    w_gametime.hpp
)
set(WOLF_SYSTEM_COMMON_SOURCES
    w_overloaded.cpp
    w_flags.cpp
    w_trace.cpp
    w_gametime.cpp
)
if (NOT EMSCRIPTEN)
    list(APPEND WOLF_SYSTEM_COMMON_HEADERS
        w_process.hpp
        w_time.hpp
        w_leak_detector.hpp
    )
    list(APPEND WOLF_SYSTEM_COMMON_SOURCES
        w_process.cpp
        w_time.cpp
        w_leak_detector.cpp
    )
endif()
target_sources(${PROJECT_NAME}
    PRIVATE
        ${WOLF_SYSTEM_COMMON_HEADERS}
        ${WOLF_SYSTEM_COMMON_SOURCES}
)

if (WOLF_SYSTEM_OPENSSL)
    # no source codes, only link against openssl library.
    vcpkg_install_force(openssl)
    find_package(OpenSSL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif()

if (WOLF_SYSTEM_POSTGRESQL OR WOLF_SYSTEM_REDIS)
    add_subdirectory(db)
endif()

if (WOLF_SYSTEM_GAMEPAD_CLIENT OR WOLF_SYSTEM_GAMEPAD_VIRTUAL)
    add_subdirectory(gamepad)
endif()

if (WOLF_SYSTEM_LZ4 OR WOLF_SYSTEM_LZMA)
    add_subdirectory(compression)
endif()

if (WOLF_SYSTEM_SOCKET OR WOLF_SYSTEM_HTTP_WS)
    add_subdirectory(socket)
endif()

if (WOLF_SYSTEM_LUA OR WOLF_SYSTEM_PYTHON)
    add_subdirectory(script)
endif()
