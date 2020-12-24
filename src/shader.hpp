//
// Created by Thomas Lienbacher on 05.12.2020.
//

#ifndef CASTLEGAME3D2_SHADER_HPP
#define CASTLEGAME3D2_SHADER_HPP

#include "master.hpp"
#include <unordered_map>
#include <glm/glm.hpp>
#include <string>

class Shader {
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;

    std::unordered_map<std::string, GLint> uniform_locations;

public:
    Shader() = default;

    Shader(std::string &src_vertex_shader, std::string &src_fragment_shader);

    ~Shader();

    void set_uniform(std::string &&name, glm::mat4 val);

    void use() const;

    static void use(GLuint program);
};


#endif //CASTLEGAME3D2_SHADER_HPP
