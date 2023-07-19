#pragma once

#include <string_view>
#include <filesystem>
#include <unordered_map>

#include <boost/leaf.hpp>

#include "wolf/wolf.hpp"

namespace wolf::system {

class W_API w_dotenv {
public:
    w_dotenv() = delete;

    static boost::leaf::result<void> load(const std::filesystem::path&);
    static boost::leaf::result<void> load_from_string(const std::string_view);
};

}  // namespace wolf::system
