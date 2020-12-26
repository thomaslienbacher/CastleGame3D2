//
// Created by Thomas Lienbacher on 25.12.2020.
//

#include "level.hpp"
#include "utils.hpp"
#include "debug.hpp"

LevelFormat::LevelFormat(std::string levelfile) {
    std::ifstream in;
    in.open(levelfile, std::ifstream::in | std::ifstream::binary);

    if (!in.is_open()) {
        std::cerr << "Couln't open " << levelfile << panic;
    }

    in.read((char *) &this->header, sizeof(LevelFormatHeader));

    if (this->header.magic != LEVEL_MAGIC) {
        std::cerr << "Magic number is incorrect" << panic;
    }

    if (this->header.version != LEVEL_VERSION) {
        std::cerr << "File is out of date" << panic;
    }

    for (int i = 0; i < header.num_meshes; ++i) {
        mesh_headers.emplace_back(LevelFormatMeshHeader());
        in.read((char *) &mesh_headers.back(), sizeof(LevelFormatMeshHeader));

        mesh_blobs.emplace_back(LevelFormatMeshBlob());
        mesh_blobs.back().indices = new uint32_t[mesh_headers[i].num_indices];
        mesh_blobs.back().vertices = new float[mesh_headers[i].num_vertices * 3];
        mesh_blobs.back().texcoords = new float[mesh_headers[i].num_vertices * 2];
        mesh_blobs.back().normals = new float[mesh_headers[i].num_vertices * 3];

        in.read((char *) mesh_blobs.back().indices, mesh_headers[i].num_indices * sizeof(uint32_t));
        in.read((char *) mesh_blobs.back().vertices, mesh_headers[i].num_vertices * sizeof(float) * 3);
        in.read((char *) mesh_blobs.back().texcoords, mesh_headers[i].num_vertices * sizeof(float) * 2);
        in.read((char *) mesh_blobs.back().normals, mesh_headers[i].num_vertices * sizeof(float) * 3);
    }

    for (int i = 0; i < header.num_objects; ++i) {
        objects.emplace_back(LevelFormatObject());
        in.read((char *) &objects.back(), sizeof(LevelFormatObject));
    }

    in.close();
}

LevelFormat::~LevelFormat() {
    for (auto mb : mesh_blobs) {
        delete[] mb.indices;
        delete[] mb.vertices;
        delete[] mb.texcoords;
        delete[] mb.normals;
    }
}

Level::Level(std::string levelfile, std::string texture, rp3d::PhysicsCommon *physx, rp3d::PhysicsWorld *world)
        : format(levelfile) {
    this->texture = new Texture(texture);

    for (int i = 0; i < format.header.num_meshes; ++i) {
        auto h = format.mesh_headers[i];
        auto b = format.mesh_blobs[i];

        meshes.push_back(new Mesh(h.num_indices, b.indices, h.num_vertices, b.vertices, b.texcoords, b.normals));
    }
}

Level::~Level() {
    delete texture;
    for (auto m : meshes) {
        delete m;
    }
}

void Level::draw() {
    texture->bind();
    for (auto m : meshes) {
        m->bind();
        glDrawElements(GL_TRIANGLES, m->get_num_elements(), GL_UNSIGNED_INT, nullptr);
    }

    debug::set_color(1.0f, 1.0f, 1.0f);
    auto f = format.objects.begin()->position;
    debug::sphere(glm::vec3(f[0], f[1], f[2]), 0.3f);
}
