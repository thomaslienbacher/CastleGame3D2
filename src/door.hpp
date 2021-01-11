//
// Created by Thomas Lienbacher on 29.12.2020.
//

#ifndef CASTLEGAME3D2_DOOR_HPP
#define CASTLEGAME3D2_DOOR_HPP

#include "glalheaders.hpp"
#include "geometry.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>
#include "rune.hpp"
#include "player.hpp"

class Door {
    glm::vec3 position;
    float yrot;
    bool opening;
    float open_distance;
    float max_open_distance;

    Texture *diffuse;
    GeometryFormat *geometry;
    Mesh *mesh;
    rp3d::PhysicsCommon *physx;
    rp3d::PhysicsWorld *world;
    rp3d::BoxShape *shape;
    rp3d::RigidBody *body;

    Rune *trigger;

public:
    Door(std::string model, std::string texture, glm::vec3 position, float yrot, glm::vec3 dimensions,
         rp3d::PhysicsCommon *physx, rp3d::PhysicsWorld *world, Rune *trigger);

    ~Door();

    void update(float delta, Player *player);

    void render(Shader *simple_shader);
};


#endif //CASTLEGAME3D2_DOOR_HPP
