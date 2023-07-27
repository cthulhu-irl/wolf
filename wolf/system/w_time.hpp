/*
    Project: Wolf Engine. Copyright © 2014-2023 Pooya Eimandar
    https://github.com/WolfEngine/wolf
*/
#if !defined(__APPLE__) && !defined(ANDROID) && !defined(EMSCRIPTEN)

#pragma once

#include <wolf/wolf.hpp>
#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
using namespace boost::asio::experimental::awaitable_operators;

namespace wolf::system
{

    class w_timer : public boost::asio::high_resolution_timer
    {
    public:
        W_API explicit w_timer(boost::asio::io_context &p_io_context)
            : boost::asio::high_resolution_timer(p_io_context, 0) {}
    };

    class w_time
    {
    public:
        static boost::asio::awaitable<std::errc> timeout(
            _In_ const std::chrono::steady_clock::time_point &p_deadline) noexcept;
    };

} // namespace wolf::system

#endif