#pragma once
#include <SDL2/SDL.h>
#include "components.h"
#include "../texture_manager.h"

class SpriteComponent : public Component {
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect src_rect, dest_rect;

public:
    // Constructors.
    SpriteComponent() = default;
    SpriteComponent(const char *path) {
        set_texture(path);
    }

    // Destructor.
    ~SpriteComponent() {
        SDL_DestroyTexture(texture);
    }

    // Set the component's image.
    void set_texture(const char *path) {
        this->texture = TextureManager::load_texture(path);
    }

    // Initialize the component.
    void init() override {
        this->transform = &entity->get_component<TransformComponent>();
        this->src_rect.x = this->src_rect.y = 0;
        this->src_rect.w = transform->width;
        this->src_rect.h = transform->height;
    }

    // Update the render position.
    void update() override {
        this->dest_rect.x = static_cast<int>(this->transform->position.x);
        this->dest_rect.y = static_cast<int>(this->transform->position.y);
        this->dest_rect.w = transform->width * transform->scale;
        this->dest_rect.h = transform->height * transform->scale;
    }

    // Draw the texture.
    void draw() override {
        TextureManager::draw_texture(this->texture, this->src_rect, this->dest_rect);
    }
};
