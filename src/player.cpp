//
// Created by Thomas Lienbacher on 26.12.2020.
//

#include "player.hpp"
#include "window.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/epsilon.hpp>
#include "collisionhandler.hpp"

Player::Player(glm::vec3 spawnpoint, rp3d::PhysicsCommon *physx, rp3d::PhysicsWorld *world):
        physx(physx),
        world(world) {
    cam.position = spawnpoint + glm::vec3(0, EYE_HEIGHT - 1.0f, 3.0f);
    window::add_on_resize([this](int w, int h) {
        cam.projection = glm::perspective(glm::radians(80.0f), window::get_aspect_ratio(), 0.1f, 200.f);
    });

    rp3d::Transform transform({spawnpoint.x, spawnpoint.y + 0.01f, spawnpoint.z}, rp3d::Quaternion::identity());
    body = world->createRigidBody(transform);
    body->setUserData(BodyIdentifier::PLAYER.as_ptr());
    shape = physx->createCapsuleShape(RADIUS, HEIGHT - (RADIUS * 2.0f));
    auto c = body->addCollider(shape, rp3d::Transform({0.f, HEIGHT / 2.0f, 0.f}, rp3d::Quaternion::identity()));
    body->setLinearDamping(0.9f);
    c->getMaterial().setBounciness(0.f);

    glfwGetCursorPos(window::glfw_window, &last_mousex, &last_mousey);
}

Player::~Player() {
    world->destroyRigidBody(body);
    physx->destroyCapsuleShape(shape);
}

void Player::update(float delta) {
    body->setAngularVelocity(rp3d::Vector3::zero());
    auto t = body->getTransform();
    t.setOrientation(rp3d::Quaternion::identity());
    body->setTransform(t);

    double mx, my;
    glfwGetCursorPos(window::glfw_window, &mx, &my);
    auto movex = float(mx - last_mousex);
    auto movey = float(my - last_mousey);
    last_mousex = mx;
    last_mousey = my;
    cam.yaw += delta * movex * 4.0f;
    cam.pitch = glm::clamp(cam.pitch - (delta * movey * 4.0f), -90.0f, 90.f);

    auto force = rp3d::Vector3::zero();
    float current_speed = body->getLinearVelocity().length();
    if (current_speed > MAX_SPEED) {
        auto v = body->getLinearVelocity();
        auto y = v.y;
        v.y = 0;
        v.normalize();
        v *= MAX_SPEED;
        v.y = y;
        body->setLinearVelocity(v);
    }

    if (body->getLinearVelocity().y < 0.f) {
        force.y -= 19.5f;
    }


    if (window::is_key_pressed(GLFW_KEY_W)) {
        rp3d::Vector3 a = {std::cos(glm::radians(cam.yaw - 90.0f)),
                           0.0f,
                           std::sin(glm::radians(cam.yaw - 90.0f))};
        force += a;
    }
    if (window::is_key_pressed(GLFW_KEY_S)) {
        rp3d::Vector3 a = {-std::cos(glm::radians(cam.yaw - 90.0f)),
                           0.0f,
                           -std::sin(glm::radians(cam.yaw - 90.0f))};
        force += a;
    }

    if (window::is_key_pressed(GLFW_KEY_A)) {
        rp3d::Vector3 a = {-std::cos(glm::radians(cam.yaw)),
                           0.0f,
                           -std::sin(glm::radians(cam.yaw))};
        force += a;
    }
    if (window::is_key_pressed(GLFW_KEY_D)) {
        rp3d::Vector3 a = {std::cos(glm::radians(cam.yaw)),
                           0.0f,
                           std::sin(glm::radians(cam.yaw))};
        force += a;
    }

    auto y = force.y;
    force.y = 0;
    force.normalize();
    force *= 30.0f;
    force.y = y;
    body->applyForceToCenterOfMass(force);

    auto v = body->getTransform().getPosition();
    cam.position = {v.x, v.y + EYE_HEIGHT, v.z};
}

Camera &Player::camera() {
    return cam;
}

glm::mat4 Player::get_view_mat() {
    return cam.get_view_mat();
}

const glm::vec3 &Player::get_position() const {
    return cam.position;
}
