# Wolf Engine [![Apache licensed](https://img.shields.io/badge/license-Apache-blue)](https://github.com/WolfEngine/wolf/blob/main/LICENSE.md) [![codecov](https://codecov.io/github/WolfEngine/wolf/branch/main/graph/badge.svg?token=AhoU9QV7eS)](https://codecov.io/github/WolfEngine/wolf) [![windows-x64-Microsoft C++ Code Analysis](https://github.com/WolfEngine/wolf/actions/workflows/msvc.yml/badge.svg?branch=main)](https://github.com/WolfEngine/wolf/actions/workflows/msvc.yml) [![osx-arm64](https://github.com/WolfEngine/wolf/actions/workflows/osx.yml/badge.svg?branch=main)](https://github.com/WolfEngine/wolf/actions/workflows/osx.yml) [![ubuntu-x64-CodeQL Analysis](https://github.com/WolfEngine/wolf/actions/workflows/linux.yml/badge.svg?branch=main)](https://github.com/WolfEngine/wolf/actions/workflows/linux.yml) [![android-multi-ABIs](https://github.com/WolfEngine/wolf/actions/workflows/android.yml/badge.svg?branch=main)](https://github.com/WolfEngine/wolf/actions/workflows/android.yml)[![wasm32-unknown-emscripten](https://github.com/WolfEngine/wolf/actions/workflows/wasm.yml/badge.svg?branch=main)](https://github.com/WolfEngine/wolf/actions/workflows/wasm.yml)

<img src="https://raw.githubusercontent.com/WolfEngine/wolf/main/Logo.png" width="256" height="256" alt="wolf"/>

The Wolf Engine is the next generation of the [Persian Game Engine](https://github.com/PooyaEimandar/PersianEngine), which is a cross-platform open-source game engine created by [Pooya Eimandar](https://pooyaeimandar.github.io/). Wolf is a comprehensive set of C++ modules for realtime streaming, system & media programming, machine learning and real time rendering. It supports Lua and WASM as embedded scripting languages.

# Build
- Prerequisites 
    - For Windows, make sure install the latest Windows 11/10 SDK
	- [git](https://git-scm.com/downloads)
	- [CMake](https://cmake.org/download/)
	- [vcpkg](https://vcpkg.io/)
	- [Meson](https://github.com/mesonbuild/meson/releases)
	- [Ninja](https://ninja-build.org/)
	- `autoconf` will be needed for Linux or OSX. For Linux use the following command `sudo apt-get install autoconf` and for OSX use `brew install autoconf`
	- [QT6](https://www.qt.io/download) for demos and examples
	- [NDK](https://developer.android.com/ndk/downloads) for android
	
then make sure get the main branch 
`git clone https://github.com/WolfEngine/wolf.git --branch main --depth 1`

## CMakePresets
	
To list configure presets: `cmake . --list-presets`
To list build presets: `cmake --build --list-presets`
To install wolf: `cmake --install <build-dir> --prefix <prefix>`

For example for building wolf for android:
```
cmake . --preset android-arm64-release
cmake --build --preset android-arm64-release
```

For example for building wolf for windows:
```
cmake . --preset windows-x64-msvc-static
cmake --build --preset windows-x64-msvc-static
cmake --install C:/wolf/build/windows-x64-msvc-static --prefix C:/wolf
```

## Supported platforms

| Not Supported | Planned | In Progress | Done |
|:-----------:|:-----------:|:-----------:|:-----------:|
| :x: | :memo: | :construction: | :white_check_mark: | 

### Supported platforms and APIs for render module

| API | Windows | Linux | macOS | iOS | Android | Wasm |
|:-----------:|:-----------:|:--------------------------:|:--------------:|:-------------:|:--------------:|:-------------:|
| GPU | Vulkan/OpenGL ES :construction: | Vulkan/OpenGL ES :memo: | MoltenVK :memo: | MoltenVK :memo: | Vulkan/OpenGL ES :memo: | WebGL/WebGPU :memo: |

### Supported platforms and APIs for media module

| API | Windows | Linux | macOS | iOS | Android | Wasm |
|:-----------:|:-----------:|:--------------------------:|:--------------:|:-------------:|:--------------:|:-------------:|
| [Bitmap](https://github.com/WolfEngine/wolf/blob/main/tests/media/ffmpeg.cpp) | :white_check_mark: | :white_check_mark: | :memo: | :memo: | :memo: | :x: |
| [FFmpeg](https://github.com/WolfEngine/wolf/blob/main/tests/stream/ffmpeg_stream.cpp) | :white_check_mark: | :white_check_mark: | :memo: | :memo: | :memo: | :x: |
| [JPEG](https://github.com/WolfEngine/wolf/blob/main/tests/media/ffmpeg.cpp) | :white_check_mark: | :white_check_mark: | :memo: | :memo: | :memo: | :x: |
| [OpenAL](https://github.com/WolfEngine/wolf/blob/main/tests/media/openal.cpp) | :white_check_mark: | :white_check_mark: | :memo: | :memo: | :memo: | :x: |
| [PNG](https://github.com/WolfEngine/wolf/blob/main/tests/media/ffmpeg.cpp) | :white_check_mark: | :white_check_mark: | :memo: | :memo: | :memo: | :x: |
| WebP | :memo: | :memo: | :memo: | :memo: | :memo: | :x: |

### Supported platforms and APIs for stream module

| API | Windows | Linux | macOS | iOS | Android | Wasm |
|:-----------:|:-----------:|:--------------------------:|:--------------:|:-------------:|:--------------:|:-------------:|
| gRPC | :memo: | :x: | :x: | :x: | :x: | :x: |
| [Janus](https://github.com/WolfEngine/wolf/tree/main/wolf_demo/wasm) | :construction: | :x: | :x: | :x: | :x: | :white_check_mark: |
| [QUIC](https://github.com/WolfEngine/wolf/blob/main/tests/stream/quic.cpp) | :white_check_mark: | :memo: | :memo: | :memo: | :memo: | :x: |
| [RIST](https://github.com/WolfEngine/wolf/blob/main/tests/stream/rist.cpp) | :white_check_mark: | :memo: | :memo: | :memo: | :white_check_mark: | :x: |
| RTMP | :memo: | :x: | :x: | :x: | :x: | :x: |
| [RTSP](https://github.com/WolfEngine/wolf/blob/main/tests/stream/ffmpeg_stream.cpp) | :white_check_mark: | :memo: | :memo: | :memo: | :memo: | :x: |
| [SRT](https://github.com/WolfEngine/wolf/blob/main/tests/stream/ffmpeg_stream.cpp) | :white_check_mark: | :memo: | :memo: | :memo: | :memo: | :x: |
| webRTC | :memo: | :memo: | :memo: | :memo: | :memo: | :memo: |
| [WebSocket](https://github.com/WolfEngine/wolf/blob/main/tests/system/ws.cpp) | :white_check_mark: | :white_check_mark: | :memo: | :memo: | :memo: | :memo: |

### Supported platforms and APIs for system module

| API | Windows | Linux | macOS | iOS | Android | Wasm |
|:-----------:|:-----------:|:--------------------------:|:--------------:|:-------------:|:--------------:|:-------------:|
| [Coroutine](https://github.com/WolfEngine/wolf/blob/main/tests/system/coroutine.cpp) | :white_check_mark: | :white_check_mark: | :memo: | :x: | :x: | :x: |
| [GameTime](https://github.com/WolfEngine/wolf/blob/main/tests/system/gametime.cpp) | :white_check_mark: | :white_check_mark: | :memo: | :memo: | :memo: | :white_check_mark: |
| [Gamepad](https://github.com/WolfEngine/wolf/blob/main/tests/system/gamepad.cpp) | :white_check_mark: | :white_check_mark: | :memo: | :memo: | :memo: | :white_check_mark: |
| [Virtual Gamepad](https://github.com/WolfEngine/wolf/blob/main/tests/system/gamepad.cpp) | :white_check_mark: | :x: | :x: | :x: | :x: | :x: |
| [Log](https://github.com/WolfEngine/wolf/blob/main/tests/system/log.cpp)  | :white_check_mark: | :white_check_mark: | :construction: | :construction: | :construction: | :construction: | 
| LuaJit  | :memo: | :memo: | :memo: | :memo: | :memo: | :x: |
| [LZ4](https://github.com/WolfEngine/wolf/blob/main/tests/system/compress.cpp)  | :white_check_mark: | :white_check_mark: | :memo: | :memo: | :memo: | :x: |
| [LZMA](https://github.com/WolfEngine/wolf/blob/main/tests/system/compress.cpp)  | :white_check_mark: | :white_check_mark: | :memo: | :x: | :x: | :x: |
| OpenTelemetry  | :memo: | :memo: | :memo: | :x: | :x: | :x: |
| RAFT  | :memo: | :memo: | :memo: | :memo: | :memo: | :memo: |
| [Redis](https://github.com/WolfEngine/wolf/blob/main/tests/system/redis.cpp) | :white_check_mark: | :memo: | :memo: | :x: | :x: | :x: |
| Screen Capture  | :memo: | :construction: | :construction: | :x: | :x: | :x: |
| [Signal Slot](https://github.com/WolfEngine/wolf/blob/main/tests/system/signal_slot.cpp)  | :white_check_mark: | :white_check_mark: | :construction: | :x: | :x: | :x: |
| [TCP](https://github.com/WolfEngine/wolf/blob/main/tests/system/tcp.cpp) | :white_check_mark: | :white_check_mark: | :memo: | :memo: | :memo: | :x: |
| [Trace](https://github.com/WolfEngine/wolf/blob/main/tests/system/trace.cpp) | :white_check_mark: | :white_check_mark: | :memo: | :memo: | :memo: | :x: |
| UDP | :construction: | :memo: | :memo: | :memo: | :memo: | :x: |
| Wasm3  | :memo: | :memo: | :memo: | :memo: | :memo: | :memo: |

## Projects using Wolf</h2>
* [Wolf.Playout](https://www.youtube.com/watch?v=EZSdEjBvuGY), a playout automation software
* [Falcon](https://youtu.be/ygpz35ddZ_4), a real time 3D monitoring system
* [PlayPod](https://playpod.ir), the first cloud gaming platform launched in Middle East
* [Arium](https://arium.gg), play and rival other users via our leagues and duels from any device, any location and let your skills generate income

## [Youtube](https://www.youtube.com/c/WolfEngine)
## [Twitter](https://www.twitter.com/Wolf_Engine)

Wolf Engine © 2014-2023 [Pooya Eimandar](https://www.linkedin.com/in/pooyaeimandar/)
