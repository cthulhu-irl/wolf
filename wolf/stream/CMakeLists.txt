if (WOLF_STREAM_GRPC)
    add_subdirectory(grpc)
endif()

if (WOLF_STREAM_QUIC)
    add_subdirectory(quic)
endif()

if (WOLF_STREAM_JANUS)
    add_subdirectory(janus)
endif()

if (WOLF_STREAM_RIST)
    add_subdirectory(rist)
endif()

if (WOLF_STREAM_WEBRTC)
    add_subdirectory(webrtc)
endif()
