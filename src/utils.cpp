//
// Created by Thomas Lienbacher on 05.12.2020.
//

#include <fstream>
#include "utils.hpp"

std::string utils::file_to_string(std::filesystem::path &&path) {
    std::ifstream file(path);
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}
