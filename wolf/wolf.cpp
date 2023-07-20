﻿#include "wolf.hpp"

#ifdef WOLF_SYSTEM_MIMALLOC

#include <mimalloc-new-delete.h>
#include <mimalloc-override.h>

#endif

#ifdef WOLF_SYSTEM_STACKTRACE

#include <csignal>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stacktrace>
#include <system/w_process.hpp>

static void w_signal_handler(int p_signum) {
  auto _path = wolf::system::w_process::current_exe_path();
  if (_path.has_error()) {
    return;
  }

  const auto &_path_str = _path.value().append("wolf.dump");

  std::ignore = signal(p_signum, nullptr);

  std::stringstream _traces;

#ifdef __clang__
#pragma unroll
#endif  // __clang__
  for (const auto &trace : std::stacktrace()) {
    _traces << "name: " << trace.description()
            << " source_file: " << trace.source_file() << "("
            << std::to_string(trace.source_line()) << ")\r\n";
  }

  std::fstream _file;
  _file.open(_path_str.string(), std::ios_base::out);
  if (_file.is_open()) {
    _file << _traces.str();
    _file.close();
  }

  std::ignore = raise(SIGABRT);
}

#endif

std::string wolf::w_init() {
#if defined(WOLF_SYSTEM_STACKTRACE) && !defined(WOLF_TESTS)
  std::ignore = signal(SIGSEGV, &w_signal_handler);
  std::ignore = signal(SIGABRT, &w_signal_handler);
#endif

  // Making incompatible API changes
  constexpr auto WOLF_MAJOR_VERSION = 3;
  // Adding functionality in a backwards - compatible manner
  constexpr auto WOLF_MINOR_VERSION = 0;
  // bug fixes
  constexpr auto WOLF_PATCH_VERSION = 0;
  // for debugging
  constexpr auto WOLF_DEBUG_VERSION = 0;

  auto _version =
      wolf::format("v{}.{}.{}.{}", WOLF_MAJOR_VERSION, WOLF_MINOR_VERSION,
                   WOLF_PATCH_VERSION, WOLF_DEBUG_VERSION);
  return _version;
}

std::string wolf::get_env(const std::string_view p_env) {
  std::string val;
  if (p_env.empty()) {
    return val;
  }

#ifdef _MSC_VER
  char *buf = nullptr;
  size_t size = 0;
  if (_dupenv_s(&buf, &size, p_env.data()) == 0 && buf != nullptr) {
    val = buf;
    free(buf);
  }
#else
  const auto buf = std::getenv(p_env.data());
  if (buf != nullptr) {
    val = buf;
  }
#endif

  return val;
}

void wolf::set_env(const std::string_view p_key, const std::string_view p_value)
{
#ifdef _MSC_VER
    _putenv_s(p_key.data(), p_value.data());
#else
#error TODO putenv in non-windows systems.
#endif
}
