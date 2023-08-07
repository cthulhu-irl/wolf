/*
    Project: Wolf Engine. Copyright © 2014-2023 Pooya Eimandar
    https://github.com/WolfEngine/wolf
*/

#include <boost/test/unit_test.hpp>

#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <system/w_leak_detector.hpp>
#include <wolf.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(coroutine_test)
{
  const wolf::system::w_leak_detector _detector = {};

  std::cout << "entering test case 'coroutine_test'" << std::endl;

  boost::leaf::try_handle_all(
      [&]() -> boost::leaf::result<void>
      {
        auto _io = boost::asio::io_context();
        boost::asio::co_spawn(
            _io,
            [&]() -> boost::asio::awaitable<void>
            {
              using namespace boost::asio::experimental::awaitable_operators;

              auto _coro = [](_In_ bool p_abort)
                  -> boost::asio::awaitable<std::errc>
              {
                if (p_abort)
                {
                  co_return std::errc::connection_aborted;
                }
                co_return std::errc::timed_out;
              };

              const auto _ret = co_await (_coro(true) || _coro(false));
              BOOST_REQUIRE(std::get<0>(_ret) == std::errc::connection_aborted);
            },
            boost::asio::detached);

        _io.run();

        return {};
      },
      [](const w_trace &p_trace)
      {
        const auto _msg = wolf::format("coroutine_test got an error : {}",
                                       p_trace.to_string());
        BOOST_ERROR(_msg);
      },
      []
      { BOOST_ERROR("coroutine_test got an error!"); });

  std::cout << "leaving test case 'coroutine_test'" << std::endl;
}
