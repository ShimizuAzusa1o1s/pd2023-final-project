#pragma once
#include "components.h"
#include "../vector_2d.h"

// Component used to record the entity's position.
class TransformComponent : public Component {
public:
    // Member variables.
    // The upper-left corner is (0, 0), and a vector expresses the position.
    Vector2D position;
    Vector2D velocity;
    int height = 16;
    int width = 16;
    int scale = 2;
    float speed = 3;

    // Constructors.
    TransformComponent() {
        this->position.zero();
    }
    TransformComponent(int scale) {
        this->position.zero();
        this->scale = scale;
    }
    TransformComponent(float x, float y) {
        this->position.x = x;
        this->position.y = y;
    }
    TransformComponent(float x, float y, int width, int height, int scale) {
        this->position.x = x;
        this->position.y = y;
        this->width = width;
        this->height = height;
        this->scale = scale;
    }

    // Initialize the component.
    void init() override {
        this->velocity.zero();
    }

    // Update the entity's position.
    void update() override {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }
};
