#pragma once

#include "objects.hpp"
#include "collision.hpp"
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

            obj.force += gravity / obj.invMass;  // Apply a gravitational force to the obj

            Vec2f accel = obj.force * obj.invMass;
            obj.velocity += accel * dt;
            obj.position += obj.velocity * dt;

            obj.force = Vec2f(0.0f, 0.0f);     // Reset net force at the end
        }

        const int solverIters = 20;
        for (int k = 0; k < solverIters; ++k)
        {
            for (size_t i = 0; i < objs.size(); ++i) {
                for (size_t j = i + 1; j < objs.size(); ++j) {
                    Object& A = objs[i];
                    Object& B = objs[j];

                    Manifold m{&A, &B};

                    CollisionFn fn = collisionTable[(int)A.shape][(int)B.shape];
                    if (!fn || !fn(m))
                        continue;

                    ApplyImpulse(m);
                    PositionCorrection(m);
                }
            }
        }

        for (Object& obj : objs)
            MTMWrldBounds(obj);
    }

    const std::vector<Object>& Objects() const { return objs; }
};