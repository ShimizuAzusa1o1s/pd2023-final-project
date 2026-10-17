#pragma once
#include "game.h"

class TextureManager {
public:
    // Load the game object's texture.
    static SDL_Texture *load_texture(const char *file_name);
    // Draw the game object on the window.
    static void draw_texture(SDL_Texture *texture, SDL_Rect src, SDL_Rect dest);
};
