#include "map.h"
#include "game.h"
#include <fstream>

//Below is a two-dimensional matrix containing map information.
//0: water, 1: grass, 2: dirt.

Map::Map(){

}

Map::~Map(){
    
}

void Map::loadMap( std::string path, int sizeX, int sizeY ){
    char tile;
    std::ifstream mapFile;
    mapFile.open( path );

    for( int j = 0; j < sizeY; j++ ){
        for( int i = 0; i < sizeX; i++ ){
            mapFile.get( tile );
            Game::addTile( atoi(&tile), i * 32, j * 32 );
            mapFile.ignore();
        }
    }

    mapFile.close();
}

