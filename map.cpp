#include "map.h"
#include "game.h"
#include <fstream>
#include <cctype>

Map::Map() {}

Map::~Map() {}

// Read a size_x * size_y grid of tile IDs (one digit each),
// and add one tile for each ID. Each tile is 32 * 32 pixels.
// Any non-digit character, such as ',', ' ', '\r', or '\n', is a separator.
void Map::load_map(std::string path, int size_x, int size_y) {
    char tile = 0;
    std::ifstream map_file;
    map_file.open(path);
    if (!map_file.is_open()) {
        std::cerr << "Cannot open the map file " << path << std::endl;
        return;
    }

    for (int j = 0; j < size_y; j++) {
        for (int i = 0; i < size_x; i++) {
            // Skip the separators before the next digit.
            while (map_file.get(tile) && !isdigit(tile)) {}
            if (!map_file) {
                std::cerr << "The map file " << path << " has too few tiles." << std::endl;
                return;
            }
            Game::add_tile(tile - '0', i * 32, j * 32);
        }
    }

    map_file.close();
}
