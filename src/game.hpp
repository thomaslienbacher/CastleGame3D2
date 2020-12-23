//
// Created by Thomas Lienbacher on 21.12.2020.
//

#ifndef CASTLEGAME3D2_GAME_HPP
#define CASTLEGAME3D2_GAME_HPP

#include "master.hpp"
#include <reactphysics3d/reactphysics3d.h>
#include "camera.hpp"
#include "shader.hpp"

class Game {
    rp3d::DebugRenderer *debug_renderer;
    Camera cam;

public:
    rp3d::PhysicsCommon physx;
    rp3d::PhysicsWorld *world;


    Game();

    ~Game();

    void update(float delta);

    void render();
};

#endif //CASTLEGAME3D2_GAME_HPP
