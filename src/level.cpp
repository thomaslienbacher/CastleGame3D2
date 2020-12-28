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
        std::cerr << "File version is incorrect" << panic;
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
        : format(levelfile), physx(physx), world(world) {
    this->texture = new Texture(texture);

    for (int i = 0; i < format.header.num_meshes; ++i) {
        auto h = format.mesh_headers[i];
        auto b = format.mesh_blobs[i];

        meshes.push_back(new Mesh(h.num_indices, b.indices, h.num_vertices, b.vertices, b.texcoords, b.normals));
    }

    for (int i = 0; i < format.header.num_meshes; i++) {
        auto body = world->createRigidBody(rp3d::Transform::identity());
        body->setType(rp3d::BodyType::STATIC);
        bodies.push_back(body);
        auto tri_mesh = physx->createTriangleMesh();
        tri_meshes.push_back(tri_mesh);
        auto h = format.mesh_headers[i];
        auto b = format.mesh_blobs[i];
        auto trivertexarray = new rp3d::TriangleVertexArray(h.num_vertices,
                                                            b.vertices,
                                                            3 * sizeof(float),
                                                            h.num_indices / 3,
                                                            b.indices,
                                                            3 * sizeof(unsigned int),
                                                            rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
                                                            rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);
        tvas.push_back(trivertexarray);
        tri_mesh->addSubpart(trivertexarray);
        auto shape = physx->createConcaveMeshShape(tri_mesh);
        shapes.push_back(shape);
        auto c = body->addCollider(shape, rp3d::Transform::identity());
        c->getMaterial().setBounciness(0.f);
    }

}

Level::~Level() {
    delete texture;

    for (auto m : meshes) {
        delete m;
    }

    for (auto b : bodies) {
        world->destroyRigidBody(b);
    }

    for (auto s : shapes) {
        physx->destroyConcaveMeshShape(s);
    }

    for (auto t : tri_meshes) {
        physx->destroyTriangleMesh(t);
    }

    for (auto t : tvas) {
        delete t;
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
    //debug::sphere(glm::vec3(f[0], f[1], f[2]), 0.3f);
}

glm::vec3 Level::get_spawnpoint() {
    return glm::vec3(format.header.spawnpoint[0], format.header.spawnpoint[1], format.header.spawnpoint[2]);
}
