#pragma once
#include <string>

class Map {
public:
    // Constructor.
    Map();
    // Destructor.
    ~Map();
    // Static member functions.
    static void load_map(std::string path, int size_x, int size_y);
};
