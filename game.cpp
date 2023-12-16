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
auto& enemy( manager.addEntity() );

auto& heart1( manager.addEntity() );
auto& heart2( manager.addEntity() );
auto& heart3( manager.addEntity() );

auto& eastWall( manager.addEntity() );
auto& westWall( manager.addEntity() );
auto& northWall( manager.addEntity() );
auto& southWall( manager.addEntity() );

enum groupLabels : std::size_t{
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders,
    groupStatsObject
};

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

        //Add player to the ECS manager.
        player.addComponent< TransformComponent >( 384, 526 );
        player.addComponent< SpriteComponent >( "assets/CDot.bmp" );
        player.addComponent< KeyboardController >();
        player.addComponent< ColliderComponent >( "Player" );
        player.addComponent< StatsComponent >( 3 );
        player.addGroup( groupPlayers );

        //Add health point to the ECS manager.
        heart1.addComponent< TransformComponent >( 334, 654 );
        heart1.addComponent< SpriteComponent >( "assets/HealthPoint.bmp" );
        heart1.addGroup( groupStatsObject );
        heart2.addComponent< TransformComponent >( 384, 654 );
        heart2.addComponent< SpriteComponent >( "assets/HealthPoint.bmp" );
        heart2.addGroup( groupStatsObject );
        heart3.addComponent< TransformComponent >( 434, 654 );
        heart3.addComponent< SpriteComponent >( "assets/HealthPoint.bmp" );
        heart3.addGroup( groupStatsObject );

        //Add enemy to the ECS manager.
        enemy.addComponent< TransformComponent >( 384, 15 );
        enemy.addComponent< SpriteComponent >( "assets/WhiteCross.bmp" );
        enemy.addComponent< ColliderComponent >( "Enemy" );
        enemy.addGroup( groupEnemies );
        enemy.getComponent< TransformComponent >().velocity.x = 1;
        enemy.getComponent< TransformComponent >().velocity.y = 1;
        enemy.getComponent< TransformComponent >().speed = 2;

        //Add wall to the ECS manager.
        eastWall.addComponent< TransformComponent >( 790, 0, 10, 640, 1 );
        eastWall.addComponent< SpriteComponent >( "assets/VerticalWall.bmp" );
        eastWall.addComponent< ColliderComponent >( "eWall" );
        eastWall.addGroup( groupMap );
        westWall.addComponent< TransformComponent >( 0, 0, 10, 640, 1 );
        westWall.addComponent< SpriteComponent >( "assets/VerticalWall.bmp" );
        westWall.addComponent< ColliderComponent >( "wWall" );
        westWall.addGroup( groupMap );
        northWall.addComponent< TransformComponent >( 0, 0, 800, 10, 1 );
        northWall.addComponent< SpriteComponent >( "assets/HorizontalWall.bmp" );
        northWall.addComponent< ColliderComponent >( "nWall" );
        northWall.addGroup( groupMap );
        southWall.addComponent< TransformComponent >( 0, 630, 800, 10, 1 );
        southWall.addComponent< SpriteComponent >( "assets/HorizontalWall.bmp" );
        southWall.addComponent< ColliderComponent >( "sWall" );
        southWall.addGroup( groupMap );

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
    Vector2D direction( 
        player.getComponent< TransformComponent >().position.x - enemy.getComponent< TransformComponent >().position.x,
        player.getComponent< TransformComponent >().position.y - enemy.getComponent< TransformComponent >().position.y
    );
    direction.unitVector();
    enemy.getComponent< TransformComponent >().velocity.x = direction.x;
    enemy.getComponent< TransformComponent >().velocity.y = direction.y;

    //Wall rebound evaluation.
    for( auto cc : colliders ){
        //Player collision.
        if( Collision::AABB( player.getComponent< ColliderComponent >(), *cc ) ){
            if( cc->tag == "eWall" ){
                player.getComponent< TransformComponent >().position.x -=
                player.getComponent< TransformComponent >().speed;
            }
            if( cc->tag == "wWall" ){
                player.getComponent< TransformComponent >().position.x +=
                player.getComponent< TransformComponent >().speed;
            }
            if( cc->tag == "nWall" ){
                player.getComponent< TransformComponent >().position.y +=
                player.getComponent< TransformComponent >().speed;
            }
            if( cc->tag == "sWall" ){
                player.getComponent< TransformComponent >().position.y -=
                player.getComponent< TransformComponent >().speed;
            }
            if( cc->tag == "Enemy" ){
                player.getComponent< TransformComponent >().velocity * 0;
                Vector2D respawnPoint( 384, 526 );
                player.getComponent< StatsComponent >().healthPoint--;
                player.getComponent< TransformComponent >().position = respawnPoint;
                switch( player.getComponent< StatsComponent >().healthPoint ){
                    case 2:
                        heart1.getComponent< SpriteComponent >().setTexture( "assets/DamagedHealthPoint.bmp" );
                        break;
                    case 1:
                        heart2.getComponent< SpriteComponent >().setTexture( "assets/DamagedHealthPoint.bmp" );
                        break;
                    case 0:
                        heart3.getComponent< SpriteComponent >().setTexture( "assets/DamagedHealthPoint.bmp" );
                        break;
                }
            }
        }
        //Enemy collision.
        if( Collision::AABB( enemy.getComponent< ColliderComponent >(), *cc ) ){
            if( cc->tag == "eWall" || cc->tag == "wWall" ){
                enemy.getComponent< TransformComponent >().velocity.x *= -1;
            }
            if( cc->tag == "nWall" || cc->tag == "sWall" ){
                enemy.getComponent< TransformComponent >().velocity.y *= -1;
            }
        }
    }
}

auto& tiles( manager.getGroup( groupMap ) );
auto& players( manager.getGroup( groupPlayers ) );
auto& enemies( manager.getGroup( groupEnemies ) );
auto& stats( manager.getGroup( groupStatsObject ) );

//Update the game screen in accordance with the game entities.
void Game::render(){
    SDL_RenderClear( renderer );

    for( auto& t : tiles ){
        t->draw();
    }
    for( auto& p : players ){
        p->draw();
    }
    for( auto& e : enemies ){
        e->draw();
    }
    for( auto& s : stats ){
        s->draw();
    }

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
    tile.addComponent< TileComponent >( x, y, 32, 32, id );
    tile.addGroup( groupMap );
}