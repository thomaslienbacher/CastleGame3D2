//
// Created by Thomas Lienbacher on 21.12.2020.
//

#include <glm/gtc/matrix_transform.hpp>
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

    vs = utils::file_to_string("data/overlay_vs.glsl");
    fs = utils::file_to_string("data/overlay_fs.glsl");
    overlaying_shader = new Shader(vs, fs);


    level = new Level(R"(E:\Thomas\Blender\training\level_format\basic_level.level)",
                      R"(E:\Thomas\Blender\training\level_format\level_textured_test.png)",
                      &physx, world);
    player = new Player(level->get_spawnpoint(), &physx, world);

    diffuse_rune = new Texture("data/rune_a.png");
    overlay = new Texture("data/highlight.png");
    auto s = std::string(R"(E:\Thomas\CLion-Workspace\CastleGame3D2\Assets\runes\rune_a\rune_a.model)");
    rune_geo = new GeometryFormat(s);
    rune_mesh = new Mesh(rune_geo);
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

    overlaying_shader->use();
    overlaying_shader->set_uniform("u_proj", player->camera().get_proj_mat());
    overlaying_shader->set_uniform("u_view", player->get_view_mat());

    static float time = 0;
    time += 1.0f / 60.f;
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, {0.f, 0.5f + std::sin(time * 1.8f) * 0.1f, -5.f});
    //model  = glm::rotate(model, glm::radians(time * 90.f), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(1.0f));
    overlaying_shader->set_uniform("u_model", model);
    overlaying_shader->set_uniform("u_offset", glm::vec2(time * 0.3f));
    rune_mesh->bind();
    diffuse_rune->bind_unit(0);
    overlay->bind_unit(1);
    glDrawElements(GL_TRIANGLES, rune_mesh->get_num_elements(), GL_UNSIGNED_INT, nullptr);

    //debug::render_physics(debug_renderer);
    debug::set_color(0.7f, 0.7f, 0.7f);
    glPointSize(3.0f);
    debug::point({0.0, 0.0});
    glPointSize(2.0f);
}
