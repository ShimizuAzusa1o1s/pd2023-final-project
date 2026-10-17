#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "components.h"

class ColliderComponent : public Component {
public:
    // Member variables.
    SDL_Rect collider;
    std::string tag;
    TransformComponent *transform;

    // Constructor.
    ColliderComponent(std::string tag) { this->tag = tag; }

    // Initialize the component.
    void init() override {
        if (!entity->has_component<TransformComponent>()) {
            entity->add_component<TransformComponent>();
        }
        transform = &entity->get_component<TransformComponent>();

        Game::colliders.push_back(this);
    }

    // Update the position of the collider.
    void update() override {
        this->collider.x = static_cast<int>(transform->position.x);
        this->collider.y = static_cast<int>(transform->position.y);
        this->collider.w = transform->width * transform->scale;
        this->collider.h = transform->height * transform->scale;
    }
};
