#pragma once
#include "game.h"

class GameObject{
    private:
        int xpos;
        int ypos;
        SDL_Renderer* renderer;
        SDL_Texture* objectTexture;
        SDL_Rect srcRect, destRect; //Used to desginated a certain position of rectangle space for rendering.
    public:
        GameObject( const char* textureSheet, int x, int y );
        ~GameObject();

        void update();
        void render();
};