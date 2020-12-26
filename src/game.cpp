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

    cam.position = glm::vec3(0, 1.75f, 0.0f);
    window::add_on_resize([this](int w, int h) {
        cam.projection = glm::perspective(glm::radians(90.0f), window::get_aspect_ratio(), 0.1f, 100.f);
    });

    auto vs = utils::file_to_string("data/common_vs.glsl");
    auto fs = utils::file_to_string("data/common_fs.glsl");
    simple_shader = new Shader(vs, fs);

    level = new Level(R"(E:\Thomas\Blender\training\level_format\basic_level.level)",
                      R"(E:\Thomas\Blender\training\level_format\level_full.png)",
                      &physx, world);
}

Game::~Game() {
    delete simple_shader;
    delete level;
    physx.destroyPhysicsWorld(world);
}

void Game::update(float delta) {
    world->update(delta);
    debug_renderer->reset();
    debug_renderer->computeDebugRenderingPrimitives(*world);

    constexpr float speed = 6.0;

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
        cam.yaw -= delta * 70.0f;
    }
    if (window::is_key_pressed(GLFW_KEY_RIGHT)) {
        cam.yaw += delta * 70.0f;
    }

    if (window::is_key_pressed(GLFW_KEY_UP)) {
        cam.pitch += delta * 70.0f;
    }
    if (window::is_key_pressed(GLFW_KEY_DOWN)) {
        cam.pitch -= delta * 70.0f;
    }
}

void Game::render() {
    debug::set_cam(cam);

    simple_shader->use();
    simple_shader->set_uniform("u_proj", cam.get_proj_mat());
    simple_shader->set_uniform("u_view", cam.get_view_mat());
    simple_shader->set_uniform("u_model", glm::mat4(1.0f));

    level->draw();

    debug::render_physics(debug_renderer);
    debug::set_color(0.1f, 1.0f, 0.1f);
    debug::point({0.0, 0.0});

    /*debug::set_color(1.0f, 1.0f, 0.1f);
    for (auto o : level_format->objects) {
        glPointSize(5.0f);
        debug::point({o.position[0], o.position[1], o.position[2]});
        glPointSize(2.0f);
    }

    debug::set_color(0.1f, 0.4f, 1.0f);
    for (int i = 0; i < level_format->header.num_meshes; i++) {
        auto h = level_format->mesh_headers[i];
        auto b = level_format->mesh_blobs[i];

        for (int j = 0; j < h.num_indices; j += 3) {
            int ta = b.indices[j];
            int tb = b.indices[j + 1];
            int tc = b.indices[j + 2];

            auto a = glm::vec3(b.vertices[ta * 3], b.vertices[ta * 3 + 1], b.vertices[ta * 3 + 2]);
            auto b2 = glm::vec3(b.vertices[tb * 3], b.vertices[tb * 3 + 1], b.vertices[tb * 3 + 2]);
            auto c = glm::vec3(b.vertices[tc * 3], b.vertices[tc * 3 + 1], b.vertices[tc * 3 + 2]);
            debug::triangle(a, b2, c);
        }
    }*/
}
