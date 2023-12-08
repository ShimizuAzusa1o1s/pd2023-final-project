#pragma once
#include "game.h"

class TextureManager
{
    public:
        //Loading the game object's texture.
        static SDL_Texture* loadTexture( const char* filename);
        //Drawing the game object on the window.
        static void draw( SDL_Texture* tex, SDL_Rect src, SDL_Rect dest );
};