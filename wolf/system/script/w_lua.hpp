#pragma once

#ifdef WOLF_SYSTEM_LUA

#include <wolf.hpp>

#define SOL_EXCEPTIONS_SAFE_PROPAGATION 1
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <filesystem>

namespace wolf::system::script
{
  using w_lib = sol::lib;
  class w_lua
  {
  public:
    W_API w_lua()
        : _lua(std::make_shared<sol::state>(
              sol::c_call<decltype(&w_lua::w_lua_panic), &w_lua::w_lua_panic>)),
          _load_result(nullptr, -1, 0, 0, sol::load_status::syntax) {}

    W_API ~w_lua()
    {
      _release();
    }

    // move constructor.
    W_API w_lua(w_lua &&p_other) noexcept { _move(std::forward<w_lua &&>(p_other)); }
    // move assignment operator.
    W_API w_lua &operator=(w_lua &&p_other) noexcept
    {
      _move(std::forward<w_lua &&>(p_other));
      return *this;
    }

    /**
     * open lua libraries
     * @param p_libs library enum
     */
    template <typename... ARGS>
    W_API void open_libraries(ARGS &&...p_libs)
    {
      this->_lua->open_libraries(std::forward<ARGS>(p_libs)...);
    }

    /**
     * load lua script from file
     * @param p_file file path
     */
    W_API boost::leaf::result<void> load_file(const std::filesystem::path &p_file)
    {
      _load_result = this->_lua->load_file(p_file.string());
      if (!_load_result.valid())
      {
        sol::error err = _load_result;
        return W_FAILURE(1, err.what());
      }
      return {};
    }

    /**
     * load from buffer
     * @param p_str string buffer
     */
    W_API boost::leaf::result<void> load_from_buffer(const std::string_view p_str)
    {
      _load_result = this->_lua->load(p_str);
      if (!_load_result.valid())
      {
        sol::error err = _load_result;
        return W_FAILURE(1, err.what());
      }
      return {};
    }

    /**
     * run previously loaded lua script
     * @return result
     */
    template <typename... RETYPES, typename... ARGS>
    W_API boost::leaf::result<std::tuple<RETYPES...>> run(ARGS &&...p_args)
    {
      if (!_load_result.valid())
      {
        return W_FAILURE(1, "no script currently loaded");
      }
      auto result = _load_result(std::forward<ARGS>(p_args)...);
      if (!result.valid())
      {
        sol::error err = result;
        return W_FAILURE(1, err.what());
      }

      std::tuple<RETYPES...> value;
      try
      {
        value = result;
      }
      catch (...)
      {
        return W_FAILURE(1, "can not convert arguments");
      }
      return value;
    }

    /**
     * run lua script from file
     * @param p_file file path
     */
    template <typename... RETYPES>
    W_API boost::leaf::result<std::tuple<RETYPES...>> run_file(const std::filesystem::path &p_file)
    {
      auto result = this->_lua->script_file(p_file.string());
      if (!result.valid())
      {
        sol::error err = result;
        return W_FAILURE(1, err.what());
      }

      std::tuple<RETYPES...> value;
      try
      {
        value = result;
      }
      catch (...)
      {
        return W_FAILURE(1, "can not convert arguments");
      }
      return value;
    }

    /**
     * run from buffer
     * @param p_str string buffer
     */
    template <typename... RETYPES>
    W_API boost::leaf::result<std::tuple<RETYPES...>> run_from_buffer(const std::string_view &p_str)
    {
      auto result = this->_lua->script(p_str);
      if (!result.valid())
      {
        sol::error err = result;
        return W_FAILURE(1, err.what());
      }

      std::tuple<RETYPES...> value;
      try
      {
        value = std::move(result);
      }
      catch (...)
      {
        return W_FAILURE(1, "can not convert arguments");
      }
      return value;
    }

    /**
     * bind to the function
     * @param p_lua_function lua function name
     * @param p_args c++ function
     */
    template <typename... ARGS>
    W_API boost::leaf::result<void> bind_to_function(const std::string &p_lua_function,
                                                     ARGS &&...p_args)
    {
      try
      {
        this->_lua->set_function(p_lua_function, std::forward<ARGS>(p_args)...);
      }
      catch (...)
      {
        return W_FAILURE(1, "can not set function");
      }
      return {};
    }

    /**
     * call lua functions
     * @param p_name lua function name
     * @param p_args function arguments
     */
    template <typename... RETYPES, typename... ARGS>
    W_API boost::leaf::result<std::tuple<RETYPES...>> call_function(const std::string_view p_name,
                                                                    ARGS &&...p_args)
    {
      auto _ptr = this->_lua.get();
      sol::protected_function function = (*_ptr)[p_name];
      auto result = function(std::forward<ARGS>(p_args)...);
      if (!result.valid())
      {
        sol::error err = result;
        return W_FAILURE(1, err.what());
      }

      std::tuple<RETYPES...> value;
      try
      {
        value = result;
      }
      catch (...)
      {
        return W_FAILURE(1, "can not convert arguments");
      }

      return value;
    }

    /**
     * return the lua state for custom operation
     * @return soL state object
     */
    W_API std::shared_ptr<sol::state> get_state() { return this->_lua; };

    /**
     * get global variable
     * @param p_name name of variable
     * @param pValue value of variable
     * @return result
     */
    template <typename T>
    W_API boost::leaf::result<void> get_global_variable(const std::string_view p_name, T &p_value)
    {
      try
      {
        p_value = (*this->_lua)[p_name];
      }
      catch (...)
      {
        return W_FAILURE(1, "can not get global variable");
      }
      return {};
    }

    /**
     *  set global variable
     * @param p_name name of variable
     * @param p_value value of variable
     * @return result
     */
    template <typename... ARGS>
    W_API boost::leaf::result<void> set_global_variable(std::string_view p_name, ARGS &&...p_value)
    {
      try
      {
        this->_lua->set(p_name, std::forward<ARGS>(p_value)...);
      }
      catch (...)
      {
        return W_FAILURE(1, "can not set global variable");
      }

      return {};
    }

  private:
    // disable copy constructor
    w_lua(const w_lua &) = delete;
    // disable copy operator
    w_lua &operator=(const w_lua &) = delete;

    void _release()
    {
      if (this->_lua)
      {
        this->_lua->stack_clear();
        this->_lua->collect_garbage();
      }
    }

    void _move(w_lua &&p_other) noexcept
    {
      if (this == &p_other)
      {
        return;
      }
      this->_lua = std::move(p_other._lua);
      this->_load_result = std::move(p_other._load_result);
    }

    inline void w_lua_panic(sol::optional<std::string> p_maybe_msg)
    {
      if (p_maybe_msg)
      {
        throw std::runtime_error(p_maybe_msg.value());
      }
    }

    std::shared_ptr<sol::state> _lua;
    sol::load_result _load_result;
  };
} // namespace wolf::system::script

#endif
