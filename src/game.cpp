//
// Created by Thomas Lienbacher on 21.12.2020.
//

#include "game.hpp"
#include "debug.hpp"
#include "window.hpp"
#include "utils.hpp"
#include "model.hpp"
#include <glm/gtc/matrix_transform.hpp>

Game::Game() : physx() {
    auto settings = rp3d::PhysicsWorld::WorldSettings();
    settings.gravity = rp3d::Vector3(0.f, -3.0f, 0.f);
    settings.worldName = "main_world";
    world = physx.createPhysicsWorld(settings);
    world->setIsDebugRenderingEnabled(true);
    world->enableSleeping(false);

    auto logger = physx.createDefaultLogger();
    auto logLevel = (unsigned int) rp3d::Logger::Level::Warning | (unsigned int) rp3d::Logger::Level::Error;
    logger->addStreamDestination(std::cout, logLevel, rp3d::DefaultLogger::Format::Text);
    physx.setLogger(logger);

    debug_renderer = &world->getDebugRenderer();
    debug_renderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
    debug_renderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
    debug_renderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
    debug_renderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
    debug_renderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, false);

    cam.position = glm::vec3(0, 0, 2.0f);
    window::add_on_resize([this](int w, int h) {
        cam.projection = glm::perspective(glm::radians(90.0f), window::get_aspect_ratio(), 0.1f, 100.f);
    });

    auto vs = utils::file_to_string("data/common_vs.glsl");
    auto fs = utils::file_to_string("data/common_fs.glsl");
    simple_shader = new Shader(vs, fs);

    cube_model = new Model(R"(E:\Thomas\Blender\training\custom_format\prototype_cf.model)", "data/cube.png");
}

Game::~Game() {
    delete simple_shader;
    delete cube_model;
    physx.destroyPhysicsWorld(world);
}

void Game::update(float delta) {
    world->update(delta);
    debug_renderer->reset();
    debug_renderer->computeDebugRenderingPrimitives(*world);

    constexpr float speed = 2.0;

    if (window::is_key_pressed(GLFW_KEY_W)) {
        cam.position.x += std::cos(glm::radians(cam.yaw - 90.0f)) * delta * speed;
        cam.position.z += std::sin(glm::radians(cam.yaw - 90.0f)) * delta * speed;
    }
    if (window::is_key_pressed(GLFW_KEY_S)) {
        cam.position.x -= std::cos(glm::radians(cam.yaw - 90.0f)) * delta * speed;
        cam.position.z -= std::sin(glm::radians(cam.yaw - 90.0f)) * delta * speed;
    }

    if (window::is_key_pressed(GLFW_KEY_A)) {
        cam.position.x -= std::cos(glm::radians(cam.yaw)) * delta * speed;
        cam.position.z -= std::sin(glm::radians(cam.yaw)) * delta * speed;
    }
    if (window::is_key_pressed(GLFW_KEY_D)) {
        cam.position.x += std::cos(glm::radians(cam.yaw)) * delta * speed;
        cam.position.z += std::sin(glm::radians(cam.yaw)) * delta * speed;
    }

    if (window::is_key_pressed(GLFW_KEY_R)) {
        cam.position.y += delta * speed;
    }
    if (window::is_key_pressed(GLFW_KEY_F)) {
        cam.position.y -= delta * speed;
    }

    if (window::is_key_pressed(GLFW_KEY_LEFT)) {
        cam.yaw -= delta * 60.0f;
    }
    if (window::is_key_pressed(GLFW_KEY_RIGHT)) {
        cam.yaw += delta * 60.0f;
    }

    if (window::is_key_pressed(GLFW_KEY_UP)) {
        cam.pitch += delta * 60.0f;
    }
    if (window::is_key_pressed(GLFW_KEY_DOWN)) {
        cam.pitch -= delta * 60.0f;
    }
}

void Game::render() {
    simple_shader->use();
    simple_shader->set_uniform("u_proj", cam.get_proj_mat());
    simple_shader->set_uniform("u_view", cam.get_view_mat());
    simple_shader->set_uniform("u_model", glm::mat4(1.0f));

    cube_model->draw();

    debug::set_cam(cam);
    debug::render_physics(debug_renderer);
    debug::set_color(0.1f, 1.0f, 0.1f);
    debug::point({0.0, 0.0});
}
