//
// Created by Thomas Lienbacher on 28.12.2020.
//

#include "collisionhandler.hpp"
#include <utility>

const BodyIdentifier BodyIdentifier::LEVEL = {1};
const BodyIdentifier BodyIdentifier::PLAYER = {2};
const BodyIdentifier BodyIdentifier::RUNE = {3};
const BodyIdentifier BodyIdentifier::DOOR = {4};

void CollisionHandler::onContact(const reactphysics3d::CollisionCallback::CallbackData &data) {
    for (int i = 0; i < data.getNbContactPairs(); ++i) {
        auto p = data.getContactPair(i);
        BodyIdentifier b1 = p.getBody1()->getUserData();
        BodyIdentifier b2 = p.getBody2()->getUserData();

        assert(b1.id != 0);
        assert(b2.id != 0);//TODO: put in DEBUG compilation clause

        if (b1 == me) {
            callback(b2, p.getEventType());
        } else {
            callback(b1, p.getEventType());
        }
    }
}

CollisionHandler::CollisionHandler(const BodyIdentifier &me,
                                   std::function<void(BodyIdentifier other, CollisionType type)> callback)
        : me(me), callback(std::move(callback)) {}
