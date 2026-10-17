#pragma once
#include <string>
#include <algorithm>
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

    // Destructor. Remove this collider from the global list,
    // so that the collision test never uses a destroyed collider.
    ~ColliderComponent() {
        auto &colliders = Game::colliders;
        colliders.erase(std::remove(colliders.begin(), colliders.end(), this), colliders.end());
    }

    // Initialize the component.
    void init() override {
        if (!entity->has_component<TransformComponent>()) {
            entity->add_component<TransformComponent>();
        }
        transform = &entity->get_component<TransformComponent>();

        Game::colliders.push_back(this);
        // Set the rectangle now, so that an entity added in the middle of a frame
        // has a valid collider in the collision test of the same frame.
        update();
    }

    // Update the position of the collider.
    void update() override {
        this->collider.x = static_cast<int>(transform->position.x);
        this->collider.y = static_cast<int>(transform->position.y);
        this->collider.w = transform->width * transform->scale;
        this->collider.h = transform->height * transform->scale;
    }
};
