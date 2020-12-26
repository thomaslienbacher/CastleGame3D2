//
// Created by Thomas Lienbacher on 26.12.2020.
//

#ifndef CASTLEGAME3D2_PLAYER_HPP
#define CASTLEGAME3D2_PLAYER_HPP

#include "master.hpp"
#include "camera.hpp"
#include <glm/glm.hpp>
#include <reactphysics3d/reactphysics3d.h>

class Player {
    static constexpr float HEIGHT = 2.0f;
    static constexpr float RADIUS = 0.3f;
    static constexpr float EYE_HEIGHT = 1.75f;
    static constexpr float MAX_SPEED = 9.0f;

    Camera cam;
    glm::vec3 position;
    rp3d::RigidBody *body;
    rp3d::CapsuleShape *shape;
    rp3d::PhysicsCommon *physx;
    rp3d::PhysicsWorld *world;
    double last_mousex, last_mousey;

public:
    Player(glm::vec3 spawnpoint, rp3d::PhysicsCommon *physx, rp3d::PhysicsWorld *world);

    ~Player();

    void update(float delta);

    Camera& camera();

    glm::mat4 get_view_mat();
};


#endif //CASTLEGAME3D2_PLAYER_HPP
