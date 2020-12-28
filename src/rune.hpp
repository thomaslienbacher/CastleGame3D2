//
// Created by Thomas Lienbacher on 28.12.2020.
//

#ifndef CASTLEGAME3D2_RUNE_HPP
#define CASTLEGAME3D2_RUNE_HPP

#include "master.hpp"
#include <glm/glm.hpp>
#include "geometry.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include <reactphysics3d/reactphysics3d.h>
#include "shader.hpp"

class Rune {
    glm::vec3 position;
    float time;
    bool collected;

    Texture *overlay;
    Texture *diffuse;
    GeometryFormat *geometry;
    Mesh *mesh;
    rp3d::PhysicsCommon *physx;
    rp3d::PhysicsWorld *world;
    rp3d::SphereShape *shape;
    rp3d::CollisionBody *body;

public:
    float yrot;
    Rune(std::string model, std::string diffuse_texture, Texture *overlay, glm::vec3 position, float yrot,
         rp3d::PhysicsCommon *physx, rp3d::PhysicsWorld *world);

    ~Rune();

    void update(float delta);

    void render(Shader *overlaying_shader);
};


#endif //CASTLEGAME3D2_RUNE_HPP
