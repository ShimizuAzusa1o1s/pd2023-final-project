#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

class ColliderComponent;

class Game{
    private:
        bool isRunning; //Indicator of the game-running status.
        SDL_Window* window;
    public:
        //Static member variables.
        static void addTile( int id, int x, int y );
        static SDL_Renderer* renderer;
        static SDL_Event event;
        static std::vector< ColliderComponent* > colliders;
        //Constructors.
        Game();
        //Destructor.
        ~Game(){}
        //Member functions.
        void initialize( const char* title, int xpos, int ypos, int width, int height, bool fullScreen );
        void handleEvents();
        void update();
        void render();
        void clean();
        bool running();
};