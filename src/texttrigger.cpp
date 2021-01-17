//
// Created by Thomas Lienbacher on 17.01.2021.
//

#include <glm/ext.hpp>
#include "texttrigger.hpp"
#include "collisionhandler.hpp"
#include "window.hpp"

TextTrigger::TextTrigger(std::string text, Font *font, glm::vec3 position, glm::vec3 dimensions,
                         rp3d::PhysicsCommon *physx,
                         rp3d::PhysicsWorld *world) : triggered(false), show_time(0.f), physx(physx), world(world) {
    this->text = new Text(text, font);
    rp3d::Transform transform({position.x, position.y, position.z}, rp3d::Quaternion::identity());
    body = world->createCollisionBody(transform);
    body->setUserData(BodyIdentifier::TEXTTRIGGER.as_ptr());
    auto e = dimensions * 0.5f;
    shape = physx->createBoxShape({e.x, e.y, e.z});
    body->addCollider(shape, rp3d::Transform({0.f, 0, 0.f}, rp3d::Quaternion::identity()));
}

TextTrigger::~TextTrigger() {
    world->destroyCollisionBody(body);
    physx->destroyBoxShape(shape);
    delete text;
}

void TextTrigger::update(float delta, Game *game) {
    auto c = CollisionHandler(BodyIdentifier::RUNE, [this, game](BodyIdentifier other, CollisionType type) {
        if (!triggered && other == BodyIdentifier::PLAYER) {
            triggered |= true;
            game->set_active_text_trigger(this);
        }
    });
    world->testCollision(body, (rp3d::CollisionCallback &) c);

    if (triggered) show_time += delta;
}

void TextTrigger::render(Shader *font_shader) {
    if (triggered && show_time < 6.0f) {
        glDisable(GL_DEPTH_TEST);
        auto window_size = window::size();

        auto ortho = glm::ortho(0.f, window_size.x, 0.f, window_size.y, -1.f, 1.f);
        font_shader->set_uniform("u_proj", ortho);
        font_shader->set_uniform("u_alpha", glm::clamp((show_time - 5.0f) * -1.f, 0.f, 1.f));
        float target_size = 0.05f; // relative to window height
        float scale = target_size / (text->get_height() / window_size.y);
        auto translate = glm::translate(glm::mat4(1.0f),
                                        {-text->get_width() / 2.f * scale + window_size.x / 2.0f, 10.f, 0.f});
        font_shader->set_uniform("u_model", glm::scale(translate, glm::vec3(scale)));
        text->render(font_shader);
        glEnable(GL_DEPTH_TEST);
    }
}
