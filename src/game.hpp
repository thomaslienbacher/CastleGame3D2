//
// Created by Thomas Lienbacher on 21.12.2020.
//

#ifndef CASTLEGAME3D2_GAME_HPP
#define CASTLEGAME3D2_GAME_HPP

#include "glalheaders.hpp"
#include <reactphysics3d/reactphysics3d.h>
#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "geometry.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "level.hpp"
#include "player.hpp"
#include "rune.hpp"
#include "door.hpp"
#include "sound.hpp"
#include "text.hpp"
#include "texttrigger.hpp"

class TextTrigger;

class Game {
    rp3d::DebugRenderer *debug_renderer;
    Shader *simple_shader;
    Shader *overlaying_shader;
    Shader *font_shader;
    Player *player;
    Level *level;
    Texture *overlay;
    std::vector<Rune *> runes;
    std::vector<Door *> doors;
    std::vector<TextTrigger*> text_triggers;
    TextTrigger* text_trigger_active_render;
    Sound sound;
    SoundSource source;
    Texture *font_texture;
    Font *font;
    float time;

public:
    rp3d::PhysicsCommon physx;
    rp3d::PhysicsWorld *world;

    Game();

    ~Game();

    void update(float delta);

    void render();

    void set_active_text_trigger(TextTrigger* text_trigger);
};

#endif //CASTLEGAME3D2_GAME_HPP
