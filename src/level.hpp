//
// Created by Thomas Lienbacher on 25.12.2020.
//

#ifndef CASTLEGAME3D2_LEVEL_HPP
#define CASTLEGAME3D2_LEVEL_HPP

#include "master.hpp"
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>
#include "texture.hpp"
#include "mesh.hpp"

const uint32_t LEVEL_MAGIC = 0x4c56454c;
const uint32_t LEVEL_VERSION = 0x1;

struct LevelFormatHeader {
    uint32_t magic = 0;
    uint32_t version = 0;
    uint32_t flags = 0;
    char identifier[32] = {0};
    float spawnpoint[3] = {0.f, 0.f, 0.f};
    uint32_t num_meshes = 0;
    uint32_t num_objects = 0;
} __attribute__ ((packed));

struct LevelFormatMeshHeader {
    uint32_t blob_size = 0;
    uint32_t num_indices = 0;
    uint32_t num_vertices = 0;
} __attribute__ ((packed));

struct LevelFormatMeshBlob {
    uint32_t *indices = nullptr;
    float *vertices = nullptr;
    float *texcoords = nullptr;
    float *normals = nullptr;
};

struct LevelFormatObject {
    uint32_t id = 0;
    uint32_t type = 0;
    char identifier[32] = {0};
    float position[3] = {0.f, 0.f, 0.f};
    union {
        char _phantom[24] = {0};
    } custom_data;
} __attribute__ ((packed));

class LevelFormat {
public:
    LevelFormatHeader header;
    std::vector<LevelFormatMeshHeader> mesh_headers;
    std::vector<LevelFormatMeshBlob> mesh_blobs;
    std::vector<LevelFormatObject> objects;

    explicit LevelFormat(std::string levelfile);

    ~LevelFormat();
};

class Level {
    LevelFormat format;
    Texture *texture;
    std::vector<Mesh *> meshes;
    std::vector<rp3d::RigidBody *> bodies;

public:
    explicit Level(std::string levelfile, std::string texture, rp3d::PhysicsCommon *physx, rp3d::PhysicsWorld *world);

    ~Level();

    void draw();

};


#endif //CASTLEGAME3D2_LEVEL_HPP
