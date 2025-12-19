#pragma once

#include "objects.hpp"

bool CirclevsCircle(Manifold& m) 
{
    Object& A = *m.A;
    Object& B = *m.B;

    float r = A.circle.radius + B.circle.radius;

    Vec2f Apos = A.position;
    Vec2f Bpos = B.position;
    Vec2f n = Bpos - Apos;
    // Distance between centers
    float dist = Apos.distance(Bpos);

    if (dist * dist >= r * r)
        return false;

    // If distance is less than the sum of radii, collide
    if (dist != 0.0f) {
        m.penetration = r - dist;
        m.normal = n / dist;
    } else {
        // Perfect Overlap
        m.penetration = A.circle.radius;
        m.normal = {1.0f, 0.0f};
    }

    return true;
}

/*
bool CirclevsRect(Manifold& m)
{

}

bool RectvsRect(Manifold& m)
{

}
*/

void ApplyImpulse(Manifold& m)
{
    Object& A = *m.A;
    Object& B = *m.B;

    Vec2f rv = B.velocity - A.velocity;
    float velNormal = rv.dot(m.normal);

    if (velNormal > 0)
        return;

    float e = std::min(A.restitution, B.restitution);
    float j = -(1 + e) * velNormal;
    j /= A.invMass + B.invMass;

    Vec2f impulse = j * m.normal;
    A.velocity -= impulse * A.invMass;
    B.velocity += impulse * B.invMass;
}

void PositionCorrection(Manifold& m)
{
    const float percent = 0.8f;
    const float slop = 0.01f;

    Vec2f correction = std::max(m.penetration - slop, 0.0f) / (m.A->invMass + m.B->invMass) * percent * m.normal;

    m.A->position -= correction * m.A->invMass;
    m.B->position += correction * m.B->invMass;
}

using CollisionFn = bool(*)(Manifold&);

CollisionFn collisionTable[2][2] =
{
    {CirclevsCircle, nullptr},
    {nullptr, nullptr}
};

void MTMWrldBounds(Object& obj)
{
    if (obj.shape == ShapeType::Circle) {
        float r = obj.circle.radius;
        // Left border
        if (obj.position.x - r < -1.0f) {
            obj.position.x = -1.0f + r;
            obj.velocity.x *= -obj.restitution;
        }
        // Right border
        if (obj.position.x + r > 1.0f) {
            obj.position.x = 1.0f - r;
            obj.velocity.x *= -obj.restitution;
        }
        // Top border
        if (obj.position.y + r > 1.0f) {
            obj.position.y = 1.0f - r;
            obj.velocity.y *= -obj.restitution;
        }
         // Bottom border
        if (obj.position.y - r < -1.0f) {
            obj.position.y = -1.0f + r;
            obj.velocity.y *= -obj.restitution;
        }
    }
    if (obj.shape == ShapeType::Rect) {
        float halfWidth = obj.rect.halfDim.x;
        float halfHeight = obj.rect.halfDim.y;
        // Left border
        if (obj.position.x - halfWidth < -1.0f) {
            obj.position.x = -1.0f + halfWidth;
            obj.velocity.x *= -obj.restitution;
        }
        // Right border
        if (obj.position.x + halfWidth > 1.0f) {
            obj.position.x = 1.0f - halfWidth;
            obj.velocity.x *= -obj.restitution;
        }
        // Top border
        if (obj.position.y + halfHeight > 1.0f) {
            obj.position.y = 1.0f - halfHeight;
            obj.velocity.y *= -obj.restitution;
        }
         // Bottom border
        if (obj.position.y - halfHeight < -1.0f) {
            obj.position.y = -1.0f + halfHeight;
            obj.velocity.y *= -obj.restitution;
        }
    }
}