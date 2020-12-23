//
// Created by Thomas Lienbacher on 05.12.2020.
//

#ifndef CASTLEGAME3D2_UTILS_HPP
#define CASTLEGAME3D2_UTILS_HPP

#include "master.hpp"
#include <filesystem>
#include <iostream>

namespace utils {
    std::string file_to_string(std::filesystem::path &&path);
}

static std::string _panic(const char *file, const char *function, int line) {
    return " : in '" + std::string(file) + ":" + std::to_string(line) + "' '" + std::string(function) + "'";
}

#define panic _panic(__FILE__, __PRETTY_FUNCTION__, __LINE__) << std::endl; exit(1)

#endif //CASTLEGAME3D2_UTILS_HPP
