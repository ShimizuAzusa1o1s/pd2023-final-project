#include "game.h"
#include "textureManager.h"
#include "map.h"
#include "ECS/components.h"
#include "vector2D.h"
#include "ECS/collision.h"

//Map object declaration.
Map* map;

//Texture manager declaration.
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

std::vector< ColliderComponent* > Game::colliders;

//Entity component system manager declaration.
Manager manager;
auto& player( manager.addEntity() );
auto& wall( manager.addEntity() );

Game::Game(){}

//Initialize elements composing of the game. e.g: Game window, renderer, etc.
void Game::initialize( const char* title, int xpos, int ypos, int width, int height, bool fullScreen ){
    int flags = 0;
    if( fullScreen ) flags = SDL_WINDOW_FULLSCREEN;

    if( SDL_Init(SDL_INIT_EVERYTHING) == 0 ){ //Check if the game is initialized successfully.
        std::cout << "Subsystem initialized!" << std::endl;

        //Create game window.
        this->window = SDL_CreateWindow( title, xpos, ypos, width, height, flags );
        if( window ) std::cout << "Window created!" << std::endl;

        //Create game renderer.
        this->renderer = SDL_CreateRenderer( window, -1, 0 );
        if( renderer ){
            SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
            std::cout << "Renderer created!" << std::endl;
        } 

        //Generating game maps.
        Map::loadMap( "assets/Map.txt", 25, 20 );

        //Add components to the ECS manager.
        player.addComponent< TransformComponent >();
        player.addComponent< SpriteComponent >( "assets/CDot.bmp" );
        player.addComponent< KeyboardController >();
        player.addComponent< ColliderComponent >( "player" );

        wall.addComponent< TransformComponent >( 300, 500, 300, 20, 1 );
        wall.addComponent< SpriteComponent >( "assets/Wall.bmp" );
        wall.addComponent< ColliderComponent >( "wall" );

        this->isRunning = true;
    }
    //If the game cannot be opened correctly, stop.
    else this->isRunning = false;
}

//Handling game events happening in the game.
void Game::handleEvents(){
    //Obtain access to the given events. 
    SDL_PollEvent( &event );

    //Operating given events depending on its type.
    switch( event.type ){
        case SDL_QUIT:
            this->isRunning = false;
            break;
        default:
            break;
    }
}

//Update the elements of the game entities.
void Game::update(){
    manager.refresh();
    manager.update();
    for( auto cc : colliders ){
        Collision::AABB( player.getComponent< ColliderComponent >(), *cc );
    }
}

//Update the game screen in accordance with the game entities.
void Game::render(){
    SDL_RenderClear( renderer );
    manager.draw();
    SDL_RenderPresent( renderer );
}

//Clean the whole game when it's closed.
void Game::clean(){
    SDL_DestroyWindow( this->window );
    SDL_DestroyRenderer( this->renderer );
    SDL_Quit();
    std::cout << "Game cleaned!" << std::endl;
}

//Check if the game is going to keep running in the whole game loop.
bool Game::running(){
    return this->isRunning; 
}

void Game::addTile( int id, int x, int y ){
    auto& tile( manager.addEntity() );
    tile.addComponent< TileComponent >( x, y, 32, 32, id);
}