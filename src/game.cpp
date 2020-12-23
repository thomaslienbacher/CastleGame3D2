//
// Created by Thomas Lienbacher on 21.12.2020.
//

#include "game.hpp"
#include "debug.hpp"
#include "window.hpp"
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

    cam.position = glm::vec3(0, 0, 0);
    window::add_on_resize([this](int w, int h) {
        cam.projection = glm::perspective(glm::radians(90.0f), window::get_aspect_ratio(), 0.1f, 100.f);
    });
}

Game::~Game() {
    physx.destroyPhysicsWorld(world);
}

void Game::update(float delta) {
    world->update(delta);
    debug_renderer->reset();
    debug_renderer->computeDebugRenderingPrimitives(*world);
}

void Game::render() {
    debug::set_cam(cam);
    debug::render_physics(debug_renderer);

    debug::set_color(1.0f, 0.3, 0.0f);
    debug::point({0.0, 0.0});
}
