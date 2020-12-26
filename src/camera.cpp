//
// Created by Thomas Lienbacher on 06.12.2020.
//

#include <glm/gtc/matrix_transform.hpp>
#include "camera.hpp"
#include "window.hpp"

Camera::Camera() {
    projection = glm::perspective(glm::radians(75.0f), window::get_aspect_ratio(), 0.1f, 200.f);
    view = glm::mat4(1.0f);
    position = glm::vec3();
    pitch = 0;
    yaw = 0;
}

glm::mat4 Camera::get_projview_mat() {
    return projection * this->get_view_mat();
}

glm::mat4 Camera::get_proj_mat() {
    return projection;
}

glm::mat4 Camera::get_view_mat() {
    auto camera = glm::mat4(1.0f);
    camera = glm::translate(camera, position);
    auto yawmat = glm::rotate(glm::mat4(1.0f), glm::radians(-yaw), glm::vec3(0, 1, 0));
    auto pitchmat = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), glm::vec3(1, 0, 0));
    auto local_view = glm::inverse(camera * yawmat * pitchmat);
    this->view = local_view;
    return local_view;
}
