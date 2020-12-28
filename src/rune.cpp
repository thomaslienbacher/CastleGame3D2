//
// Created by Thomas Lienbacher on 28.12.2020.
//

#include <glm/gtc/matrix_transform.hpp>
#include "rune.hpp"
#include "collisionhandler.hpp"

Rune::Rune(std::string model, std::string diffuse_texture, Texture *overlay, glm::vec3 position, float yrot,
           rp3d::PhysicsCommon *physx, rp3d::PhysicsWorld *world) :
        position(position), yrot(yrot), time(0.f), collected(false), overlay(overlay), physx(physx), world(world) {
    geometry = new GeometryFormat(model);
    mesh = new Mesh(geometry);
    diffuse = new Texture(diffuse_texture);

    rp3d::Transform transform({position.x, position.y, position.z}, rp3d::Quaternion::identity());
    body = world->createCollisionBody(transform);
    body->setUserData(BodyIdentifier::RUNE.as_ptr());
    shape = physx->createSphereShape(0.5);
    body->addCollider(shape, rp3d::Transform({0.f, 0.5f, 0.f}, rp3d::Quaternion::identity()));
}

Rune::~Rune() {
    world->destroyCollisionBody(body);
    physx->destroySphereShape(shape);
    delete diffuse;
    delete mesh;
    delete geometry;
}

void Rune::update(float delta) {
    time += delta;
    collected = false;
    auto c = CollisionHandler(BodyIdentifier::RUNE, [this](BodyIdentifier other, CollisionType type) {
        collected = other == BodyIdentifier::PLAYER;
    });
    world->testCollision(body, (rp3d::CollisionCallback &) c);
}

void Rune::render(Shader *overlaying_shader) {
    if (collected) return;

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, {position.x, position.y + std::sin(time * 1.8f) * 0.1f, position.z});
    model = glm::rotate(model, glm::radians(yrot), glm::vec3(0, 1, 0));
    overlaying_shader->set_uniform("u_model", model);
    overlaying_shader->set_uniform("u_offset", glm::vec2(time * 0.3f));
    mesh->bind();
    diffuse->bind_unit(0);
    overlay->bind_unit(1);
    glDrawElements(GL_TRIANGLES, mesh->get_num_elements(), GL_UNSIGNED_INT, nullptr);
}
