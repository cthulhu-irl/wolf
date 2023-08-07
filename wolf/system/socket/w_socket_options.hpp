/*
    Project: Wolf Engine. Copyright © 2014-2023 Pooya Eimandar
    https://github.com/WolfEngine/wolf
*/

#pragma once

#ifdef WOLF_SYSTEM_SOCKET

#include <functional>
#include <random>
#include <wolf.hpp>

// NOLINTBEGIN
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : ALL_CODE_ANALYSIS_WARNINGS)
#endif

#include <boost/asio.hpp>
#include <boost/system/errc.hpp>
#ifdef WOLF_SYSTEM_HTTP_WS
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/beast.hpp>
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif
// NOLINTEND

namespace wolf::system::socket
{

    inline std::string make_connection_id()
    {
        constexpr auto _max = 999;
        constexpr auto _min = 100;

        std::default_random_engine _rand_engine{};
        std::uniform_int_distribution<int> _rand_gen(_min, _max);

        const auto _now = std::chrono::system_clock::now();
        return wolf::format("{}_{}", _now, _rand_gen(_rand_engine));
    }

    struct w_socket_options
    {
        bool keep_alive = true;
        bool no_delay = true;
        bool reuse_address = true;
        int max_connections = boost::asio::socket_base::max_listen_connections;

        void set_to_socket(_Inout_ boost::asio::ip::tcp::socket &p_socket)
        {
            // set acceptor's options
            const auto _keep_alive_option =
                boost::asio::socket_base::keep_alive(this->keep_alive);

            const auto _reuse_address_option =
                boost::asio::socket_base::reuse_address(this->reuse_address);

            const auto _no_delay_opt = boost::asio::ip::tcp::no_delay(this->no_delay);

            p_socket.set_option(_no_delay_opt);
            p_socket.set_option(_keep_alive_option);
            p_socket.set_option(_reuse_address_option);
        }

        template <typename T>
        void set_to_acceptor(
            _Inout_ boost::asio::basic_socket_acceptor<T> &p_acceptor)
        {
            const auto _keep_alive_option =
                boost::asio::socket_base::keep_alive(this->keep_alive);
            p_acceptor.set_option(_keep_alive_option);

            const auto _reuse_address_option =
                boost::asio::socket_base::reuse_address(this->reuse_address);
            p_acceptor.set_option(_reuse_address_option);
        }
    };

#ifdef WOLF_SYSTEM_HTTP_WS
    using w_ws_stream = boost::beast::websocket::stream<
        typename boost::beast::tcp_stream::rebind_executor<
            typename boost::asio::use_awaitable_t<>::executor_with_default<
                boost::asio::any_io_executor>>::other>;

    typedef std::function<boost::beast::websocket::close_code(
        _In_ const std::string &p_conn_id, _Inout_ w_buffer &p_mut_data,
        _Inout_ bool &p_is_binary)>
        w_session_ws_on_data_callback;
#endif

    typedef std::function<boost::system::errc::errc_t(
        _In_ const std::string &p_conn_id, _Inout_ w_buffer &p_mut_data)>
        w_session_on_data_callback;

    typedef std::function<void(_In_ const std::string &p_conn_id,
                               _In_ const boost::system::system_error &p_error)>
        w_session_on_error_callback;

} // namespace wolf::system::socket

#endif