//
// Created by Thomas Lienbacher on 06.12.2020.
//

#ifndef CASTLEGAME3D2_CAMERA_HPP
#define CASTLEGAME3D2_CAMERA_HPP

#include "master.hpp"
#include <glm/glm.hpp>

class Camera {
public:
    glm::mat4 projection;
    glm::mat4 view;

    glm::vec3 position;
    float pitch;
    float yaw;

    Camera();

    glm::mat4 get_projview_mat();
    glm::mat4 get_proj_mat();
    glm::mat4 get_view_mat();

};


#endif //CASTLEGAME3D2_CAMERA_HPP
