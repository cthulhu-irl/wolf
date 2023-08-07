/*
    Project: Wolf Engine. Copyright © 2014-2023 Pooya Eimandar
    https://github.com/WolfEngine/wolf
*/
#if defined(WOLF_SYSTEM_GAMEPAD_VIRTUAL) && !defined(EMSCRIPTEN)

#pragma once

#include <wolf.hpp>
#include "w_gamepad_virtual.hpp"

namespace wolf::system::gamepad
{
    class w_gamepad_virtual_pool
    {
    public:
        /**
         * initialize ViGEm bus
         * @returns VIGEM_ERROR
         */
        W_API static boost::leaf::result<VIGEM_ERROR> init() noexcept;

        /**
         * add a gamepad to the ViGEm bus
         * @returns a tuple which contains index of gamepad and VIGEM_ERROR
         */
        W_API static boost::leaf::result<w_gamepad_virtual> add() noexcept;

        /**
         * convert a ViGEm error to string
         * @returns a string of VIGEM_ERROR
         */
        W_API static std::string vigem_error_to_string(
            _In_ const VIGEM_ERROR p_error) noexcept;

        /**
         * finitialize initialize ViGEm bus
         * @returns a string of VIGEM_ERROR
         */
        W_API static void fini() noexcept;
    };
} // namespace wolf::system::gamepad

#endif // defined(WOLF_SYSTEM_VIRTUAL_GAMEPAD) && !defined(EMSCRIPTEN)
