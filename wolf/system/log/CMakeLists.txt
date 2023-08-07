if (WOLF_SYSTEM_LOG)
    if (EMSCRIPTEN)
        message(FATAL_ERROR "WOLF_SYSTEM_LOG is not supported for wasm target.")
    endif()

    set(WOLF_SYSTEM_LOG_HEADERS w_log.hpp w_log_config.hpp)
    set(WOLF_SYSTEM_LOG_SOURCES w_log.cpp)
    target_sources(${PROJECT_NAME}
        PRIVATE
            ${WOLF_SYSTEM_LOG_HEADERS}
            ${WOLF_SYSTEM_LOG_SOURCES}
    )

    vcpkg_install(spdlog)
    find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog spdlog::spdlog_header_only)
endif()
