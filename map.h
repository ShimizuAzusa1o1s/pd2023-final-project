#pragma once
#include <string>

class Map{
    private:
        
    public:
        //Constructor.
        Map();
        //Destructor.
        ~Map();
        //Static member variables & functions.
        static void loadMap( std::string path, int sizeX, int sizeY );
};