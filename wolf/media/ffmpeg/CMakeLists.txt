set(WOLF_MEDIA_FFMPEG_HEADERS
    w_av_config.hpp
    w_av_format.hpp
    w_av_frame.hpp
    w_av_packet.hpp
    w_decoder.hpp
    w_encoder.hpp
    w_ffmpeg_ctx.hpp
    w_ffmpeg.hpp
)
set(WOLF_MEDIA_FFMPEG_SOURCES
    w_av_config.cpp
    w_av_format.cpp
    w_av_frame.cpp
    w_av_packet.cpp
    w_decoder.cpp
    w_encoder.cpp
    w_ffmpeg_ctx.cpp
    w_ffmpeg.cpp
)
target_sources(${PROJECT_NAME}
    PRIVATE
        ${WOLF_MEDIA_FFMPEG_HEADERS}
        ${WOLF_MEDIA_FFMPEG_SOURCES}
)

# dependency is availbale by wolf-deps in root cmake.
target_link_libraries(${PROJECT_NAME} PUBLIC ffmpeg::ffmpeg)
target_compile_definitions(${PROJECT_NAME} PUBLIC WOLF_MEDIA_FFMPEG)
