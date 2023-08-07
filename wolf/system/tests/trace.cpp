/*
    Project: Wolf Engine. Copyright © 2014-2023 Pooya Eimandar
    https://github.com/WolfEngine/wolf
*/

#if defined(WOLF_TEST)

#include <boost/test/unit_test.hpp>
#include <system/w_leak_detector.hpp>
#include <wolf.hpp>

BOOST_AUTO_TEST_CASE(trace_test)
{
  const wolf::system::w_leak_detector _detector = {};

  std::cout << "entering test case 'trace_test'" << std::endl;

  const auto _function_1 = []() noexcept -> boost::leaf::result<void>
  {
    return W_FAILURE(std::errc::bad_message, "error from function 1");
  };

  const auto _function_2 = [&]() noexcept -> boost::leaf::result<void>
  {
    return _function_1();
  };

  // how to use w_trace
  boost::leaf::try_handle_all(
      [&]() -> boost::leaf::result<void>
      {
        BOOST_LEAF_CHECK(_function_2());
        std::cout << "success!" << std::endl;
        BOOST_ERROR(false);
        return {};
      },
      [](const w_trace &p_trace)
      {
        const auto _msg = wolf::format("caught a test error! trace info: {}",
                                       p_trace.to_string());
        BOOST_WARN_MESSAGE(false, _msg);
      },
      []
      { BOOST_ERROR("trace_test caught an error!"); });

  std::cout << "leaving test case 'trace_test'" << std::endl;
}

#endif // WOLF_TESTS