#pragma once

#include "objects.hpp"
#include <vector>

class Engine
{
private:
    std::vector<Object> objs;
    Vec2f gravity = Vec2(0.0f, -9.81f);

public:
    void MtmAdd(const Object& obj) { objs.push_back(obj); }
    //void MtmRem(const Object& obj);

    void step(float dt) {
        for (Object& obj : objs) {
            if (obj.invMass == 0.0f) continue;

            obj.force += obj.invMass * gravity;  // Apply a gravitational force to the obj

            obj.velocity += obj.force / obj.invMass * dt;
            obj.position += obj.velocity * dt;

            obj.force = Vec2f(0.0f, 0.0f);     // Reset net force at the end
        }
    }

    const std::vector<Object>& Objects() const { return objs; }
};