#include "game.h"
#include "game.cpp"
#include "textureManager.cpp"
#include "map.cpp"
#include "vector2D.cpp"
#include "ECS/collision.cpp"
#include "ECS/ECS.cpp"

//Controling max frame rate.
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

//Game declaration.
Game* game = nullptr;

int main( int argc, char* argv[] ){
    //Generating new game.
    game = new Game();
    uint32_t frameStart = 0;
    int frameTime = 0;

    //Initialize the game.
    game->initialize( "暈到咪咪冒冒小狗的下船大作戰！", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 640, false );
    while( game->running() ){
        //Obtain the start of the this game loop.
        frameStart = SDL_GetTicks();

        //Refresh the game in the order below:
        //1. Handling events occur in the game process.
        //2. Update the data in accordance with the game elements.
        //3. Refresh the screen such that it shows the current status of the game elements.
        game->handleEvents();
        game->update();
        game->render();

        //Cap the max frame rate.
        frameTime = SDL_GetTicks() - frameStart;
        if( FRAME_DELAY > frameTime ){
            SDL_Delay( FRAME_DELAY - frameTime );
        }
    }

    //While the game loop is done, clean the game.
    game->clean();
    return 0;
}