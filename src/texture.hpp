//
// Created by Thomas Lienbacher on 05.12.2020.
//

#ifndef CASTLEGAME3D2_TEXTURE_HPP
#define CASTLEGAME3D2_TEXTURE_HPP

#include "master.hpp"
#include <filesystem>

class Texture {
    GLuint id;

public:
    Texture(unsigned int width, unsigned int height, GLint format, void* data);
    ~Texture();

    static Texture from_file(std::filesystem::path &&path);

    void bind();

    static void bind(GLuint id);
};


#endif //CASTLEGAME3D2_TEXTURE_HPP
