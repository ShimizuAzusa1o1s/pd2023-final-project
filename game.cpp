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

//Game event declaration.
SDL_Event Game::event;

//Colliders declaration.
std::vector< ColliderComponent* > Game::colliders;

//Game status indicator.
bool isTagRunning = true;
int gameTime = 0;

//Entity component system manager declaration.
Manager manager;

//Group labels declaration.
enum groupLabels : std::size_t{
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders,
    groupStatsObject
};

//Players declaration.
auto& player( manager.addEntity() );
auto& players( manager.getGroup( groupPlayers ) );

//Enemies declaration.
auto& enemy( manager.addEntity() );
auto& enemies( manager.getGroup( groupEnemies ) );

//Player status object declaration.
auto& heart1( manager.addEntity() );
auto& heart2( manager.addEntity() );
auto& heart3( manager.addEntity() );
auto& gameOver( manager.addEntity() );
auto& stats( manager.getGroup( groupStatsObject ) );

//Map objects declaration.
auto& eastWall( manager.addEntity() );
auto& westWall( manager.addEntity() );
auto& northWall( manager.addEntity() );
auto& southWall( manager.addEntity() );
auto& tiles( manager.getGroup( groupMap ) );

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
        heart1.addComponent< TransformComponent >( 830, 54 );
        heart1.addComponent< SpriteComponent >( "assets/HealthPoint.bmp" );
        heart1.addGroup( groupStatsObject );
        heart2.addComponent< TransformComponent >( 830, 108 );
        heart2.addComponent< SpriteComponent >( "assets/HealthPoint.bmp" );
        heart2.addGroup( groupStatsObject );
        heart3.addComponent< TransformComponent >( 830, 162 );
        heart3.addComponent< SpriteComponent >( "assets/HealthPoint.bmp" );
        heart3.addGroup( groupStatsObject );

        //Add gameover banner to the ECS manager.
        gameOver.addComponent< TransformComponent >( 165, 290, 94, 16, 5 );
        gameOver.addComponent< SpriteComponent >();
        gameOver.addGroup( groupStatsObject );

        //Add enemy to the ECS manager.
        enemy.addComponent< TransformComponent >( 384, 15 );
        enemy.addComponent< SpriteComponent >( "assets/WhiteCross.bmp" );
        enemy.addComponent< ColliderComponent >( "Enemy" );
        enemy.addComponent< StatsComponent >( 10 );
        enemy.addGroup( groupEnemies );

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
    //Update the status of game object.
    manager.refresh();
    manager.update();
    //Stop if the game had ended.
    if( !isTagRunning ) return;

    //Timer, recorded second by second.
    if( SDL_GetTicks() > 1000 * gameTime ){
        gameTime++;
        //Add cooldown to ensure that no overshooting in the same second.
        enemy.getComponent< StatsComponent >().skillCooldown = true;
        //Invincible time deducted.
        if( player.getComponent< StatsComponent >().invincibleTime != 0 ){
            player.getComponent< StatsComponent >().invincibleTime--;
        }
        else{
            player.getComponent< SpriteComponent >().setTexture( "assets/CDot.bmp" );
        }
    }

    //Add enemy every 10 seconds.
    if( gameTime % 2 == 0 && enemy.getComponent< StatsComponent >().skillCooldown == true ){
        //Enemy set in cooldown
        enemy.getComponent< StatsComponent >().skillCooldown = false;
        //Add new bullets.
        //Bullets increase while their speed decrease.
        for( int i = 0; i < gameTime / 2; i++ ){
            auto& bullet( manager.addEntity() );
            //Bullets are generated at enemy's position.
            bullet.addComponent< TransformComponent >
            ( enemy.getComponent< TransformComponent >().position.x,
            enemy.getComponent< TransformComponent >().position.y, 10, 10, 2 );
            bullet.addComponent< SpriteComponent >( "assets/Bullet.bmp" );
            bullet.addComponent< ColliderComponent >( "SpreadEnemySummon" );
            bullet.addComponent< StatsComponent >( 5 );
            bullet.addGroup( groupEnemies );
            //Calculating initial direction of bullets.
            Vector2D direction( 0, 0 );
            if( ( gameTime / 2 ) % 2 == 1 ){
                int bulletCnt = ( gameTime - 1 ) / 2;
                direction.x = 50 * ( i - ( bulletCnt - 1 ) / 2 );
                direction.y = 50 * bulletCnt;
            }
            else{
                int bulletCnt = gameTime / 2;
                direction.x = 50 * ( i - bulletCnt / 2 ); 
                direction.y = 50 * bulletCnt;
            } 
            direction.unitVector();
            //Spread bullets in arc mode.
            bullet.getComponent< TransformComponent >().velocity.x = direction.x;
            bullet.getComponent< TransformComponent >().velocity.y = direction.y;
            bullet.getComponent< TransformComponent >().speed = 3;
        }
    }

    //Wall rebound evaluation.
    for( auto cc : colliders ){
        if( Collision::AABB( player.getComponent< ColliderComponent >(), *cc ) ){
            //Player hit vertical wall.
            if( cc->tag == "eWall" ){
                player.getComponent< TransformComponent >().position.x -=
                player.getComponent< TransformComponent >().speed;
            }
            else if( cc->tag == "wWall" ){
                player.getComponent< TransformComponent >().position.x +=
                player.getComponent< TransformComponent >().speed;
            }
            //Player hit horizontal wall.
            if( cc->tag == "nWall" ){
                player.getComponent< TransformComponent >().position.y +=
                player.getComponent< TransformComponent >().speed;
            }
            else if( cc->tag == "sWall" ){
                player.getComponent< TransformComponent >().position.y -=
                player.getComponent< TransformComponent >().speed;
            }
            //Player hits enemy.
            if( ( cc->tag == "Enemy" || cc->tag == "EnemySummon" ) &&
                player.getComponent< StatsComponent >().invincibleTime == 0 ){
                //Player hit, damaged.
                player.getComponent< StatsComponent >().healthPoint--;
                player.getComponent< StatsComponent >().invincibleTime = 2;
                player.getComponent< SpriteComponent >().setTexture( "assets/InvincibleCDot.bmp" );
                //healthPoint deducted.
                switch( player.getComponent< StatsComponent >().healthPoint ){
                    case 2:
                        heart1.getComponent< SpriteComponent >().setTexture( "assets/DamagedHealthPoint.bmp" );
                        break;
                    case 1:
                        heart2.getComponent< SpriteComponent >().setTexture( "assets/DamagedHealthPoint.bmp" );
                        break;
                    case 0:
                        //healthPoint is used up, gameover.
                        heart3.getComponent< SpriteComponent >().setTexture( "assets/DamagedHealthPoint.bmp" );
                        gameOver.getComponent< SpriteComponent >().setTexture( "assets/GameOver.bmp" );
                        player.destroy();
                        for( auto& e : enemies ){
                            e->destroy();
                        }
                        isTagRunning = false;
                        break;
                }
            }
        }
        //Enemy collision.
        for( auto& e: enemies ){
            if( Collision::AABB( e->getComponent< ColliderComponent >(), *cc ) ){
                if( cc->tag == "eWall" || cc->tag == "wWall" ){
                    e->getComponent< TransformComponent >().velocity.x *= -1;
                }
                if( cc->tag == "nWall" || cc->tag == "sWall" ){
                    e->getComponent< TransformComponent >().velocity.y *= -1;
                }
            }
        }
    }
}

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

//Add map tiles.
void Game::addTile( int id, int x, int y ){
    auto& tile( manager.addEntity() );
    tile.addComponent< TileComponent >( x, y, 32, 32, id );
    tile.addGroup( groupMap );
}