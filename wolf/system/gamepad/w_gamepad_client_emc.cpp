#if defined(WOLF_SYSTEM_GAMEPAD_CLIENT) && defined(EMSCRIPTEN)

#include "w_gamepad_client.hpp"
#include "w_gamepad_client_keymap.hpp"

// NOLINTBEGIN
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : ALL_CODE_ANALYSIS_WARNINGS)
#endif

#include <emscripten/html5.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif
// NOLINTEND

using w_gamepad_client = wolf::system::gamepad::w_gamepad_client;
using w_gamepad_event = wolf::system::gamepad::w_gamepad_event;

std::vector<EmscriptenGamepadEvent> gamepads;
std::vector<w_gamepad_event> w_gamepad_client::_events;

EM_BOOL gamepadconnected_callback(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData)
{
  gamepads.push_back(*gamepadEvent);
  reinterpret_cast<w_gamepad_client *>(userData)->update();

  return EM_TRUE;
}

EM_BOOL gamepaddisconnected_callback(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData)
{
  std::erase_if(gamepads, [gamepadEvent](const auto &e)
                { return gamepadEvent->index == e.index; });

  return EM_TRUE;
}

boost::leaf::result<int> w_gamepad_client::init() noexcept
{
  // TODO: callback registration may fail, check for EMSCRIPTEN_RESULT_SUCCESS
  emscripten_set_gamepadconnected_callback(this, EM_TRUE,
                                           gamepadconnected_callback);
  emscripten_set_gamepaddisconnected_callback(this, EM_TRUE,
                                              gamepaddisconnected_callback);

  return S_OK;
}

void w_gamepad_client::update()
{
  emscripten_sample_gamepad_data();

  for (auto &gamepad : gamepads)
  {
    EmscriptenGamepadEvent gamepadState;
    auto result = emscripten_get_gamepad_status(gamepad.index, &gamepadState);

    if (result != EMSCRIPTEN_RESULT_SUCCESS)
    {
      continue;
    }

    if (gamepadState.timestamp != 0 &&
        gamepadState.timestamp == gamepad.timestamp)
    {
      continue;
    }

    for (int i = 0; i < gamepadState.numButtons; i++)
    {
      if (gamepad.digitalButton[i] == gamepadState.digitalButton[i])
      {
        continue;
      }

      auto state = gamepadState.digitalButton[i]
                       ? w_gamepad_state_type::PRESSED
                       : w_gamepad_state_type::RELEASED;
      w_gamepad_button button{.type = w_gamepad_event_type::BUTTON,
                              .which = (std::uint32_t)gamepad.index,
                              .button = w_gamepad_button_map[i],
                              .state = state};
      w_gamepad_event event{.button = button};
      _events.emplace_back(event);
    }

    for (int i = 0; i < gamepadState.numAxes; i++)
    {
      if (gamepad.axis[i] == gamepadState.axis[i])
      {
        continue;
      }

      auto value = (std::int16_t)(32767. * gamepadState.axis[i]);
      w_gamepad_axis axis{.type = w_gamepad_event_type::AXIS,
                          .which = (std::uint32_t)gamepad.index,
                          .axis = w_gamepad_axis_map[i],
                          .value = value};
      w_gamepad_event event{.axis = axis};
      _events.emplace_back(event);
    }

    gamepad = gamepadState;
  }
}

void w_gamepad_client::fini() noexcept
{
  this->gamepads.clear();
  this->_events.clear();
}

#endif //  defined(WOLF_SYSTEM_GAMEPAD) && defined(EMSCRIPTEN)