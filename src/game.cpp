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


    level = new Level(R"(E:\Thomas\Blender\training\level_format\basic_level_doors.level)",
                      R"(E:\Thomas\Blender\training\level_format\level_textured_test.png)",
                      &physx, world);
    player = new Player(level->get_spawnpoint(), &physx, world);

    overlay = new Texture("data/highlight.png");

    for (auto &o : level->get_objects()) {
        if (o.type == LevelFormatObject::Type::Rune) {
            std::string mod;
            std::string tex;

            switch (o.custom_data.rune.kind) {
                case LevelFormatObject::CustomData::RuneObject::Kind::A:
                    mod = R"(E:\Thomas\CLion-Workspace\CastleGame3D2\Assets\runes\rune_a\rune_a.model)";
                    tex = "data/rune_a.png";
                    break;
                default:
                case LevelFormatObject::CustomData::RuneObject::Kind::B:
                    mod = R"(E:\Thomas\CLion-Workspace\CastleGame3D2\Assets\runes\rune_b\rune_b.model)";
                    tex = "data/rune_b.png";
                    break;
            }

            auto r = new Rune(mod, tex, overlay, {o.position[0], o.position[1], o.position[2]}, o.custom_data.rune.yrot,
                              o.custom_data.rune.kind, &physx, world);
            runes.emplace_back(r);
        }

        if (o.type == LevelFormatObject::Type::Door) {
            auto dd = o.custom_data.door;
            auto rune = *std::find_if(runes.begin(), runes.end(), [&](Rune *r) {
                return dd.trigger == r->get_kind();
            });

            Door *d;
            if (dd.trigger == LevelFormatObject::CustomData::RuneObject::A) {
                d = new Door(R"(E:\Thomas\Blender\training\level_format\basic_level_doors_door_a.model)",
                             R"(E:\Thomas\Blender\training\level_format\door_a.png)",
                             {o.position[0], o.position[1], o.position[2]},
                             dd.yrot,
                             {dd.dimensions[0], dd.dimensions[1], dd.dimensions[2]},
                             &physx, world, rune);
            } else if (dd.trigger == LevelFormatObject::CustomData::RuneObject::B) {

                d = new Door(R"(E:\Thomas\Blender\training\level_format\basic_level_doors_door_b.model)",
                             R"(E:\Thomas\Blender\training\level_format\door_b.png)",
                             {o.position[0], o.position[1], o.position[2]},
                             dd.yrot,
                             {dd.dimensions[0], dd.dimensions[1], dd.dimensions[2]},
                             &physx, world, rune);
            }
            doors.emplace_back(d);
        }
    }
}

Game::~Game() {
    for (auto r : runes) {
        delete r;
    }
    for (auto d : doors) {
        delete d;
    }
    delete overlay;
    delete overlaying_shader;
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
    for (auto r : runes) {
        r->update(delta);
    }
    for (auto d : doors) {
        d->update(delta);
    }
}

void Game::render() {
    debug::set_cam(player->camera());

    simple_shader->set_uniform("u_proj", player->camera().get_proj_mat());
    simple_shader->set_uniform("u_view", player->get_view_mat());
    level->render(simple_shader);

    for (auto d : doors) {
        d->render(simple_shader);
    }

    overlaying_shader->set_uniform("u_proj", player->camera().get_proj_mat());
    overlaying_shader->set_uniform("u_view", player->get_view_mat());
    for (auto r : runes) {
        r->render(overlaying_shader);
    }

    static bool debug = false;

    if (window::is_key_pressed(GLFW_KEY_F1)) {
        debug = true;
    }
    if (window::is_key_pressed(GLFW_KEY_F2)) {
        debug = false;
    }

    if (debug) {
        debug::render_physics(debug_renderer);
        debug::set_color(0.7f, 0.7f, 0.7f);
        glPointSize(3.5f);
        debug::point({0.0, 0.0});
        glPointSize(2.0f);
    }
}
