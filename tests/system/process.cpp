/*
    Project: Wolf Engine. Copyright © 2014-2023 Pooya Eimandar
    https://github.com/WolfEngine/wolf
*/

#ifdef WOLF_TEST

#include <boost/test/unit_test.hpp>
#include <system/w_leak_detector.hpp>
#include <system/w_process.hpp>
#include <wolf/wolf.hpp>

BOOST_AUTO_TEST_CASE(process_current_path_test) {
  const wolf::system::w_leak_detector _detector = {};
  using w_process = wolf::system::w_process;

  std::cout << "entering test case 'process_current_path_test'" << std::endl;

  const auto _path = w_process::current_path();
  BOOST_REQUIRE(_path.has_error() == false);
  BOOST_REQUIRE(_path.value().empty() == false);

  const auto _exe_path = w_process::current_exe_path();
  BOOST_REQUIRE(_exe_path.has_error() == false);
  BOOST_REQUIRE(_exe_path.value().empty() == false);

  std::cout << "leaving test case 'process_current_path_test'" << std::endl;
}

#endif  // WOLF_TEST