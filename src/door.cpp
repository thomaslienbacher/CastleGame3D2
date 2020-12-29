//
// Created by Thomas Lienbacher on 29.12.2020.
//

#include <glm/gtc/matrix_transform.hpp>
#include "door.hpp"
#include "collisionhandler.hpp"

Door::Door(std::string model, std::string texture, glm::vec3 position, float yrot, glm::vec3 dimensions,
           rp3d::PhysicsCommon *physx, rp3d::PhysicsWorld *world, Rune *trigger) :
        position(position), yrot(yrot), opening(false), open_distance(0.f), max_open_distance(dimensions.y + 0.1f),
        physx(physx), world(world), trigger(trigger) {

    geometry = new GeometryFormat(model);
    mesh = new Mesh(geometry);
    diffuse = new Texture(texture);

    rp3d::Transform transform({position.x, position.y, position.z}, rp3d::Quaternion::fromEulerAngles(0.f, glm::radians(yrot), 0.f));
    body = world->createRigidBody(transform);
    body->setType(rp3d::BodyType::KINEMATIC);
    body->setUserData(BodyIdentifier::DOOR.as_ptr());
    auto e = dimensions * 0.5f;
    shape = physx->createBoxShape({e.x, e.y, e.z});
    auto c = body->addCollider(shape, rp3d::Transform({0.f, 0, 0.f}, rp3d::Quaternion::identity()));
    c->getMaterial().setBounciness(0.f);
}

Door::~Door() {
    world->destroyRigidBody(body);
    physx->destroyBoxShape(shape);
    delete diffuse;
    delete mesh;
    delete geometry;
}

void Door::update(float delta) {
    if (open_distance > max_open_distance) return;
    if (trigger->is_collected()) {
        opening = true;
        open_distance += delta * 0.8f;
        body->setTransform(rp3d::Transform({position.x, position.y - open_distance, position.z},
                                           rp3d::Quaternion::fromEulerAngles(0.f, glm::radians(yrot), 0.f)));
    }
}

void Door::render(Shader *simple_shader) {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, {position.x, position.y - open_distance, position.z});
    model = glm::rotate(model, glm::radians(yrot), glm::vec3(0, 1, 0));
    simple_shader->set_uniform("u_model", model);
    mesh->bind();
    diffuse->bind();
    glDrawElements(GL_TRIANGLES, mesh->get_num_elements(), GL_UNSIGNED_INT, nullptr);
}
