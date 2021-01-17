//
// Created by Thomas Lienbacher on 05.12.2020.
//

#include <vector>
#include <glm/ext.hpp>
#include "shader.hpp"
#include "utils.hpp"

Shader::Shader(std::string &src_vertex_shader, std::string &src_fragment_shader) {
    this->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    this->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    char infolog[512];
    int result;

    char *src = const_cast<char *>(src_vertex_shader.c_str());
    glShaderSource(vertex_shader, 1, &src, nullptr);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
    glGetShaderInfoLog(vertex_shader, 512, nullptr, infolog);
    if (!result) {
        std::cerr << "Shader failed " << std::string(infolog) << panic;
    }

    src = const_cast<char *>(src_fragment_shader.c_str());
    glShaderSource(fragment_shader, 1, &src, nullptr);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
    glGetShaderInfoLog(fragment_shader, 512, nullptr, infolog);
    if (!result) {
        std::cerr << "Shader failed " << std::string(infolog) << panic;
    }

    this->program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramInfoLog(program, 512, nullptr, infolog);
    if (!result) {
        std::cerr << "Programm failed " << std::string(infolog) << panic;
    }
}

Shader::~Shader() {
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(program);
}

static GLuint program_in_use = 0;

void Shader::use() const {
    if (program != program_in_use) {
        glUseProgram(program);
        program_in_use = program;
    }
}

void Shader::set_uniform(std::string &&name, glm::mat4 val) {
    use();

    if (uniform_locations.contains(name)) {
        glUniformMatrix4fv(uniform_locations[name], 1, GL_FALSE, glm::value_ptr(val));
    } else {
        GLint loc = glGetUniformLocation(program, name.c_str());

        if (loc == -1) {
            std::cerr << "Uniform " << name << " no location" << panic;
        }

        uniform_locations[name] = loc;
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
    }
}

void Shader::set_uniform(std::string &&name, glm::vec2 val) {
    use();

    if (uniform_locations.contains(name)) {
        glUniform2f(uniform_locations[name], val.x, val.y);
    } else {
        GLint loc = glGetUniformLocation(program, name.c_str());

        if (loc == -1) {
            std::cerr << "Uniform " << name << " no location" << panic;
        }

        uniform_locations[name] = loc;
        glUniform2f(uniform_locations[name], val.x, val.y);
    }
}

void Shader::set_uniform(std::string &&name, glm::vec3 val) {
    use();

    if (uniform_locations.contains(name)) {
        glUniform3f(uniform_locations[name], val.x, val.y, val.z);
    } else {
        GLint loc = glGetUniformLocation(program, name.c_str());

        if (loc == -1) {
            std::cerr << "Uniform " << name << " no location" << panic;
        }

        uniform_locations[name] = loc;
        glUniform3f(uniform_locations[name], val.x, val.y, val.z);
    }
}

void Shader::use(GLuint program) {
    if (program != program_in_use) {
        glUseProgram(program);
        program_in_use = program;
    }
}

void Shader::set_uniform(std::string &&name, float val) {
    use();

    if (uniform_locations.contains(name)) {
        glUniform1f(uniform_locations[name], val);
    } else {
        GLint loc = glGetUniformLocation(program, name.c_str());

        if (loc == -1) {
            std::cerr << "Uniform " << name << " no location" << panic;
        }

        uniform_locations[name] = loc;
        glUniform1f(uniform_locations[name], val);
    }
}
