#pragma once
#include "game.h"

class Map{
    private:
        SDL_Rect srcRect, destRect;
        SDL_Texture* dirt;
        SDL_Texture* grass;
        SDL_Texture* water;
        int map[20][25];
    public:
        Map();
        ~Map();
        void loadMap( int arr[20][25] );
        void drawMap();
};