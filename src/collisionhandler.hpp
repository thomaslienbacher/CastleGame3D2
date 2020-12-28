//
// Created by Thomas Lienbacher on 28.12.2020.
//

#ifndef CASTLEGAME3D2_COLLISIONHANDLER_HPP
#define CASTLEGAME3D2_COLLISIONHANDLER_HPP

#include "master.hpp"
#include <reactphysics3d/reactphysics3d.h>
#include <functional>

struct BodyIdentifier;

struct BodyIdentifier {
    static const BodyIdentifier LEVEL;
    static const BodyIdentifier PLAYER;
    static const BodyIdentifier RUNE;

    size_t id;

    BodyIdentifier(size_t id) : id(id) {}

    BodyIdentifier(void *ptr) {
        id = (size_t) ptr;
    }

    void *as_ptr() const {
        return (void *) id;
    }

    bool operator==(const BodyIdentifier &rhs) const {
        return id == rhs.id;
    }

    bool operator!=(const BodyIdentifier &rhs) const {
        return !(rhs == *this);
    }
};

typedef  rp3d::CollisionCallback::ContactPair::EventType CollisionType;

class CollisionHandler : public rp3d::CollisionCallback {

    BodyIdentifier me;
    std::function<void(BodyIdentifier other, CollisionType type)> callback;

public:
    CollisionHandler(const BodyIdentifier &me, std::function<void(BodyIdentifier other, CollisionType type)> callback);

    void onContact(const CallbackData &data) override;
};

#endif //CASTLEGAME3D2_COLLISIONHANDLER_HPP
