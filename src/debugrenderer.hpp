//
// Created by Thomas Lienbacher on 15.12.2020.
//

#ifndef CASTLEGAME3D2_DEBUGRENDERER_HPP
#define CASTLEGAME3D2_DEBUGRENDERER_HPP

#include "master.hpp"
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include "camera.hpp"

namespace debug {
    void set_color(float r, float g, float b);

    void set_cam(Camera &cam);

    void point(glm::vec2 point);

    void point(glm::vec3 point);

    void sphere(glm::vec3 position, float radius);

    void triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
}

#endif //CASTLEGAME3D2_DEBUGRENDERER_HPP
