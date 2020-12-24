//
// Created by Thomas Lienbacher on 05.12.2020.
//

#include "utils.hpp"
#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

Texture::Texture(std::filesystem::path &&path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(reinterpret_cast<const char *>(path.generic_u8string().c_str()), &width, &height,
                                    &channels, 0);

    if (!data) {
        std::cerr << "Couldn't load " << path << panic;
    }

    GLint format = GL_RGB;

    if (channels == 4) {
        format = GL_RGBA;
    }

    glGenTextures(1, &this->id);

    float aniso = 0.0f;
    glBindTexture(GL_TEXTURE_2D, id);
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, aniso);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &this->id);
}

static GLuint texture_in_use = 0;

void Texture::bind() {
    if (id != texture_in_use) {
        glBindTexture(GL_TEXTURE_2D, id);
        texture_in_use = id;
    }
}

void Texture::bind(GLuint id) {
    if (id != texture_in_use) {
        glBindTexture(GL_TEXTURE_2D, id);
        texture_in_use = id;
    }
}
