target_sources(${TEST_PROJECT_NAME}
    PRIVATE
        compress.cpp
        gamepad.cpp
        gametime.cpp
        log.cpp
        lua.cpp
        postgresql.cpp
        process.cpp
        python.cpp
        redis.cpp
        signal_slot.cpp
        tcp.cpp
        trace.cpp
        ws.cpp
)

if (NOT ANDROID)
    target_sources(${TEST_PROJECT_NAME}
        PRIVATE
            coroutine.cpp
    )
endif()
