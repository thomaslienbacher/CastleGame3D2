//
// Created by Thomas Lienbacher on 17.01.2021.
//

#ifndef CASTLEGAME3D2_TEXTTRIGGER_HPP
#define CASTLEGAME3D2_TEXTTRIGGER_HPP

#include "level.hpp"
#include "text.hpp"
#include "game.hpp"

class Game;

class TextTrigger {
    rp3d::PhysicsCommon *physx;
    rp3d::PhysicsWorld *world;
    rp3d::BoxShape *shape;
    rp3d::CollisionBody *body;
    Text *text;
    bool triggered;
    float show_time;

public:
    TextTrigger(std::string text, Font *font, glm::vec3 position, glm::vec3 dimensions, rp3d::PhysicsCommon *physx,
                rp3d::PhysicsWorld *world);

    ~TextTrigger();

    void update(float delta, Game* game);

    void render(Shader *font_shader);
};


#endif //CASTLEGAME3D2_TEXTTRIGGER_HPP
