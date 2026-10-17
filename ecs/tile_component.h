#pragma once
#include <SDL2/SDL.h>
#include "ecs.h"
#include "transform_component.h"
#include "sprite_component.h"

class TileComponent : public Component {
public:
    // Member variables.
    TransformComponent *transform;
    SpriteComponent *sprite;
    SDL_Rect tile_rect;
    int tile_id;
    const char *path;

    // Constructors.
    TileComponent() = default;
    TileComponent(int x, int y, int width, int height, int id) {
        this->tile_rect.x = x;
        this->tile_rect.y = y;
        this->tile_rect.w = width;
        this->tile_rect.h = height;
        this->tile_id = id;
        // Determine the texture by the ID number.
        // An unknown ID uses the background texture.
        switch (tile_id) {
            case 0:
                this->path = "assets/background_pink.bmp";
                break;
            default:
                std::cerr << "Unknown tile ID " << tile_id << ", the background is used." << std::endl;
                this->path = "assets/background_pink.bmp";
                break;
        }
    }

    // Initialize the component.
    void init() override {
        entity->add_component<TransformComponent>(
            (float)tile_rect.x, (float)tile_rect.y,
            tile_rect.w, tile_rect.h, 1
        );
        transform = &entity->get_component<TransformComponent>();
        entity->add_component<SpriteComponent>(path);
        sprite = &entity->get_component<SpriteComponent>();
    }
};
