#include "texture_manager.h"

SDL_Texture *TextureManager::load_texture(const char *file_name) {
    // Difference between SDL_Surface and SDL_Texture:
    // SDL_Surface is used in software rendering,
    // while SDL_Texture is used in hardware rendering.
    SDL_Surface *temp_image = SDL_LoadBMP(file_name);
    SDL_Texture *target_texture = SDL_CreateTextureFromSurface(Game::renderer, temp_image);
    // When the texture is created, free the surface.
    SDL_FreeSurface(temp_image);

    return target_texture;
}

void TextureManager::draw_texture(
    SDL_Texture *texture, 
    SDL_Rect src, SDL_Rect dest) {
        
    // Draw the designated texture on the game screen.
    SDL_RenderCopy(Game::renderer, texture, &src, &dest);
}
