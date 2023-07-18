/*
    Project: Wolf Engine. Copyright © 2014-2023 Pooya Eimandar
    https://github.com/WolfEngine/wolf
*/

#if defined(WOLF_TEST) && defined(WOLF_MEDIA_STB)

#include <boost/test/unit_test.hpp>
#include "wolf/media/stb/w_image.hpp"
#include "wolf/system/w_leak_detector.hpp"

BOOST_AUTO_TEST_CASE(image_load_save_test) {
  const wolf::system::w_leak_detector _detector = {};

  std::cout << "entering test case 'image_load_save_test'" << std::endl;

  boost::leaf::try_handle_all(
      [&]() -> boost::leaf::result<void> {
        using w_image = wolf::media::w_image;
        using w_image_data = wolf::media::w_image_data;

        const auto &_rgb_file = std::filesystem::current_path().append(
            "../../content/texture/rgb.png");
        auto _src_image_res = w_image::load(_rgb_file, 0);
        // POOYA: don't use BOOST_LEAF_AUTO because CodeCov might break
        if (!_src_image_res.has_error()) {
          BOOST_LEAF_AUTO(_src_image, std::move(_src_image_res));
          const auto &_bmp_path =
              std::filesystem::current_path().append("/rgb_bmp.png");
          BOOST_LEAF_CHECK(w_image::save_bmp(_bmp_path, _src_image));

          constexpr auto _quality = 90;
          const auto &_jpg_path =
              std::filesystem::current_path().append("/rgb_jpg.png");
          BOOST_LEAF_CHECK(w_image::save_jpg(_jpg_path, _src_image, _quality));
        }

        return {};
      },
      [](const w_trace &p_trace) {
        const auto _msg = wolf::format("got error: {}", p_trace.to_string());
        BOOST_ERROR(_msg);
      },
      [] {
        const auto _msg = wolf::format("got an error");
        BOOST_ERROR(_msg);
      });

  std::cout << "leaving test case 'image_load_save_test'" << std::endl;
}

#endif