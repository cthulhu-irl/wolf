#pragma once

#include "wolf/wolf.hpp"

#include <string_view>
#include <filesystem>

constexpr const char* ENVKEY_CONTENT_PATH = "WOLF_CONTENT_PATH";
inline std::filesystem::path get_content_path(
    const std::string_view p_subpath) {
  auto env_content_path = wolf::get_env(ENVKEY_CONTENT_PATH);
  return env_content_path.empty()
             ? std::filesystem::current_path().append(p_subpath)
             : std::filesystem::path(env_content_path).append(p_subpath);
}
