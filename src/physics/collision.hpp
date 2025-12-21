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


bool CirclevsRect(Manifold& m)
{
    Object& C = *m.A;
    Object& R = *m.B;

    Vec2f test = C.position;

    // Clamp to rectangle bounds (center + halfDims)
    if (C.position.x < R.position.x - R.rect.halfDim.x)
        test.x = R.position.x - R.rect.halfDim.x;
    else if (C.position.x > R.position.x + R.rect.halfDim.x)
        test.x = R.position.x + R.rect.halfDim.x;

    if (C.position.y < R.position.y - R.rect.halfDim.y)
        test.y = R.position.y - R.rect.halfDim.y;
    else if (C.position.y > R.position.y + R.rect.halfDim.y)
        test.y = R.position.y + R.rect.halfDim.y;

    // Vector from closest point → circle center
    Vec2f delta = C.position - test;
    float distSq = delta.dot(delta);
    float r = C.circle.radius;

    if (distSq > r * r)
        return false;

    // Handle inside-rectangle case
    if (distSq < 1e-6f) {
        Vec2f diff = C.position - R.position;
        Vec2f absDiff = {abs(diff.x), abs(diff.y)};

        float dx = R.rect.halfDim.x - absDiff.x;
        float dy = R.rect.halfDim.y - absDiff.y;

        if (dx < dy) {
            m.normal = (diff.x > 0) ? Vec2f(1, 0) : Vec2f(-1, 0);
            m.penetration = r + dx;
        } else {
            m.normal = (diff.y > 0) ? Vec2f(0, 1) : Vec2f(0, -1);
            m.penetration = r + dy;
        }
    }
    else {
        float dist = std::sqrt(distSq);
        m.normal = delta / dist;        // points rect → circle
        m.penetration = r - dist;
    }

    return true;
}

bool RectvsCircle(Manifold& m)
{
    // Swap A and B
    std::swap(m.A, m.B);

    // Reuse Circle vs Rect
    bool collided = CirclevsRect(m);

    // Flip normal so it points A → B
    m.normal = -m.normal;

    return collided;
}


/*
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
    const float percent = 0.2f;
    const float slop = 0.01f;

    float totalinvMass = m.A->invMass + m.B->invMass;
    if (totalinvMass == 0)
        return;

    Vec2f correction = m.normal * (m.penetration * percent / totalinvMass);

    m.A->position -= correction * m.A->invMass;
    m.B->position += correction * m.B->invMass;
}

using CollisionFn = bool(*)(Manifold&);

CollisionFn collisionTable[2][2] =
{
    {CirclevsCircle, CirclevsRect},
    {RectvsCircle, nullptr}
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