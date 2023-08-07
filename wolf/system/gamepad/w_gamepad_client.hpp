#pragma once

#ifdef WOLF_SYSTEM_GAMEPAD_CLIENT

#include "w_gamepad_client_types.hpp"
#include <vector>
#include <wolf.hpp>

namespace wolf::system::gamepad
{
  class w_gamepad_client
  {
  public:
    W_API static boost::leaf::result<int> init() noexcept;

    W_API static bool
    get_events(_Inout_ w_gamepad_client_event &p_event) noexcept
    {
      if (_events.empty())
      {
        return false;
      }
      p_event = _events.back();
      _events.pop_back();
      return true;
    }

    // W_API static void update() noexcept;

    // W_API static void fini() noexcept;

  private:
    // LIFO queue of events
    static std::vector<w_gamepad_client_event> _events;
  };
} // namespace wolf::system::gamepad

#endif
