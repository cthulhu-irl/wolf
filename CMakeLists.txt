cmake_minimum_required(VERSION 3.21)

if (NOT DEFINED WOLF_MAIN_PROJECT)
    set(WOLF_MAIN_PROJECT OFF)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(WOLF_MAIN_PROJECT ON)
    endif()
endif()

# set the name of the projects
project(wolf
    DESCRIPTION "Wolf Engine"
    LANGUAGES CXX
)

message("CXX Compiler ID is ${CMAKE_CXX_COMPILER_ID}")

include(cmake/common.cmake)
include(cmake/vcpkg.cmake)

# set global configurations
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(BOOST_VERSION "1.82.0" CACHE STRING "Boost version tag")
option(BUILD_SHARED_LIBS "Whether to build shared or static library" ON)
option(WOLF_TEST "Enable tests" ${WOLF_MAIN_PROJECT})
option(WOLF_EXAMPLES "Build examples" ON)
option(WOLF_FEATURES_ALL "Enable all features by default" ${WOLF_MAIN_PROJECT})

# media modules
wolf_feature_option(WOLF_MEDIA_FFMPEG "Enable ffmpeg encoding and decoding" OFF) # WOLF_FEATURES_ALL AND (WIN64 OR ANDROID) AND NOT EMSCRIPTEN)
wolf_feature_option(WOLF_MEDIA_OPENAL "Enable OpenAL soft" WOLF_FEATURES_ALL AND NOT LINUX AND NOT EMSCRIPTEN)
wolf_feature_option(WOLF_MEDIA_SCREEN_CAPTURE "Enable screen capture" OFF)
wolf_feature_option(WOLF_MEDIA_STB "Enable stb headers" WOLF_FEATURES_ALL)
wolf_feature_option(WOLF_MEDIA_GSTREAMER "Enable gstreamer wrapper" OFF)

# stream modules
wolf_feature_option(WOLF_STREAM_GRPC "Enable gRPC connection" OFF) #WOLF_FEATURES_ALL AND NOT EMSCRIPTEN)
wolf_feature_option(WOLF_STREAM_QUIC "Enable QUIC protocol" OFF)
wolf_feature_option(WOLF_STREAM_RIST "Enable RIST streaming protocol" OFF)
wolf_feature_option(WOLF_STREAM_WEBRTC "Enable webRTC" OFF)

# system modules
wolf_feature_option(WOLF_SYSTEM_GAMEPAD_CLIENT "Enable gamepad input handling" WOLF_FEATURES_ALL AND WIN64 AND NOT EMSCRIPTEN)
wolf_feature_option(WOLF_SYSTEM_GAMEPAD_VIRTUAL "Enable virtual gamepad simulator" WOLF_FEATURES_ALL AND WIN64 AND NOT EMSCRIPTEN)
wolf_feature_option(WOLF_SYSTEM_HTTP_WS "Enable http1.1 and websocket client/server based on boost beast or Emscripten" WOLF_FEATURES_ALL)
wolf_feature_option(WOLF_SYSTEM_LOG "Enable log" WOLF_FEATURES_ALL AND NOT EMSCRIPTEN)
wolf_feature_option(WOLF_SYSTEM_LZ4 "Enable lz4 for compression" WOLF_FEATURES_ALL AND NOT EMSCRIPTEN)
wolf_feature_option(WOLF_SYSTEM_LZMA "Enable lzma for compression" WOLF_FEATURES_ALL AND DESKTOP)
wolf_feature_option(WOLF_SYSTEM_LUA "Enable lua scripting language" WOLF_FEATURES_ALL AND DESKTOP)
wolf_feature_option(WOLF_SYSTEM_MIMALLOC "Enable Microsoft's mimalloc memory allocator" WOLF_FEATURES_ALL AND DESKTOP)
wolf_feature_option(WOLF_SYSTEM_POSTGRESQL "Enable postgresql database client" WOLF_FEATURES_ALL AND DESKTOP)
wolf_feature_option(WOLF_SYSTEM_PYTHON "Enable embedded Python3 scripting" OFF) #WOLF_FEATURES_ALL AND DESKTOP AND NOT LINUX)
wolf_feature_option(WOLF_SYSTEM_REDIS "Enable redis client based on hiredis" OFF)
wolf_feature_option(WOLF_SYSTEM_SIG_SLOT "Enable signal/slot based on boost signals2" WOLF_FEATURES_ALL)
wolf_feature_option(WOLF_SYSTEM_SOCKET "Enable TCP/UDP protocol over socket" WOLF_FEATURES_ALL AND NOT APPLE AND NOT MOBILE)
wolf_feature_option(WOLF_SYSTEM_OPENSSL "Enable openSSL" WOLF_FEATURES_ALL)
wolf_feature_option(WOLF_SYSTEM_STACKTRACE "Enable boost stacktrace" WOLF_FEATURES_ALL AND WIN64 AND NOT EMSCRIPTEN)
wolf_feature_option(WOLF_SYSTEM_ZLIB "Enable Zlib compression library" WOLF_FEATURES_ALL AND NOT EMSCRIPTEN)

# machine learing modules
wolf_feature_option(WOLF_ML_NUDITY_DETECTION "Enable machine learning nudity detection" OFF)
wolf_feature_option(WOLF_ML_OCR "Enable machine learning referee ocr" OFF)

# complex/pre-built wolf dependencies to be made available.
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
if (WOLF_STREAM_QUIC OR WOLF_MEDIA_FFMPEG)
    set(WDEPS_MSQUIC ${WOLF_STREAM_QUIC})
    set(WDEPS_FFMPEG5 ${WOLF_MEDIA_FFMPEG})
    FetchContent_Declare(
        wolf-deps
        GIT_REPOSITORY https://github.com/WolfEngine/wolf-deps
        GIT_TAG        main
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(wolf-deps)
endif()

add_subdirectory(wolf)

if (WOLF_TEST AND NOT EMSCRIPTEN)
    include(cmake/test.cmake)
endif()

if (WOLF_EXAMPLES AND NOT MOBILE AND NOT EMSCRIPTEN)
    add_subdirectory(examples)
endif()