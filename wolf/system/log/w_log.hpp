/*
    Project: Wolf Engine. Copyright © 2014-2023 Pooya Eimandar
    https://github.com/WolfEngine/wolf
*/

#pragma once

#ifdef WOLF_SYSTEM_LOG

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : ALL_CODE_ANALYSIS_WARNINGS)
#endif
// NOLINTBEGIN

#include <wolf.hpp>

#include "w_log_config.hpp"

#ifdef _MSC_VER
#include <spdlog/sinks/msvc_sink.h>
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif
// NOLINTEND

namespace wolf::system::log
{
  class w_log
  {
  public:
    // constructor
    W_API explicit w_log(w_log_config &&p_config) noexcept;

    // move constructor.
    W_API w_log(w_log &&p_other) noexcept
    {
      _move(std::forward<w_log &&>(p_other));
    }
    // move assignment operator.
    W_API w_log &operator=(w_log &&p_other) noexcept
    {
      _move(std::forward<w_log &&>(p_other));
      return *this;
    }

    // destructor
    W_API virtual ~w_log() noexcept;

    W_API boost::leaf::result<int> init();

    W_API void write(_In_ const std::string_view &p_fmt);

    W_API void write(_In_ const spdlog::level::level_enum &p_level,
                     _In_ const std::string_view &p_fmt);

#ifdef _MSC_VER

    template <class... Args>
    W_API void write(_In_ const std::string_view p_fmt, _In_ Args &&...p_args)
    {
      const auto _str = std::vformat(p_fmt, std::make_format_args(p_args...));
      write(_str);
    }

    template <class... Args>
    W_API void write(_In_ const spdlog::level::level_enum &p_level,
                     _In_ const std::string_view p_fmt, _In_ Args &&...p_args)
    {
      const auto _str = std::vformat(p_fmt, std::make_format_args(p_args...));
      write(p_level, _str);
    }

#else
    template <class... Args>
    W_API void write(_In_ const fmt::format_string<Args...> p_fmt,
                     _In_ Args &&...p_args)
    {
      const auto _str =
          fmt::vformat(p_fmt, fmt::make_format_args(p_args...));
      write(_str);
    }

    template <class... Args>
    W_API void write(_In_ const spdlog::level::level_enum &p_level,
                     _In_ const fmt::format_string<Args...> p_fmt,
                     _In_ Args &&...p_args)
    {
      const auto _str =
          fmt::vformat(p_fmt, fmt::make_format_args(p_args...));
      write(p_level, _str);
    }

#endif // _MSC_VER

    W_API boost::leaf::result<int> flush();

    W_API w_log_config get_config() const;

  private:
    // disable copy constructor
    w_log(const w_log &) = delete;
    // disable copy operator
    w_log &operator=(const w_log &) = delete;

    W_API void _move(_Inout_ w_log &&p_other) noexcept;

    w_log_config _config = {};
    std::shared_ptr<spdlog::logger> _logger = nullptr;
    std::shared_ptr<spdlog::logger> _async_file_logger = nullptr;
  };
} // namespace wolf::system::log

#endif
