//
// Created by Thomas Lienbacher on 21.12.2020.
//

#include "game.hpp"
#include "debug.hpp"
#include "window.hpp"
#include "utils.hpp"

Game::Game() : physx() {
    auto settings = rp3d::PhysicsWorld::WorldSettings();
    settings.gravity = rp3d::Vector3(0.f, -10.0f, 0.f);
    settings.worldName = "main_world";
    settings.defaultPositionSolverNbIterations = 15;
    settings.defaultVelocitySolverNbIterations = 15;
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

    auto vs = utils::file_to_string("data/common_vs.glsl");
    auto fs = utils::file_to_string("data/common_fs.glsl");
    simple_shader = new Shader(vs, fs);


    level = new Level(R"(E:\Thomas\Blender\training\level_format\basic_level.level)",
                      R"(E:\Thomas\Blender\training\level_format\level_textured_test.png)",
                      &physx, world);
    player = new Player(level->get_spawnpoint(), &physx, world);
}

Game::~Game() {
    delete simple_shader;
    delete level;
    delete player;
    physx.destroyPhysicsWorld(world);
}

void Game::update(float delta) {
    world->update(1.0f / 60.0f);
    debug_renderer->reset();
    debug_renderer->computeDebugRenderingPrimitives(*world);
    player->update(delta);
}

void Game::render() {
    debug::set_cam(player->camera());

    simple_shader->use();
    simple_shader->set_uniform("u_proj", player->camera().get_proj_mat());
    simple_shader->set_uniform("u_view", player->get_view_mat());
    simple_shader->set_uniform("u_model", glm::mat4(1.0f));

    level->draw();

    //debug::render_physics(debug_renderer);
    debug::set_color(0.7f, 0.7f, 0.7f);
    glPointSize(3.0f);
    debug::point({0.0, 0.0});
    glPointSize(2.0f);
}
