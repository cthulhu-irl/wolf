if (EMSCRIPTEN)
    message(FATAL_ERROR "WOLF_STREAM_GRPC feature is not supported for wasm target.")
endif()

set(WOLF_STREAM_GRPC_HEADERS
    w_grpc_client.hpp
    w_grpc_server.hpp
)
set(WOLF_STREAM_GRPC_SOURCES
    w_grpc_client.cpp
    w_grpc_server.cpp
)
target_sources(${PROJECT_NAME}
    PRIVATE
        ${WOLF_STREAM_GRPC_HEADERS}
        ${WOLF_STREAM_GRPC_SOURCES}
)

vcpkg_install(asio-grpc)
find_package(asio-grpc CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC asio-grpc::asio-grpc)

target_compile_definitions(${PROJECT_NAME} PUBLIC WOLF_SYSTEM_GRPC)
