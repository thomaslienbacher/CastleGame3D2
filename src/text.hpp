//
// Created by Thomas Lienbacher on 05.01.2021.
//

#ifndef CASTLEGAME3D2_TEXT_HPP
#define CASTLEGAME3D2_TEXT_HPP

#include "glalheaders.hpp"
#include <vector>
#include <string>
#include "texture.hpp"
#include "shader.hpp"

struct RawCharacter {
    int base_width = 0;
    int width_offset = 0;
    int x_offset = 0;
    int y_offset = 0;
};

struct BakedCharacter {
    float width;
    float height;

    union UVunion {
        float array[8];
        struct {
            struct {
                float x, y;
            } top_left;
            struct {
                float x, y;
            } top_right;
            struct {
                float x, y;
            } bottom_left;
            struct {
                float x, y;
            } bottom_right;
        } points;
    } uvs;
};

class Text;

class Font {
    friend Text;
    Texture *texture;
    std::unordered_map<char, BakedCharacter> chars;

public:
    Font(std::string deffile, Texture *texture);

    ~Font() = default;
};

class Text {
    GLuint vao;
    GLuint vbos[2];
    Font *font;
    int num_vertices;
    float width;

public:
    Text(std::string text, Font *font);

    ~Text();

    void render(Shader *font_shader);

    float get_width() const;
};


#endif //CASTLEGAME3D2_TEXT_HPP
