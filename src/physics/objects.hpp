#pragma once

#include "../util/config.hpp"

// ========== Physics ==========

enum class ShapeType {Circle, Rect};

struct Circle {float radius;};
struct Rect {Vec2f halfDim;};

struct Object
{
    Vec2f position;
    Vec2f velocity;
    Vec2f force;
    float invMass;
    float restitution;

    ShapeType shape;
    union
    {
        Circle circle;
        Rect rect;
    };
};

struct Manifold
{
    Object* A;
    Object* B;
    float penetration;
    Vec2f normal;
};

Object MTMCircle(const Vec2f& pos, float mass, float radius, float rest = 0.6f)
{
    Object o{};
    o.position = pos;
    o.velocity = {0.0f, 0.0f};
    o.force = {0.0f, 0.0f};

    o.invMass = mass;
    o.restitution = rest;

    o.shape = ShapeType::Circle;
    o.circle.radius = radius;

    return o;
}

Object MTMRect(const Vec2f& pos, float mass, Vec2f halfDim, float rest = 0.6f)
{
    Object o{};
    o.position = pos;
    o.velocity = {0.0f, 0.0f};
    o.force = {0.0f, 0.0f};

    o.invMass = mass;
    o.restitution = rest;

    o.shape = ShapeType::Rect;
    o.rect.halfDim = halfDim;

    return o;
}