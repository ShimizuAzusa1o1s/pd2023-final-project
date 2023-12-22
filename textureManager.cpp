#include "textureManager.h"

SDL_Texture* TextureManager::loadTexture( const char* filename){
    //Difference between SDL_Surface & SDL_Texture:
    //SDL_Surface is used in software rendering,
    //while SDL_Texture is used in hardware rendering.
    SDL_Surface* tempImage = SDL_LoadBMP( filename );
    SDL_Texture* tarTexture = SDL_CreateTextureFromSurface( Game::renderer, tempImage );
    //When the texture is rendered, free the surface.
    SDL_FreeSurface( tempImage );
    
    return tarTexture;
}

void TextureManager::drawTexture( SDL_Texture* tex, SDL_Rect src, SDL_Rect dest ){
    //Drawing the designated texture to the game screen.
    SDL_RenderCopy( Game::renderer, tex, &src, &dest );
}