if (EMSCRIPTEN)
    message(FATAL_ERROR "WOLF_STREAM_QUIC is not supported for wasm target.")
endif()

file(GLOB_RECURSE WOLF_STREAM_QUIC_HEADERS *.hpp)
file(GLOB_RECURSE WOLF_STREAM_QUIC_SOURCES *.cpp)
target_sources(${PROJECT_NAME}
    PRIVATE
    ${WOLF_STREAM_QUIC_HEADERS}
    ${WOLF_STREAM_QUIC_SOURCES}
)

# dependency is availbale by wolf-deps in root cmake.
target_link_libraries(${PROJECT_NAME} PUBLIC msquic::msquic)
