//
// Created by Thomas Lienbacher on 15.12.2020.
//

#ifndef CASTLEGAME3D2_DEBUG_HPP
#define CASTLEGAME3D2_DEBUG_HPP

#include "master.hpp"
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include "camera.hpp"
#include <reactphysics3d/reactphysics3d.h>
#include "geometry.hpp"

namespace debug {

    void set_color(float r, float g, float b);

    void set_cam(Camera &cam);

    void disable_depth_next();

    void point(glm::vec2 point);

    void point(glm::vec3 point);

    void line(glm::vec3 a, glm::vec3 b);

    void sphere(glm::vec3 position, float radius);

    void triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

    void geometry(GeometryFormat &g);

    void render_physics(rp3d::DebugRenderer *renderer);

    glm::vec3 vec_into(const rp3d::Vector3 &v);

    rp3d::Vector3 vec_into(glm::vec3 v);
}

#endif //CASTLEGAME3D2_DEBUG_HPP
