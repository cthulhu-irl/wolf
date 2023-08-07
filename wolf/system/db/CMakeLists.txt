if (WOLF_SYSTEM_POSTGRESQL)
    if (ANDROID)
        message(FATAL_ERROR "WOLF_SYSTEM_POSTGRESQL is not supported for Android")
    elseif(EMSCRIPTEN)
        message(FATAL_ERROR "WOLF_SYSTEM_POSTGRESQL is not supported for the wasm32 target on Emscripten")
    endif()

    vcpkg_install(libpq)
    find_package(PostgreSQL REQUIRED)

    set(WOLF_SYSTEM_POSTGRESQL_HEADERS w_postgresql.hpp)
    set(WOLF_SYSTEM_POSTGRESQL_SOURCES w_postgresql.cpp)
    target_sources(${PROJECT_NAME}
        PRIVATE
            ${WOLF_SYSTEM_POSTGRESQL_HEADERS}
            ${WOLF_SYSTEM_POSTGRESQL_SOURCES}
    )

    target_link_libraries(${PROJECT_NAME} PUBLIC PostgreSQL::PostgreSQL)
endif()

if (WOLF_SYSTEM_REDIS)
    set(WOLF_SYSTEM_REDIS_HEADERS db/w_redis_client.hpp)
    set(WOLF_SYSTEM_REDIS_SOURCES db/w_redis_client.cpp)
    target_sources(${PROJECT_NAME}
        PRIVATE
            ${WOLF_SYSTEM_REDIS_HEADERS}
            ${WOLF_SYSTEM_REDIS_SOURCES}
    )

    if (ANDROID)
        message(FATAL_ERROR "WOLF_SYSTEM_REDIS is not supported for Android")
    elseif(EMSCRIPTEN)
        message(FATAL_ERROR "WOLF_SYSTEM_REDIS is not supported for the wasm32 target on Emscripten")
    endif()

    if (NOT WOLF_SYSTEM_OPENSSL)
        message(FATAL_ERROR "WOLF_SYSTEM_REDIS requires WOLF_SYSTEM_OPENSSL")
    endif()

    vcpkg_install(boost-json)

    message("fetching https://github.com/boostorg/redis.git")
    FetchContent_Declare(
        boost_redis
        GIT_REPOSITORY https://github.com/boostorg/redis.git
        GIT_TAG        develop
        GIT_SHALLOW    ON
    )

    set(FETCHCONTENT_QUIET OFF)
    FetchContent_MakeAvailable(boost_redis)

    # target_inlcude_directories(${PROJECT_NAME} PUBLIC ${boost_redis_SOURCE_DIR}/include)
    target_link_libraries(${PROJECT_NAME} PUBLIC boost::redis)
endif()
