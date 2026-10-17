#include "map.h"
#include "game.h"
#include <fstream>

Map::Map() {}

Map::~Map() {}

// Read a size_x * size_y grid of comma-separated tile IDs (one digit each),
// and add one tile for each ID. Each tile is 32 * 32 pixels.
void Map::load_map(std::string path, int size_x, int size_y) {
    char tile;
    std::ifstream map_file;
    map_file.open(path);

    for (int j = 0; j < size_y; j++) {
        for (int i = 0; i < size_x; i++) {
            map_file.get(tile);
            Game::add_tile(tile - '0', i * 32, j * 32);
            map_file.ignore();
        }
    }

    map_file.close();
}
