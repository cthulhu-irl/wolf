/*
    Project: Wolf Engine. Copyright © 2014-2023 Pooya Eimandar
    https://github.com/WolfEngine/wolf
*/

#pragma once

#include <stdint.h>

#include <boost/leaf.hpp>
#include <deque>
#include <gsl/gsl>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>

#if WIN32
#include <Windows.h>
#endif

#include "wolf/wolf.hpp"

class w_trace {
 public:
  struct stack {
    friend std::ostream &operator<<(std::ostream &p_os,
                                    stack const &p_trace) noexcept {
      try {
        p_os << "|tid:" << p_trace.thread_id << "|code:" << p_trace.err_code
             << "|msg:" << p_trace.err_msg << "|src:" << p_trace.source_file
             << "(" << p_trace.source_file_line << ")" << std::endl;
      } catch (...) {
      }
      return p_os;
    }
    std::thread::id thread_id;
    int64_t err_code = 0;
    std::string err_msg;
    std::string source_file;
    int source_file_line = 0;
  } W_ALIGNMENT_128;

  w_trace() noexcept = default;

  explicit w_trace(_In_ stack &&p_stack) noexcept {
    try {
      this->_stacks.emplace_front(p_stack);
    } catch (...) {
    }
  }

  w_trace(_In_ int64_t p_err_code, _In_ std::string p_err_msg,
          _In_ const char *p_source_file,
          _In_ int p_source_file_line) noexcept {
    try {
      this->_stacks.emplace_front(stack{std::this_thread::get_id(), p_err_code,
                                        std::move(p_err_msg), p_source_file,
                                        p_source_file_line});
    } catch (...) {
    }
  }

  w_trace(_In_ std::errc p_err_code, _In_ std::string p_err_msg,
          _In_ const char *p_source_file,
          _In_ int p_source_file_line) noexcept {
    try {
      this->_stacks.emplace_front(stack{
          std::this_thread::get_id(), gsl::narrow_cast<int64_t>(p_err_code),
          std::move(p_err_msg), p_source_file, p_source_file_line});
    } catch (...) {
    }
  }

  void push(_In_ int64_t p_err_code, _In_ std::string p_err_msg,
            _In_ const char *p_source_file,
            _In_ int p_source_file_line) noexcept {
    try {
      this->_stacks.emplace_front(stack{std::this_thread::get_id(), p_err_code,
                                        std::move(p_err_msg), p_source_file,
                                        p_source_file_line});
    } catch (...) {
    }
  }

  std::string to_string() const noexcept {
    std::string _result;
    try {
      std::stringstream ss;
      for (const auto &_stack : this->_stacks) {
        ss << _stack;
      }
      _result = ss.str();
    } catch (...) {
    }
    return _result;
  }

  friend std::ostream &operator<<(std::ostream &p_os,
                                  w_trace const &p_trace) noexcept {
    try {
      for (const auto &index : p_trace._stacks) {
        p_os << index;
      }
    } catch (...) {
    }
    return p_os;
  }

 private:
  std::deque<stack> _stacks = {};
};

template <typename T>
#ifndef __clang__
  requires std::movable<T>
#endif
constexpr inline boost::leaf::result<T> W_SUCCESS(T &p_param) noexcept {
  return boost::leaf::result<T>(std::move(p_param));
}

#define W_FAILURE(p_code, p_msg) \
  boost::leaf::new_error(w_trace(p_code, p_msg, __FILE__, __LINE__))

#ifdef WIN32

inline std::string get_last_win_error(_In_ DWORD p_error_id) {
  if (p_error_id == 0) {
    return {};
  }

  LPSTR _msg_buffer = nullptr;

  // Ask Win32 to give us the string version of that message ID.
  // The parameters we pass in, tell Win32 to create the buffer that holds the
  // message for us (because we don't yet know how long the message string
  // will be).
  const auto _size = FormatMessageA(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr, p_error_id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPSTR)&_msg_buffer, 0, NULL);

  // Copy the error message into a std::string.
  std::string _message(_msg_buffer, _size);

  // Free the Win32's string's buffer.
  LocalFree(_msg_buffer);

  return _message;
}

#endif
