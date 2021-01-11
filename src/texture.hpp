//
// Created by Thomas Lienbacher on 05.12.2020.
//

#ifndef CASTLEGAME3D2_TEXTURE_HPP
#define CASTLEGAME3D2_TEXTURE_HPP

#include "glalheaders.hpp"
#include <filesystem>

class Texture {
    GLuint id;

public:

    Texture(std::filesystem::path &&path, GLenum mag_filter = GL_NEAREST, GLenum min_filter = GL_NEAREST_MIPMAP_LINEAR);

    ~Texture();

    void bind();

    void bind_unit(int unit) const;

    static void bind(GLuint id);
};


#endif //CASTLEGAME3D2_TEXTURE_HPP
