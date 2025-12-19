#pragma once

#include "../physics/objects.hpp"

// ========== Rendering ==========

void DrawCircle(const Object& obj)
{
    const int segments = 100;
    glBegin(GL_TRIANGLE_FAN);

    glVertex2d(obj.position.x, obj.position.y);
    for (int i = 0; i <= segments; ++i) {
        float angle = (float)i / segments * 2.0f * pi;
        float x = obj.position.x + obj.circle.radius * cos(angle);
        float y = obj.position.y + obj.circle.radius * sin(angle);

        glVertex2f(x, y);
    }

    glEnd();
}

void DrawRect(const Object& obj)
{
    Vec2f min = obj.position - obj.rect.halfDim;
    Vec2f max = obj.position + obj.rect.halfDim;

    glBegin(GL_QUADS);
    glVertex2f(min.x, min.y);
    glVertex2f(max.x, min.y);
    glVertex2f(max.x, max.y);
    glVertex2f(min.x, max.y);

    glEnd();
}

void Render(const std::vector<Object>& objs)
{
    glClear(GL_COLOR_BUFFER_BIT);

    for (const Object& obj : objs) {
        switch (obj.shape) {
            case ShapeType::Circle:
                DrawCircle(obj);
                break;

            case ShapeType::Rect:
                DrawRect(obj);
                break;
        }
    }
}