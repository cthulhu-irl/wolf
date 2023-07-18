/*
    Project: Wolf Engine. Copyright © 2014-2023 Pooya Eimandar
    https://github.com/WolfEngine/wolf
*/

#ifdef WOLF_TEST

#include <boost/test/unit_test.hpp>
#include <system/compression/w_lz4.hpp>
#include <system/compression/w_lzma.hpp>
#include <system/w_leak_detector.hpp>
#include <wolf/wolf.hpp>

#ifdef WOLF_SYSTEM_LZ4

BOOST_AUTO_TEST_CASE(compress_lz4_test) {
  const wolf::system::w_leak_detector _detector = {};

  std::cout << "entering test case 'compress_lz4_test'" << std::endl;

  boost::leaf::try_handle_all(
      [&]() -> boost::leaf::result<void> {
        using lz4 = wolf::system::compression::w_lz4;

        constexpr auto _mock_compression_data =
            "HELLO WOLF\r\nHELLO WOLF!*&%!HELLO WOLF!07*&%!\r\nThe quick "
            "brown fox jumps over the lazy dog!";

        const auto _bytes = std::span(
            reinterpret_cast<const std::byte *>(_mock_compression_data),
            strlen(_mock_compression_data));
        const auto _bytes_len = _bytes.size();

        constexpr auto _max_retry = 10;
        BOOST_LEAF_AUTO(_compress_default, lz4::compress_default(_bytes));

        BOOST_LEAF_AUTO(_decompress_default,
                        lz4::decompress(_compress_default, _max_retry));
        BOOST_REQUIRE(_decompress_default.size() == _bytes_len);

        constexpr auto _acceleration = 1000;
        BOOST_LEAF_AUTO(_compress_fast,
                        lz4::compress_fast(_bytes, _acceleration));

        BOOST_LEAF_AUTO(_decompress_fast,
                        lz4::decompress(_compress_fast, _max_retry));
        BOOST_REQUIRE(_decompress_fast.size() == _bytes_len);

        return {};
      },
      [](const w_trace &p_trace) {
        const auto _msg = wolf::format("compress_lz4_test got an error : {}",
                                       p_trace.to_string());
        BOOST_ERROR(_msg);
      },
      [] { BOOST_ERROR("compress_lz4_test got an error!"); });

  std::cout << "leaving test case 'compress_lz4_test'" << std::endl;
}

#endif  // WOLF_SYSTEM_LZ4

#ifdef WOLF_SYSTEM_LZMA

BOOST_AUTO_TEST_CASE(compress_lzma_test) {
  const wolf::system::w_leak_detector _detector = {};

  std::cout << "entering test case 'compress_lzma_test'" << std::endl;

  boost::leaf::try_handle_all(
      [&]() -> boost::leaf::result<void> {
        using lzma = wolf::system::compression::w_lzma;

        constexpr auto _mock_compression_data =
            "HELLO WOLF\r\nHELLO WOLF!*&%!HELLO WOLF!07*&%!\r\nThe quick "
            "brown fox jumps over the lazy dog!";

        const auto _bytes = std::span(
            reinterpret_cast<const std::byte *>(_mock_compression_data),
            strlen(_mock_compression_data));
        const auto _bytes_len = _bytes.size();

        BOOST_LEAF_AUTO(_compress_lzm1, lzma::compress_lzma1(_bytes, 7));

        BOOST_LEAF_AUTO(_decompress_lzm1,
                        lzma::decompress_lzma1(_compress_lzm1));

        BOOST_LEAF_AUTO(_compress_lzm2, lzma::compress_lzma2(_bytes, 7));

        BOOST_LEAF_AUTO(_decompress_lzm2,
                        lzma::decompress_lzma2(_compress_lzm2));

        return {};
      },
      [](const w_trace &p_trace) {
        const auto _msg = wolf::format("compress_lzma_test got an error : {}",
                                       p_trace.to_string());
        BOOST_ERROR(_msg);
      },
      [] { BOOST_ERROR("compress_lzma_test got an error!"); });

  std::cout << "leaving test case 'compress_lzma_test'" << std::endl;
}

#endif  // WOLF_SYSTEM_LZMA

#endif  // WOLF_TESTS